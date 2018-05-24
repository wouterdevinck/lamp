#include "Updater.h"
#include "version.h"
#include "esp_log.h"
#include <iomanip>
#include <sstream>

static char tag[] = "Updater";

using namespace lamp;
using namespace std;

Updater::Updater() {

  // TEMP TEST
  ESP_LOGD(tag, "FPGA version: %s", FPGA_HASH);
  ESP_LOGD(tag, "Lamp version: %s", LAMP_VERSION);
  ostringstream dbg;
  dbg << endl;
  for (int i = 0; i < fpga_image_end - fpga_image_start; ++i) {
    dbg << hex << setw(2) << setfill('0') << 
      static_cast<unsigned int>(fpga_image_start[i]) << ' ';
    if ((i + 1) % 8 == 0) {
      dbg << endl;
    }
  }
  ESP_LOGD(tag, "%s", dbg.str().c_str())

}