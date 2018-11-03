#pragma once

#include "IIrReceiver.h"
#include "IIrHandler.h"

#define CONV(x) ((F_CPU/8)/(1000000/(x)))

namespace lamp {

  class IrReceiver : public IIrReceiver {

    public:
      explicit IrReceiver();

      void start(IIrHandler* handler) override;
      void loop();

    private:
      IIrHandler* _handler; 

  };

}