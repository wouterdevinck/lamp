#include <string.h>
#include "hsm_client_data.h"
#include "esp_log.h"
#include "nvscrt.h"

static const char* TAG = "HSM";

typedef struct CUSTOM_HSM_INFO_TAG {
  const char* certificate;
  const char* common_name;
  const char* key;
} CUSTOM_HSM_INFO;

int hsm_client_x509_init() {
  return 0;
}

void hsm_client_x509_deinit() {}

HSM_CLIENT_HANDLE custom_hsm_create() {
  CUSTOM_HSM_INFO* info = malloc(sizeof(CUSTOM_HSM_INFO));
  if (info == NULL) {
    ESP_LOGE(TAG, "Failed allocating info");
    return NULL;
  }

  // Private key - refer to the ATECC608A secure element
  info->key = "atca";

  // Certificate - get from NVS in flash
  char* pem = readNvsCertPem();
  if (pem == NULL) {
    ESP_LOGE(TAG, "Failed getting PEM certificate from NVS");
    return NULL;
  }
  info->certificate = pem;

  // CN - read from certificate
  char* cn = readNvsCn();
  if (cn == NULL) {
    ESP_LOGE(TAG, "Failed getting CN");
    return NULL;
  }
  info->common_name = cn;

  return info;
}

void custom_hsm_destroy(HSM_CLIENT_HANDLE handle) {
  if (handle != NULL) {
    CUSTOM_HSM_INFO* info = (CUSTOM_HSM_INFO*)handle;
    free(info);
  }
}

char* custom_hsm_get_certificate(HSM_CLIENT_HANDLE handle) {
  char* result;
  if (handle == NULL)     {
    ESP_LOGE(TAG, "Invalid handle value specified");
    result = NULL;
  } else {
    CUSTOM_HSM_INFO* info = (CUSTOM_HSM_INFO*)handle;
    size_t len = strlen(info->certificate);
    if ((result = (char*)malloc(len + 1)) == NULL) {
      ESP_LOGE(TAG, "Failure allocating certificate");
      result = NULL;
    } else {
      strcpy(result, info->certificate);
    }
  }
  return result;
}

char* custom_hsm_get_key(HSM_CLIENT_HANDLE handle) {
  char* result;
  if (handle == NULL) {
    ESP_LOGE(TAG, "Invalid handle value specified");
    result = NULL;
  } else {
    CUSTOM_HSM_INFO* info = (CUSTOM_HSM_INFO*)handle;
    size_t len = strlen(info->key);
    if ((result = (char*)malloc(len + 1)) == NULL) {
      ESP_LOGE(TAG, "Failure allocating certificate");
      result = NULL;
    } else {
      strcpy(result, info->key);
    }
  }
  return result;
}

char* custom_hsm_get_common_name(HSM_CLIENT_HANDLE handle) {
  char* result;
  if (handle == NULL)   {
    ESP_LOGE(TAG, "Invalid handle value specified");
    result = NULL;
  } else {
    CUSTOM_HSM_INFO* info = (CUSTOM_HSM_INFO*)handle;
    size_t len = strlen(info->common_name);
    if ((result = (char*)malloc(len + 1)) == NULL) {
      ESP_LOGE(TAG, "Failure allocating certificate");
      result = NULL;
    } else {
      strcpy(result, info->common_name);
    }
  }
  return result;
}

static const HSM_CLIENT_X509_INTERFACE x509_interface = {
  custom_hsm_create,
  custom_hsm_destroy,
  custom_hsm_get_certificate,
  custom_hsm_get_key,
  custom_hsm_get_common_name
};

const HSM_CLIENT_X509_INTERFACE* hsm_client_x509_interface() {
  return &x509_interface;
}