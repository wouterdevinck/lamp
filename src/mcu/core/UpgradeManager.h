#pragma once

#include "IUpdater.h"
#include "IHttpClient.h"

#define CHUNK_SIZE 100 * 1024

namespace lamp {

  struct UpgradeManager {

    public:
      explicit UpgradeManager(IUpdater* updater, IHttpClient* httpclient) : 
        _updater(updater), _httpclient(httpclient) {}

      void boot() const;
      bool upgrade(string url) const;
      string getVersion() const;

    private:
      IUpdater* _updater;
      IHttpClient* _httpclient;

  };

}