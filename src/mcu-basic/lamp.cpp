#include "new.h"

#include "Platform.h"
#include "Lamp.h"

#define BOARDS 2

using namespace lamp;

extern "C" {
  int main(void);
}

int main(void) {
  auto platform = new Platform(BOARDS);
  auto lamp = new Lamp(platform);
  lamp->start();
  while(1){
    platform->loop();
  }
}