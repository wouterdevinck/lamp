#pragma once

#include <string>

using namespace std;

namespace lamp {

  struct ILogger {

    virtual ~ILogger() noexcept = default;

    virtual void logError(string tag, string line) = 0;
    virtual void logWarning(string tag, string line) = 0;
    virtual void logInfo(string tag, string line) = 0;
    virtual void logDebug(string tag, string line) = 0;
    virtual void logVerbose(string tag, string line) = 0;

  };

}
