#include "HttpHandler.h"

using namespace lamp;
	
string HttpHandler::handleHttpRequest(string method, string path) {
  if (method == "GET") {
    if (path == "/r") {
      const RgbLedColor color = { 255, 0, 0 };
      _led->setLedColor(color);
    } else if (path == "/g") {
      const RgbLedColor color = { 0, 255, 0 };
      _led->setLedColor(color);
    } else if (path == "/b") {
      const RgbLedColor color = { 0, 0, 255 };
      _led->setLedColor(color);
    }
  }
  return "<!DOCTYPE html>"
    "<html>\n"
    "<head>\n"
    "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
    "  <title>Lamp Control</title>\n"
    "</head>\n"
    "<body>\n"
    "  <h1>Lamp Control</h1>\n"
    "  <a href=\"/r\">RED</a>\n"
    "  <a href=\"/g\">GREEN</a>\n"
    "  <a href=\"/b\">BLUE</a>\n"
    "</body>\n"
    "</html>\n";
}