#pragma once

#include "IUpdater.h"
#include "IHttpClient.h"

namespace lamp {

  struct UpgradeManager {

    public:
      explicit UpgradeManager(IUpdater* updater, IHttpClient* httpclient) : 
        _updater(updater), _httpclient(httpclient) {}

      void boot() const;
      void upgrade(string url) const;

    private:
      IUpdater* _updater;
      IHttpClient* _httpclient;

  };

}