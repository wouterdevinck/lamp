#pragma once

#include "IHttpServer.h"
#include "IHttpHandler.h"
#include "mongoose.h"

namespace lamp {

  class HttpServer : public IHttpServer {

    public:
      explicit HttpServer() {}

      void start(int port, IHttpHandler* handler) override;
      void stop() override;

    private:
      TaskHandle_t _handle = NULL;
      static bool _started;
      static int _port;
      static IHttpHandler* _handler;
      static void mongooseTask(void* parameters);
      static void mongooseEventHandler(struct mg_connection* nc, int ev, void* evData);

  };

}