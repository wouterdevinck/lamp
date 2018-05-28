#pragma once

#include "IHttpClient.h"

namespace lamp {

  class HttpClient : public IHttpClient {

    public:
      explicit HttpClient() {}

      HttpResponse request(HttpRequest req) override;

  };

}