#include "Updater.h"
#include "version.h"
#include "esp_log.h"
// #include <iomanip>
// #include <sstream>

static char tag[] = "Updater";

using namespace lamp;
using namespace std;

Updater::Updater() {

  // TEMP TEST
  ESP_LOGD(tag, "FPGA version: %s", FPGA_HASH);
  ESP_LOGD(tag, "Lamp version: %s", LAMP_VERSION);
  /* ostringstream dbg;
  dbg << endl;
  for (int i = 0; i < fpga_image_end - fpga_image_start; ++i) {
    dbg << hex << setw(2) << setfill('0') << 
      static_cast<unsigned int>(fpga_image_start[i]) << ' ';
    if ((i + 1) % 8 == 0) {
      dbg << endl;
    }
  }
  ESP_LOGD(tag, "%s", dbg.str().c_str())*/

}

string Updater::getRunningVersion() {
  return LAMP_VERSION;
}

string Updater::getRunningFpgaHash() {
  // TODO
  return "";
}

string Updater::getInstalledFpgaHash() {
  return FPGA_HASH;
}

bool Updater::beginUpgrade() {
  // TODO
  ESP_LOGD(tag, "beginUpgrade");
  return true;
}

bool Updater::writeChunk(vector<uint8_t> chunk) {
  // TODO
  return true;
}

bool Updater::completeUpgrade() {
  // TODO
  return true;
}

bool Updater::flashFpga() {
  // TODO
  return true;
}
