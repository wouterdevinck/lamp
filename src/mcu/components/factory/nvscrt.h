// Helper library to work with certificates stored in NVS
#pragma once

#include "nvs.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "mbedtls/x509_crt.h"
#include "mbedtls/oid.h"
#include "mbedtls/pem.h"
#include "constants.h"

#define NVS_NAMESPACE NVS_NS_IOT
#define NVS_KEY IOT_NVS_KEY

static const char* NVS_CRT_TAG = "nvscrt.h";

typedef struct NvsCertDers {
  char* data;
  size_t size;
} NvsCertDer;

int writeNvsCertDer(char* data, size_t size) {
  esp_err_t err;
  nvs_handle handle;
  err = nvs_flash_init();
  if (err != ESP_OK) {
    return 1;
  }
  err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
  if (err != ESP_OK) {
    return 2;
  }
  err = nvs_set_blob(handle, NVS_KEY, data, size);
  if (err != ESP_OK) {
    return 3;
  }
  err = nvs_commit(handle);
  if (err != ESP_OK) {
    return 4;
  }
  nvs_close(handle);
  return 0;
}

int readNvsCertDer(NvsCertDer* der) {
  esp_err_t err;
  nvs_handle handle;
  der->size = 0;
  err = nvs_flash_init();
  if (err != ESP_OK) {
    ESP_LOGE(NVS_CRT_TAG, "Failed to init NVS");
    return 1;
  }
  err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
  if (err != ESP_OK) {
    ESP_LOGE(NVS_CRT_TAG, "Failed to open NVS");
    return 2;
  }
  err = nvs_get_blob(handle, NVS_KEY, NULL, &der->size);
  if (err != ESP_OK) {
    ESP_LOGE(NVS_CRT_TAG, "Failed to get NVS blob size");
    return 3;
  }
  der->data = (char*)malloc(der->size + sizeof(char));
  if (der->data == NULL) {
    ESP_LOGE(NVS_CRT_TAG, "Failed allocating certData");
    return 4;
  } 
  err = nvs_get_blob(handle, NVS_KEY, der->data, &der->size);
  if (err != ESP_OK) {
    ESP_LOGE(NVS_CRT_TAG, "Failed to read NVS blob");
    return 5;
  }
  nvs_close(handle);
  // ESP_LOG_BUFFER_HEX(NVS_CRT_TAG, der->data, der->size);
  return 0;
}

char* readNvsCertPem() {
  int ret;
  unsigned char buf[1024];
  size_t olen = 0;
  NvsCertDer* der = (NvsCertDer*)malloc(sizeof(NvsCertDer));
  ret = readNvsCertDer(der);
  if (ret != 0) {
    return NULL;
  }
  ret = mbedtls_pem_write_buffer(
    "-----BEGIN CERTIFICATE-----\n", 
    "-----END CERTIFICATE-----\n",
    (const unsigned char*)der->data, der->size, 
    buf, 1024, &olen);
  free(der);
  if (ret != 0) {
    ESP_LOGE(NVS_CRT_TAG, "Failed to write PEM certificate");
    return NULL;
  }
  buf[olen] = 0;
  char* res = (char*)malloc(sizeof(char) * (olen + 1));
  strcpy(res, (char*)buf);
  return res;
}

char* readNvsCn() {
  int ret;
  char* cn = NULL;
  bool gotcn = false;
  mbedtls_x509_crt crt;
  NvsCertDer* der = (NvsCertDer*)malloc(sizeof(NvsCertDer));
  ret = readNvsCertDer(der);
  if (ret != 0) {
    return NULL;
  }
  mbedtls_x509_crt_init(&crt);
  ret = mbedtls_x509_crt_parse_der(&crt, (const unsigned char*)der->data, der->size);
  if (ret != 0) {
    ESP_LOGE(NVS_CRT_TAG, "Failed to parse DER certificate data");
    return NULL;
  }
  for (const mbedtls_x509_name *n = &crt.subject; n != NULL; n = n->next) {
    if ((n->oid.len == MBEDTLS_OID_SIZE(MBEDTLS_OID_AT_CN)) &&
        (memcmp(n->oid.p, MBEDTLS_OID_AT_CN, MBEDTLS_OID_SIZE(MBEDTLS_OID_AT_CN)) == 0)) {
      cn = (char*)n->val.p;
      cn[n->val.len] = 0;
      gotcn = true;
    }
  }
  free(der);
  if (!gotcn) {
    ESP_LOGE(NVS_CRT_TAG, "Failed to get CN from certificate");
    return NULL;
  }
  return cn;
}