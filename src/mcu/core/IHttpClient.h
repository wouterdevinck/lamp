#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include <map>

#define HTTP_GET "GET"
#define HTTP_RANGE "Range"
#define HTTP_CONTENT_RANGE "Content-Range"

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