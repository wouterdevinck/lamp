// Adapted from https://github.com/espressif/esp-azure/blob/master/port/src/tlsio_esp_tls.c
// Added support for ATECC608A backed signatures, private key stays in crypto device.

// Note: this implementation depends on a modified version of esp-tls, which 
// allows passing mbedtls_pk_context to the underlying mbedtls library, rather 
// than the private key in PEM format in a buffer.

// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license.

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "tlsio_pal.h"
#include "azure_c_shared_utility/gballoc.h"
#include "azure_c_shared_utility/xlogging.h"
#include "azure_c_shared_utility/agenttime.h"
#include "azure_c_shared_utility/singlylinkedlist.h"
#include "azure_c_shared_utility/crt_abstractions.h"
#include "azure_c_shared_utility/tlsio_options.h"
#include "cryptoauthlib.h"
#include "mbedtls/atca_mbedtls_wrap.h"
#include "esp_tls.h"

#define MAX_VALID_PORT 0xffff
#define TLSIO_RECEIVE_BUFFER_SIZE 64

/*uint8_t atca_io_protection_key[32] = {
    0x37, 0x80, 0xe6, 0x3d, 0x49, 0x68, 0xad, 0xe5,
    0xd8, 0x22, 0xc0, 0x13, 0xfc, 0xc3, 0x23, 0x84,
    0x5d, 0x1b, 0x56, 0x9f, 0xe7, 0x05, 0xb6, 0x00,
    0x06, 0xfe, 0xec, 0x14, 0x5a, 0x0d, 0xb1, 0xe3
};*/

int atca_mbedtls_ecdh_slot_cb(void) {
  // ESP_LOGI("tlsio", "atca_mbedtls_ecdh_slot_cb");
  // return 0xFFFF; // TODO Use tempkey slot
  return 2;
}

int atca_mbedtls_ecdh_ioprot_cb(uint8_t secret[32]) {
  // ESP_LOGI("tlsio", "atca_mbedtls_ecdh_ioprot_cb");
  // memcpy(secret, atca_io_protection_key, 32);
  return 0;
}

typedef struct {
  unsigned char* bytes;
  size_t size;
  size_t unsent_size;
  ON_SEND_COMPLETE on_send_complete;
  void* callback_context;
} PENDING_TRANSMISSION;

typedef enum TLSIO_STATE_TAG {
  TLSIO_STATE_CLOSED,
  TLSIO_STATE_INIT,
  TLSIO_STATE_OPEN,
  TLSIO_STATE_ERROR,
} TLSIO_STATE;

bool is_an_opening_state(TLSIO_STATE state) {
  return state == TLSIO_STATE_INIT;
}

typedef struct TLS_IO_INSTANCE_TAG {
  ON_BYTES_RECEIVED on_bytes_received;
  ON_IO_ERROR on_io_error;
  ON_IO_OPEN_COMPLETE on_open_complete;
  void* on_bytes_received_context;
  void* on_io_error_context;
  void* on_open_complete_context;
  esp_tls_cfg_t esp_tls_cfg;
  esp_tls_t   *esp_tls_handle;
  TLSIO_STATE tlsio_state;
  uint16_t port;
  char* hostname;
  SINGLYLINKEDLIST_HANDLE pending_transmission_list;
  TLSIO_OPTIONS options;
} TLS_IO_INSTANCE;

static void enter_tlsio_error_state(TLS_IO_INSTANCE* tls_io_instance) {
  if (tls_io_instance->tlsio_state != TLSIO_STATE_ERROR) {
    tls_io_instance->tlsio_state = TLSIO_STATE_ERROR;
    tls_io_instance->on_io_error(tls_io_instance->on_io_error_context);
  }
}

