#include "WiFiClient.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_smartconfig.h"
#include "string.h"
#include "stdlib.h"

static char tag[] = "WiFiClient";

using namespace lamp;
using namespace std;

IWiFiHandler* WiFiClient::_handler = NULL;

WiFiClient::WiFiClient(Storage* _) {
  ::tcpip_adapter_init();
  ESP_ERROR_CHECK(::esp_event_loop_create_default());
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(::esp_wifi_init(&cfg));
  ESP_ERROR_CHECK(::esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &eventHandler, NULL));
  ESP_ERROR_CHECK(::esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &eventHandler, NULL));
  ESP_ERROR_CHECK(::esp_event_handler_register(SC_EVENT, ESP_EVENT_ANY_ID, &eventHandler, NULL));
  ESP_ERROR_CHECK(::esp_wifi_set_mode(WIFI_MODE_STA));
}

void WiFiClient::setHandler(IWiFiHandler* handler) {
  _handler = handler;
}

void WiFiClient::connect(string ssid, string pwd) {
  wifi_config_t sta_config;
  ::memset(&sta_config, 0, sizeof(sta_config));
  ::memcpy(sta_config.sta.ssid, ssid.data(), ssid.size());
  ::memcpy(sta_config.sta.password, pwd.data(), pwd.size());
  sta_config.sta.bssid_set = 0;
  ::esp_wifi_set_config(ESP_IF_WIFI_STA, &sta_config);
  ::esp_wifi_start();
}

void WiFiClient::reconnect() {
  ::esp_wifi_connect();
}

void WiFiClient::startSmartConfig() {
  ::esp_wifi_start();
  ::esp_smartconfig_set_type(SC_TYPE_ESPTOUCH);
  smartconfig_start_config_t cfg = SMARTCONFIG_START_CONFIG_DEFAULT();
  ::esp_smartconfig_start(&cfg);
}

void WiFiClient::eventHandler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
  if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
    ::esp_wifi_connect();
  } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
    ESP_LOGI(tag, "WiFi disconnected\n");
    _handler->onDisconnected();
  } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_CONNECTED) {
    ESP_LOGI(tag, "WiFi connected\n");
  } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
    ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
    ESP_LOGI(tag, "Got ip: " IPSTR, IP2STR(&event->ip_info.ip));
    _handler->onConnected();
  } else if (event_base == SC_EVENT && event_id == SC_EVENT_SCAN_DONE) {
    ESP_LOGI(tag, "Smart Config: SC_EVENT_SCAN_DONE");
  } else if (event_base == SC_EVENT && event_id == SC_EVENT_FOUND_CHANNEL) {
    ESP_LOGI(tag, "Smart Config: SC_EVENT_FOUND_CHANNEL");
  } else if (event_base == SC_EVENT && event_id == SC_EVENT_GOT_SSID_PSWD) {
    ESP_LOGI(tag, "Smart Config: SC_EVENT_GOT_SSID_PSWD");
    smartconfig_event_got_ssid_pswd_t *evt = (smartconfig_event_got_ssid_pswd_t *)event_data;
    uint8_t ssid[33] = { 0 };
    uint8_t password[65] = { 0 };
    ::memcpy(ssid, evt->ssid, sizeof(evt->ssid));
    ::memcpy(password, evt->password, sizeof(evt->password));
    ESP_LOGI(tag, "SSID: %s", ssid);
    ESP_LOGI(tag, "Password: %s", password);
    _handler->onSmartConfig(string(ssid, ssid + sizeof(ssid)), string(password, password + sizeof(password)));
  } else if (event_base == SC_EVENT && event_id == SC_EVENT_SEND_ACK_DONE) {
    ESP_LOGI(tag, "Smart Config: SC_EVENT_SEND_ACK_DONE");
  }
}