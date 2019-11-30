#pragma once

#include <cstdint>
#include <string>
#include <vector>

#define BUF_SIZE (1024)
#define UART UART_NUM_0

using namespace std;

namespace lamp {

  class Factory {

    public:
      explicit Factory();

      bool isProvisioned();
      void provision();

    private:
      void process(string line);
      string bin2hex(uint8_t* data, uint8_t len);
      vector<char> hex2bin(string hex);
      static void restartHandler(void* parameters);

  };

}