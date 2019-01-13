#pragma once

#include <string>

#define STORE_SSID "lamp.ssid"
#define STORE_PWD "lamp.pwd"

using namespace std;

namespace lamp {

  struct IStorage {

    virtual ~IStorage() noexcept = default;

    virtual string getValue(string key) = 0;
    virtual void setValue(string key, string value) = 0;

  };

}