static bool process_and_destroy_head_message(TLS_IO_INSTANCE* tls_io_instance, IO_SEND_RESULT send_result) {
  bool result;
  LIST_ITEM_HANDLE head_pending_io;
  if (send_result == IO_SEND_ERROR) {
    enter_tlsio_error_state(tls_io_instance);
  }
  head_pending_io = singlylinkedlist_get_head_item(tls_io_instance->pending_transmission_list);
  if (head_pending_io != NULL) {
    PENDING_TRANSMISSION* head_message = (PENDING_TRANSMISSION*)singlylinkedlist_item_get_value(head_pending_io);
    if (singlylinkedlist_remove(tls_io_instance->pending_transmission_list, head_pending_io) != 0) {
      enter_tlsio_error_state(tls_io_instance);
      LogError("Failed to remove message from list");
    }
    head_message->on_send_complete(head_message->callback_context, send_result);
    free(head_message->bytes);
    free(head_message);
    result = true;
  } else {
    result = false;
  }
  return result;
}

static void internal_close(TLS_IO_INSTANCE* tls_io_instance) {
  esp_tls_conn_delete(tls_io_instance->esp_tls_handle);
  while (process_and_destroy_head_message(tls_io_instance, IO_SEND_CANCELLED));
  tls_io_instance->on_bytes_received = NULL;
  tls_io_instance->on_io_error = NULL;
  tls_io_instance->on_bytes_received_context = NULL;
  tls_io_instance->on_io_error_context = NULL;
  tls_io_instance->tlsio_state = TLSIO_STATE_CLOSED;
  tls_io_instance->on_open_complete = NULL;
  tls_io_instance->on_open_complete_context = NULL;
}

static void tlsio_esp_tls_destroy(CONCRETE_IO_HANDLE tls_io) {
  if (tls_io == NULL) {
    LogError("NULL tlsio");
  } else {
    TLS_IO_INSTANCE* tls_io_instance = (TLS_IO_INSTANCE*)tls_io;
    if (tls_io_instance->tlsio_state != TLSIO_STATE_CLOSED) {
      LogError("tlsio_esp_tls_destroy called while not in TLSIO_STATE_CLOSED.");
      internal_close(tls_io_instance);
    }
    if (tls_io_instance->hostname != NULL) {
      free(tls_io_instance->hostname);
    }
    tlsio_options_release_resources(&tls_io_instance->options);
    if (tls_io_instance->pending_transmission_list != NULL) {
      singlylinkedlist_destroy(tls_io_instance->pending_transmission_list);
    }
    free(tls_io_instance);
  }
}

static CONCRETE_IO_HANDLE tlsio_esp_tls_create(void* io_create_parameters) {
  TLS_IO_INSTANCE* result;
  if (io_create_parameters == NULL) {
    LogError("NULL tls_io_config");
    result = NULL;
  } else {
    TLSIO_CONFIG* tls_io_config = (TLSIO_CONFIG*)io_create_parameters;
    if (tls_io_config->hostname == NULL) {
      LogError("NULL tls_io_config->hostname");
      result = NULL;
    } else if (tls_io_config->port < 0 || tls_io_config->port > MAX_VALID_PORT) {
      LogError("tls_io_config->port out of range");
      result = NULL;
    } else {
      result = malloc(sizeof(TLS_IO_INSTANCE));
      if (result == NULL) {
        LogError("malloc failed");
      } else {
        int ms_result;
        memset(result, 0, sizeof(TLS_IO_INSTANCE));
        result->port = (uint16_t)tls_io_config->port;
        result->tlsio_state = TLSIO_STATE_CLOSED;
        result->hostname = NULL;
        result->pending_transmission_list = NULL;
        tlsio_options_initialize(&result->options, TLSIO_OPTION_BIT_TRUSTED_CERTS |
        TLSIO_OPTION_BIT_x509_RSA_CERT | TLSIO_OPTION_BIT_x509_ECC_CERT);
        result->esp_tls_handle = calloc(1, sizeof(esp_tls_t));
        if (result->esp_tls_handle == NULL) {
          LogError("malloc failed");
          tlsio_esp_tls_destroy(result);
          result = NULL;
        }
        ms_result = mallocAndStrcpy_s(&result->hostname, tls_io_config->hostname);
        if (ms_result != 0) {
          LogError("malloc failed");
          tlsio_esp_tls_destroy(result);
          result = NULL;
        } else {
          result->pending_transmission_list = singlylinkedlist_create();
          if (result->pending_transmission_list == NULL) {
            LogError("Failed singlylinkedlist_create");
            tlsio_esp_tls_destroy(result);
            result = NULL;
          }
        }
      }
    }
  }
  return (CONCRETE_IO_HANDLE)result;
}

