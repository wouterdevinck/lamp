#pragma once

#include <cstdint>
#include <string>

#define BUF_SIZE (1024)
#define UART UART_NUM_0
#define NVS_NAMESPACE "factory"
#define NVS_KEY "certificate"

using namespace std;

namespace lamp {

  class Factory {

    public:
      explicit Factory();

      bool isProvisioned();
      void provision();

    private:
      void process(string line);
      static void restartHandler(void* parameters);

  };

}