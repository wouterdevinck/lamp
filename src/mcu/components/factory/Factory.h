#pragma once

#include <cstdint>

namespace lamp {

  class Factory {

    public:
      explicit Factory() {}

      bool isProvisioned();
      void provision();

    private:
      uint8_t init();

  };

}