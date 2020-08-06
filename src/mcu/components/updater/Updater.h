#pragma once

#include "IUpdater.h"
#include "SpiFlash.h"
#include "Storage.h"
#include "esp_ota_ops.h"
#include <cstdint>

// Load fpga bitstream from .rodata section
extern const uint8_t fpga_image_start[] asm("_binary_lamp_bin_start");
extern const uint8_t fpga_image_end[]   asm("_binary_lamp_bin_end");

namespace lamp {

  class Updater : public IUpdater {

    public:
      explicit Updater(SpiFlash* flash, Storage* storage, uint16_t chunkSize);

      string getRunningVersion() override;
      string getRunningFpgaHash() override;
      string getInstalledFpgaHash() override;

      bool beginUpgrade() override;
      bool writeChunk(vector<uint8_t> chunk) override;
      bool completeUpgrade() override;

      bool flashFpga() override;

      uint16_t getPreferredChunkSize() override;

    private:
      SpiFlash* _flash;
      Storage* _storage;
      uint16_t _chunkSize;
      esp_ota_handle_t _otaHandle;
      const esp_partition_t* _otaTarget;
      int roundUp(int numToRound, int multiple);

  };

}