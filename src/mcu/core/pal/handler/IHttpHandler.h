#pragma once

#include <string>

using namespace std;

namespace lamp {

  struct IHttpHandler {

    virtual ~IHttpHandler() noexcept = default;

    virtual string handleHttpRequest(string method, string path, string query, string body) = 0;

  };

}
