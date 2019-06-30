#pragma once

#include "ILogger.h"

namespace lamp {

  class Logger : public ILogger {

    public:
      explicit Logger();

      void logError(string tag, string line) override;
      void logWarning(string tag, string line) override;
      void logInfo(string tag, string line) override;
      void logDebug(string tag, string line) override;
      void logVerbose(string tag, string line) override;

  };

}