#include "WiFiClient.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "string.h"

static char tag[] = "WiFiClient";

using namespace lamp;
using namespace std;

IWiFiHandler* WiFiClient::_handler = NULL;

WiFiClient::WiFiClient(Storage* _) {
  ::tcpip_adapter_init();
  ESP_ERROR_CHECK(::esp_event_loop_init(&eventHandler, NULL));
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(::esp_wifi_init(&cfg));
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
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &sta_config));
  ESP_ERROR_CHECK(esp_wifi_start());
}

void WiFiClient::reconnect() {
  ::esp_wifi_connect();
}

void WiFiClient::startSmartConfig() {
  // TODO
}

esp_err_t WiFiClient::eventHandler(void *ctx, system_event_t *event) {
  switch(event->event_id) {
    case SYSTEM_EVENT_STA_START:
      ::esp_wifi_connect();
      break;
    case SYSTEM_EVENT_STA_GOT_IP:
      ESP_LOGI(tag, "Got ip: %s", ::ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
      _handler->onConnected();
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      ESP_LOGI(tag, "WiFi disconnected\n");
      _handler->onDisconnected();
      break;
    case SYSTEM_EVENT_STA_CONNECTED:
      ESP_LOGI(tag, "WiFi connected\n");
      break;
    default:
      break;
  }
  return ESP_OK;
}