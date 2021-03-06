#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include <map>

#define HTTPCLIENT_GET "GET"
#define HTTPCLIENT_POST "POST"
#define HTTPCLIENT_RANGE "Range"
#define HTTPCLIENT_CONTENT_RANGE "Content-Range"

using namespace std;

namespace lamp {

  struct HttpRequest {
    string method;
    string url;
    map<string, string> headers;
  };

  struct HttpResponse {
    uint8_t status;
    map<string, string> headers;
    vector<uint8_t> body;
  };

  struct IHttpClient {

    virtual ~IHttpClient() noexcept = default;

    virtual HttpResponse request(HttpRequest req) = 0;

  };

}