#include "Lamp.h"
#include "Platform.h"
#include "Factory.h"

using namespace lamp;

extern "C" {
  void app_main(void);
}

void app_main(void) {
  auto factory = new Factory();
  if (factory->isProvisioned()) {
    auto platform = new Platform();
    auto lamp = new Lamp(platform);
    lamp->start();
  } else {
    factory->provision();
  }
}