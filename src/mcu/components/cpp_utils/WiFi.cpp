// Inspired by: https://github.com/nkolban/esp32-snippets/tree/master/cpp_utils/WiFi.cpp

#include "WiFi.h"
#include "GeneralUtils.h"

#include "esp_log.h"
#include "esp_wifi.h"
#include "nvs_flash.h"

#include "string.h"

static const char* LOG_TAG = "WiFi";

WiFi::WiFi() {
  m_eventLoopStarted = false;
  m_initCalled = false;
  m_pWifiEventHandler = new WiFiEventHandler();
} 

void WiFi::connectAP(const std::string& ssid, const std::string& password) {
  ESP_LOGD(LOG_TAG, ">> connectAP");
  init();
  esp_err_t errRc = ::esp_wifi_set_mode(WIFI_MODE_STA);
  if (errRc != ESP_OK) {
    ESP_LOGE(LOG_TAG, "esp_wifi_set_mode: rc=%d %s", errRc, GeneralUtils::errorToString(errRc));
    abort();
  }
  wifi_config_t sta_config;
  ::memset(&sta_config, 0, sizeof(sta_config));
  ::memcpy(sta_config.sta.ssid, ssid.data(), ssid.size());
  ::memcpy(sta_config.sta.password, password.data(), password.size());
  sta_config.sta.bssid_set = 0;
  errRc = ::esp_wifi_set_config(WIFI_IF_STA, &sta_config);
  if (errRc != ESP_OK) {
  ESP_LOGE(LOG_TAG, "esp_wifi_set_config: rc=%d %s", errRc, GeneralUtils::errorToString(errRc));
    abort();
  }
  errRc = ::esp_wifi_start();
  if (errRc != ESP_OK) {
    ESP_LOGE(LOG_TAG, "esp_wifi_start: rc=%d %s", errRc, GeneralUtils::errorToString(errRc));
    abort();
  }

  m_connectFinished.take("connectAP");   

  errRc = ::esp_wifi_connect();
  if (errRc != ESP_OK) {
    ESP_LOGE(LOG_TAG, "esp_wifi_connect: rc=%d %s", errRc, GeneralUtils::errorToString(errRc));
    abort();
  }

  m_connectFinished.wait("connectAP"); 

  ESP_LOGD(LOG_TAG, "<< connectAP");
}

esp_err_t WiFi::eventHandler(void* ctx, system_event_t* event) {
  WiFi *pWiFi = (WiFi *)ctx;
  esp_err_t rc = pWiFi->m_pWifiEventHandler->getEventHandler()(pWiFi->m_pWifiEventHandler, event);

  if (event->event_id == SYSTEM_EVENT_STA_GOT_IP || event->event_id == SYSTEM_EVENT_STA_DISCONNECTED) {
    pWiFi->m_connectFinished.give();
  }

  return rc;
}

void WiFi::init() {
  if (m_eventLoopStarted) {
    ::esp_event_loop_set_cb(WiFi::eventHandler, this);
  } else {
    esp_err_t errRc = ::esp_event_loop_init(WiFi::eventHandler, this);
    if (errRc != ESP_OK) {
      ESP_LOGE(LOG_TAG, "esp_event_loop_init: rc=%d %s", errRc, GeneralUtils::errorToString(errRc));
      abort();
    }
    m_eventLoopStarted = true;
  }
  if (!m_initCalled) {
    ::nvs_flash_init();
    ::tcpip_adapter_init();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_err_t errRc = ::esp_wifi_init(&cfg);
    if (errRc != ESP_OK) {
      ESP_LOGE(LOG_TAG, "esp_wifi_init: rc=%d %s", errRc, GeneralUtils::errorToString(errRc));
      abort();
    }
    errRc = ::esp_wifi_set_storage(WIFI_STORAGE_RAM);
    if (errRc != ESP_OK) {
      ESP_LOGE(LOG_TAG, "esp_wifi_set_storage: rc=%d %s", errRc, GeneralUtils::errorToString(errRc));
      abort();
    }
  }
  m_initCalled = true;
}

void WiFi::setWifiEventHandler(WiFiEventHandler* wifiEventHandler) {
  this->m_pWifiEventHandler = wifiEventHandler;
}