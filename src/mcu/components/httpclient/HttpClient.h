#pragma once

#include "IHttpClient.h"
#include "esp_http_client.h"

#define BUFFER_SIZE 51 * 1024

namespace lamp {

  class HttpClient : public IHttpClient {

    public:
      explicit HttpClient() {}

      HttpResponse request(HttpRequest req) override;

    private:
      static esp_err_t httpEventHandler(esp_http_client_event_t *evt);

  };

}