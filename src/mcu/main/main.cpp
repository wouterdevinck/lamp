#include "Lamp.h"
#include "Platform.h"

using namespace lamp;

extern "C" {
  void app_main(void);
}

void app_main(void) {
  auto platform = new Platform();
  auto lamp = new Lamp(platform); 
  lamp->start();
}