static int tlsio_esp_tls_open_async(CONCRETE_IO_HANDLE tls_io,
  ON_IO_OPEN_COMPLETE on_io_open_complete, void* on_io_open_complete_context,
  ON_BYTES_RECEIVED on_bytes_received, void* on_bytes_received_context,
  ON_IO_ERROR on_io_error, void* on_io_error_context)
{
  int result;
  if (on_io_open_complete == NULL) {
    LogError("Required parameter on_io_open_complete is NULL");
    result = __FAILURE__;
  } else {
    if (tls_io == NULL) {
      result = __FAILURE__;
      LogError("NULL tlsio");
    } else {
      if (on_bytes_received == NULL) {
        LogError("Required parameter on_bytes_received is NULL");
        result = __FAILURE__;
      } else {
        if (on_io_error == NULL) {
          LogError("Required parameter on_io_error is NULL");
          result = __FAILURE__;
        } else {
          TLS_IO_INSTANCE* tls_io_instance = (TLS_IO_INSTANCE*)tls_io;
          if (tls_io_instance->tlsio_state != TLSIO_STATE_CLOSED) {
            LogError("Invalid tlsio_state. Expected state is TLSIO_STATE_CLOSED.");
            result = __FAILURE__;
          } else {
            // Note: modified here to pass ATECC608 context as private key
            mbedtls_pk_context pkey;
            if (atca_mbedtls_pk_init(&pkey, 1) != 0) {
              LogError("Failed to get key from ATCA device.");
              result = __FAILURE__;
            } else {
              tls_io_instance->on_bytes_received = on_bytes_received;
              tls_io_instance->on_bytes_received_context = on_bytes_received_context;
              tls_io_instance->on_io_error = on_io_error;
              tls_io_instance->on_io_error_context = on_io_error_context;
              tls_io_instance->on_open_complete = on_io_open_complete;
              tls_io_instance->on_open_complete_context = on_io_open_complete_context;
              tls_io_instance->esp_tls_cfg.non_block = true;
              if (tls_io_instance->options.x509_cert != NULL) {
                tls_io_instance->esp_tls_cfg.clientcert_pem_buf = (unsigned char *)tls_io_instance->options.x509_cert;
                tls_io_instance->esp_tls_cfg.clientcert_pem_bytes = strlen(tls_io_instance->options.x509_cert) + 1;                            
                tls_io_instance->esp_tls_cfg.clientkey = pkey; // Instead of clientkey_pem_buf & clientkey_pem_bytes
              }
              if (tls_io_instance->options.trusted_certs != NULL) {
                tls_io_instance->esp_tls_cfg.cacert_pem_buf = (unsigned char *)tls_io_instance->options.trusted_certs;
                tls_io_instance->esp_tls_cfg.cacert_pem_bytes = strlen(tls_io_instance->options.trusted_certs) + 1;
              }
              tls_io_instance->tlsio_state = TLSIO_STATE_INIT;
              result = 0;
            }
          }
        }
      }
    }
  }
  return result;
}

