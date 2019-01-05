#pragma once

#include <cstdint>

namespace lamp {

  class MemMon {

    public:
      explicit MemMon(uint16_t periodMs);

    private:
      static void memMonTask(void* parameters);

  };

}