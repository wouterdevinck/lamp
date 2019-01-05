#pragma once

#include "IHttpClient.h"

namespace lamp {

  class HttpClient : public IHttpClient {

    public:
      explicit HttpClient(uint16_t chunkSize) : _chunkSize(chunkSize) {}

      HttpResponse request(HttpRequest req) override;

    private:
      uint16_t _chunkSize;

  };

}