static int tlsio_esp_tls_close_async(CONCRETE_IO_HANDLE tls_io, ON_IO_CLOSE_COMPLETE on_io_close_complete, void* callback_context) {
  int result;
  if (tls_io == NULL) {
    LogError("NULL tlsio");
    result = __FAILURE__;
  } else {
    if (on_io_close_complete == NULL) {
      LogError("NULL on_io_close_complete");
      result = __FAILURE__;
    } else {
      TLS_IO_INSTANCE* tls_io_instance = (TLS_IO_INSTANCE*)tls_io;
      if (tls_io_instance->tlsio_state != TLSIO_STATE_OPEN && tls_io_instance->tlsio_state != TLSIO_STATE_ERROR) {
        LogInfo("tlsio_esp_tls_close has been called when in neither TLSIO_STATE_OPEN nor TLSIO_STATE_ERROR.");
      }
      if (is_an_opening_state(tls_io_instance->tlsio_state)) {
        tls_io_instance->on_open_complete(tls_io_instance->on_open_complete_context, IO_OPEN_CANCELLED);
      }
      internal_close(tls_io_instance);
      on_io_close_complete(callback_context);
      result = 0;
    }
  }
  return result;
}

static int dowork_read(TLS_IO_INSTANCE* tls_io_instance) {
  unsigned char buffer[TLSIO_RECEIVE_BUFFER_SIZE];
  int rcv_bytes = 0;
  if (tls_io_instance->tlsio_state == TLSIO_STATE_OPEN) {
    rcv_bytes = esp_tls_conn_read(tls_io_instance->esp_tls_handle, buffer, sizeof(buffer));
    while (rcv_bytes > 0) {
      tls_io_instance->on_bytes_received(tls_io_instance->on_bytes_received_context, buffer, rcv_bytes);
      rcv_bytes = esp_tls_conn_read(tls_io_instance->esp_tls_handle, buffer, sizeof(buffer));
    }
  }
  return rcv_bytes;
}

static int dowork_send(TLS_IO_INSTANCE* tls_io_instance) {
  LIST_ITEM_HANDLE first_pending_io = singlylinkedlist_get_head_item(tls_io_instance->pending_transmission_list);
  int write_result = 0;
  if (first_pending_io != NULL) {
    PENDING_TRANSMISSION* pending_message = (PENDING_TRANSMISSION*)singlylinkedlist_item_get_value(first_pending_io);
    uint8_t* buffer = ((uint8_t*)pending_message->bytes) + pending_message->size - pending_message->unsent_size;
    write_result = esp_tls_conn_write(tls_io_instance->esp_tls_handle, buffer, pending_message->unsent_size);
    if (write_result > 0) {
      pending_message->unsent_size -= write_result;
      if (pending_message->unsent_size == 0) {
        process_and_destroy_head_message(tls_io_instance, IO_SEND_OK);
      }
    } else {
      LogInfo("Error from SSL_write: %d", write_result);
    }
  }
  return write_result;
}

static void tlsio_esp_tls_dowork(CONCRETE_IO_HANDLE tls_io) {
  if (tls_io == NULL) {
    LogError("NULL tlsio");
  } else {
    TLS_IO_INSTANCE* tls_io_instance = (TLS_IO_INSTANCE*)tls_io;
    switch (tls_io_instance->tlsio_state) {
    case TLSIO_STATE_CLOSED:
      break;
    case TLSIO_STATE_INIT:
      {
        int result = esp_tls_conn_new_async(tls_io_instance->hostname, strlen(tls_io_instance->hostname), tls_io_instance->port, &tls_io_instance->esp_tls_cfg, tls_io_instance->esp_tls_handle);
        if (result == 1) {
          tls_io_instance->tlsio_state = TLSIO_STATE_OPEN;
          tls_io_instance->on_open_complete(tls_io_instance->on_open_complete_context, IO_OPEN_OK);
        } else if (result == -1) {
          tls_io_instance->tlsio_state = TLSIO_STATE_ERROR;
        }
      }
      break;
    case TLSIO_STATE_OPEN:
        if (dowork_read(tls_io_instance) < 0 && errno != EAGAIN) {
          tls_io_instance->tlsio_state = TLSIO_STATE_ERROR;
        }
        if (dowork_send(tls_io_instance) < 0 && errno != EAGAIN) {
          tls_io_instance->tlsio_state = TLSIO_STATE_ERROR;
        }
        break;
    case TLSIO_STATE_ERROR:
        break;
    default:
        LogError("Unexpected internal tlsio state");
        break;
    }
  }
}

