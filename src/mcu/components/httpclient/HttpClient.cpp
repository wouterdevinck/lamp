#include "HttpClient.h"
#include "esp_log.h"
#include "esp_http_client.h"
#include "string.h"
#include "fnptr.h"

static char tag[] = "HttpClient";

using namespace lamp;
using namespace std;

HttpResponse HttpClient::request(HttpRequest req) {
  auto resp = HttpResponse();
  esp_http_client_config_t config;
  ::memset(&config, 0, sizeof(esp_http_client_config_t));
  config.url = req.url.c_str();
  config.buffer_size = _chunkSize + 1024;
  // TODO Provide certificate(s)
  // Http client does not verify server if no cert provided
  // config.cert_pem = github_com_root_cert_pem_start; 
  config.event_handler = fnptr<esp_err_t(esp_http_client_event_t*)>(
    [&resp] (esp_http_client_event_t *evt) -> esp_err_t {
      switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
          ESP_LOGD(tag, "HTTP_EVENT_ERROR");
          break;
        case HTTP_EVENT_ON_CONNECTED:
          ESP_LOGD(tag, "HTTP_EVENT_ON_CONNECTED");
          break;
        case HTTP_EVENT_HEADER_SENT:
          ESP_LOGD(tag, "HTTP_EVENT_HEADER_SENT");
          break;
        case HTTP_EVENT_ON_HEADER:
          ESP_LOGD(tag, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
          resp.headers[evt->header_key] = evt->header_value;
          break;
        case HTTP_EVENT_ON_DATA:
          ESP_LOGD(tag, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
          if (::esp_http_client_get_status_code(evt->client) < 300) {
            auto data = (uint8_t*)(evt->data);
            resp.body.insert(resp.body.end(), data, data + evt->data_len);
          }
          break;
        case HTTP_EVENT_ON_FINISH:
          ESP_LOGD(tag, "HTTP_EVENT_ON_FINISH");
          break;
        case HTTP_EVENT_DISCONNECTED:
          ESP_LOGD(tag, "HTTP_EVENT_DISCONNECTED");
          break;
      }
      return ESP_OK;
    }
  );
  esp_http_client_handle_t client = ::esp_http_client_init(&config);
  if (req.method == HTTPCLIENT_GET) {
    // nop
  } else if (req.method == HTTPCLIENT_POST) {
    ::esp_http_client_set_method(client, HTTP_METHOD_POST);
  } else {
    ESP_LOGE(tag, "Http method %s not implemented", req.method.c_str());
    return HttpResponse();    
  }
  for(auto const& x: req.headers) {
    ::esp_http_client_set_header(client, x.first.c_str(), x.second.c_str());
  }
  esp_err_t err = ::esp_http_client_perform(client);
  if (err == ESP_OK) {
    auto status = ::esp_http_client_get_status_code(client);
    ESP_LOGI(tag, "Status = %d, Length = %d",
      status, ::esp_http_client_get_content_length(client));
    resp.status = status;
  } else {
    ESP_LOGE(tag, "Error perform http request %s", esp_err_to_name(err));
  }
  ::esp_http_client_cleanup(client);
  return resp;
}