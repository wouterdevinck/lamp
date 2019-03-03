#pragma once

#include "INvs.h"
#include "nvs_flash.h"

namespace lamp {

  class Storage : public INvs {

    public:
      explicit Storage();

      string getValue(string key) override;
      void setValue(string key, string value) override;

    private:
      nvs_handle _handle;

  };

}