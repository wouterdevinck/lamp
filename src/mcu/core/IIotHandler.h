#pragma once

#include <string>

using namespace std;

struct IIotHandler {

  virtual ~IIotHandler() noexcept = default;

  virtual string handleDirectMethod(string method, string payload) = 0; // also return int?
  // handle cloud to device message
  // handle twin desired property

};
