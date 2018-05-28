#pragma once

#include "IUpdater.h"
#include <cstdint>

// Load fpga bitstream from .rodata section
extern const uint8_t fpga_image_start[] asm("_binary_lamp_bin_start");
extern const uint8_t fpga_image_end[]   asm("_binary_lamp_bin_end");

namespace lamp {

  class Updater : public IUpdater {

    public:
      explicit Updater();

      string getRunningVersion() override;
      string getRunningFpgaHash() override;
      string getInstalledFpgaHash() override;

      bool beginUpgrade() override;
      bool writeChunk(vector<uint8_t> chunk) override;
      bool completeUpgrade() override;

      bool flashFpga() override;

  };

}