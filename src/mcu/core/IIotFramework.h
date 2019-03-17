#pragma once

#include "IIotClient.h"
#include "IIotHandler.h"

struct IIotFramework {

  virtual ~IIotFramework() noexcept = default;

  virtual IIotClient* connect(IIotHandler* handler) = 0;

};
