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
  ::esp_smartconfig_start(&smartConfigCallback);
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

void WiFiClient::smartConfigCallback(smartconfig_status_t status, void *pdata) {
  switch (status) {
    case SC_STATUS_WAIT:
      ESP_LOGI(tag, "Smart Config: WAIT");
      break;
    case SC_STATUS_FIND_CHANNEL:
      ESP_LOGI(tag, "Smart Config: FIND_CHANNEL");
      break;
    case SC_STATUS_GETTING_SSID_PSWD:
      ESP_LOGI(tag, "Smart Config: GETTING_SSID_PSWD");
      break;
    case SC_STATUS_LINK: 
      {
        ESP_LOGI(tag, "Smart Config: LINK");
        auto wifi_config = (wifi_config_t*)pdata;
        auto ssid = (char*)wifi_config->sta.ssid;
        auto pwd = (char*)wifi_config->sta.password;
        ESP_LOGI(tag, "SSID: %s", ssid);
        ESP_LOGI(tag, "Password: %s", pwd);
        _handler->onSmartConfig(string(ssid), string(pwd));
      }
      break;
    case SC_STATUS_LINK_OVER:
      ESP_LOGI(tag, "Smart Config: LINK_OVER");
      if (pdata != NULL) {
        uint8_t phone_ip[4] = { 0 };
        memcpy(phone_ip, (uint8_t* )pdata, 4);
        ESP_LOGI(tag, "Phone IP: %d.%d.%d.%d\n", phone_ip[0], phone_ip[1], phone_ip[2], phone_ip[3]);
      }
      ::esp_smartconfig_stop();
      break;
    default:
      break;
  }
}