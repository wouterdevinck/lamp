#include "Lamp.h"
#include "HttpHandler.h"

using namespace lamp;

void Lamp::start(const int port) const {
  const RgbLedColor color = { 0, 0, 255 };
  _led->setLedColor(color);
  _http->start(port, _handler);
}
