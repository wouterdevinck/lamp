#pragma once

#include "IIrReceiver.h"

namespace lamp {

  class IrReceiver : public IIrReceiver {

    public:
      explicit IrReceiver();

      void start(IIrHandler* handler) override;

    private:
      static void rxTask(void* parameters);


  };

}