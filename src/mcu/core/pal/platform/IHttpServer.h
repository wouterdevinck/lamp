#pragma once

#include "IHttpHandler.h"

namespace lamp {

  struct IHttpHandler;

  struct IHttpServer {

    virtual ~IHttpServer() noexcept = default;

    virtual void start(int port, IHttpHandler* handler) = 0;
    virtual void stop() = 0;

  };

}