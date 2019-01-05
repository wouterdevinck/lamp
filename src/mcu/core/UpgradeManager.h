#pragma once

#include "IUpdater.h"
#include "IHttpClient.h"
#include "ILogger.h"

namespace lamp {

  class UpgradeManager {

    public:
      explicit UpgradeManager(IUpdater* updater, ILogger* logger, IHttpClient* httpclient) : 
        _updater(updater), _logger(logger), _httpclient(httpclient) {}

      void start() const;
      bool upgrade(string url) const;
      string getVersion() const;

    private:
      IUpdater* _updater;
      ILogger* _logger;
      IHttpClient* _httpclient;
      const string _tag = "Core.UpgradeManager";

  };

}