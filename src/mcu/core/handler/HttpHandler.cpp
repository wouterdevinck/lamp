#ifndef BASIC

#include "HttpHandler.h"

using namespace lamp;
	
string HttpHandler::handleHttpRequest(string method, string path, string query, string body) {
  if (method == "GET") {
    if (path == "/r") {
      const RgbLedColor color = { 255, 0, 0 };
      _led->setLedColor(color);
      _leds->changeColor(LampColor::Red);
    } else if (path == "/g") {
      const RgbLedColor color = { 0, 255, 0 };
      _led->setLedColor(color);
      _leds->changeColor(LampColor::Green);
    } else if (path == "/b") {
      const RgbLedColor color = { 0, 0, 255 };
      _led->setLedColor(color);
      _leds->changeColor(LampColor::Blue);
    }
  }
  if (method == "POST") {
    if (path == "/upgrade") {
      auto url = urlDecode(body.substr(4));
      if (_upgrade->upgrade(url)) {
        return "Upgrade completed";
      } else {
        return "Upgrade failed";
      }
    } else {
      return "Not Found";
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
    "  <h1>Upgrade</h1>\n"
    "  <form action=\"upgrade\" method=\"POST\">\n"
    "    <input type=\"text\" name=\"url\" value=\"https://github.com/wouterdevinck/lamp/releases/download/0.8.1/lamp.bin\" size=\"60\"><br />\n"
    "    <input type=\"submit\" value=\"Upgrade\">\n"
    "  </form>\n"
    "  <footer>\n"
    "    Version: " + _upgrade->getVersion() + "\n"
    "  </footer>\n"
    "</body>\n"
    "</html>\n";
}

string HttpHandler::urlDecode(string str){
  string ret;
  char ch;
  int i, ii, len = str.length();
  for (i=0; i < len; i++) {
    if(str[i] != '%') {
      if(str[i] == '+') {
        ret += ' ';
      } else {
        ret += str[i];
      }
    } else {
      sscanf(str.substr(i + 1, 2).c_str(), "%x", &ii);
      ch = static_cast<char>(ii);
      ret += ch;
      i = i + 2;
    }
  }
  return ret;
}

#endif