static int tlsio_esp_tls_send_async(CONCRETE_IO_HANDLE tls_io, const void* buffer, size_t size, ON_SEND_COMPLETE on_send_complete, void* callback_context) {
  int result;
  if (on_send_complete == NULL) {
    result = __FAILURE__;
    LogError("NULL on_send_complete");
  } else {
    if (tls_io == NULL) {
      result = __FAILURE__;
      LogError("NULL tlsio");
    } else {
      if (buffer == NULL) {
        result = __FAILURE__;
        LogError("NULL buffer");
      } else {
        if (size == 0) {
          result = __FAILURE__;
          LogError("0 size");
        } else {
          TLS_IO_INSTANCE* tls_io_instance = (TLS_IO_INSTANCE*)tls_io;
          if (tls_io_instance->tlsio_state != TLSIO_STATE_OPEN) {
            result = __FAILURE__;
            LogError("tlsio_esp_tls_send_async without a prior successful open");
          } else {
            PENDING_TRANSMISSION* pending_transmission = (PENDING_TRANSMISSION*)malloc(sizeof(PENDING_TRANSMISSION));
            if (pending_transmission == NULL) {
              result = __FAILURE__;
              LogError("malloc failed");
            } else {
              pending_transmission->bytes = (unsigned char*)malloc(size);
              if (pending_transmission->bytes == NULL) {
                LogError("malloc failed");
                free(pending_transmission);
                result = __FAILURE__;
              } else {
                pending_transmission->size = size;
                pending_transmission->unsent_size = size;
                pending_transmission->on_send_complete = on_send_complete;
                pending_transmission->callback_context = callback_context;
                (void)memcpy(pending_transmission->bytes, buffer, size);
                if (singlylinkedlist_add(tls_io_instance->pending_transmission_list, pending_transmission) == NULL) {
                  LogError("Unable to add socket to pending list.");
                  free(pending_transmission->bytes);
                  free(pending_transmission);
                  result = __FAILURE__;
                } else {
                  dowork_send(tls_io_instance);
                  result = 0;
                }
              }
            }
          }
        }
      }
    }
  }
  return result;
}

static int tlsio_esp_tls_setoption(CONCRETE_IO_HANDLE tls_io, const char* optionName, const void* value) {
  TLS_IO_INSTANCE* tls_io_instance = (TLS_IO_INSTANCE*)tls_io;
  int result;
  if (tls_io_instance == NULL) {
    LogError("NULL tlsio");
    result = __FAILURE__;
  } else {
    TLSIO_OPTIONS_RESULT options_result = tlsio_options_set(&tls_io_instance->options, optionName, value);
    if (options_result != TLSIO_OPTIONS_RESULT_SUCCESS) {
      LogError("Failed tlsio_options_set");
      result = __FAILURE__;
    } else {
      result = 0;
    }
  }
  return result;
}

static OPTIONHANDLER_HANDLE tlsio_esp_tls_retrieveoptions(CONCRETE_IO_HANDLE tls_io) {
  TLS_IO_INSTANCE* tls_io_instance = (TLS_IO_INSTANCE*)tls_io;
  OPTIONHANDLER_HANDLE result;
  if (tls_io_instance == NULL) {
    LogError("NULL tlsio");
    result = NULL;
  } else {
    result = tlsio_options_retrieve_options(&tls_io_instance->options, tlsio_esp_tls_setoption);
  }
  return result;
}

static const IO_INTERFACE_DESCRIPTION tlsio_esp_tls_interface_description = {
  tlsio_esp_tls_retrieveoptions,
  tlsio_esp_tls_create,
  tlsio_esp_tls_destroy,
  tlsio_esp_tls_open_async,
  tlsio_esp_tls_close_async,
  tlsio_esp_tls_send_async,
  tlsio_esp_tls_dowork,
  tlsio_esp_tls_setoption
};

const IO_INTERFACE_DESCRIPTION* tlsio_pal_get_interface_description(void) {
  return &tlsio_esp_tls_interface_description;
}