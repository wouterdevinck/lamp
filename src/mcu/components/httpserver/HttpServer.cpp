#include <iostream>
#include <sstream>
#include "HttpServer.h"
#include "esp_log.h"

static char tag[] = "HttpServer";

using namespace lamp;
using namespace std;

bool HttpServer::_started = false;
int HttpServer::_port = 0;
IHttpHandler* HttpServer::_handler = NULL;

void HttpServer::start(int port, IHttpHandler* handler) {
  if(_started) stop();
  _started = true;
  _port = port;
  _handler = handler;
  ::xTaskCreatePinnedToCore(&mongooseTask, "mongooseTask", 20000, NULL, 5, &_handle, 0);
}

void HttpServer::stop() {
  if(_started && _handle != NULL) {
    ::vTaskDelete(_handle);
    _started = false;
  }
}

void HttpServer::mongooseTask(void* parameters) {
  ESP_LOGD(tag, "Mongoose: Starting task");
  struct mg_mgr mgr;
  ::mg_mgr_init(&mgr, NULL);
  ESP_LOGD(tag, "Mongoose: Succesfully inited");
  ostringstream addr;  
  addr << ":" << _port;
  auto c = ::mg_bind(&mgr, addr.str().c_str(), HttpServer::mongooseEventHandler); 
  if (c == NULL) {
    ESP_LOGE(tag, "Mongoose: Failed to bind");
    ::vTaskDelete(NULL); 
    return;
  }
  ESP_LOGD(tag, "Mongoose: Successfully bound");
  ::mg_set_protocol_http_websocket(c);
  while (1) {
    ::mg_mgr_poll(&mgr, 1000);
  }
}

void HttpServer::mongooseEventHandler(struct mg_connection* nc, int ev, void* evData) {
  switch (ev) {
    case MG_EV_HTTP_REQUEST: {
      ESP_LOGI(tag, "Mongoose: Incoming HTTP request");
      auto message = (struct http_message*)evData;
      string path(message->uri.p, message->uri.len);
      string method(message->method.p, message->method.len);
      auto resp = _handler->handleHttpRequest(method, path); 
      ::mg_send_head(nc, 200, resp.length(), "Content-Type: text/html");
      ::mg_printf(nc, "%s", resp.c_str());
      nc->flags |= MG_F_SEND_AND_CLOSE;
      break;
    }
  }
}