#pragma once

#include "IUpdater.h"
#include "IHttpClient.h"
#include "ILogger.h"

#define CHUNK_SIZE 50 * 1024 // TODO Make this a config somewhere (re-use in httpclient implementation for receive buffer size)

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