#include "UpgradeManager.h"

using namespace lamp;

// TODO: logging

void UpgradeManager::boot() const {
  auto running = _updater->getRunningFpgaHash();
  auto installed = _updater->getInstalledFpgaHash();
  if (running != installed) {
    _updater->flashFpga();
  }
}

void UpgradeManager::upgrade(string url) const {
  map<string, string> headers = { 
    { "Range", "bytes=0-100" } 
  };
  const HttpRequest req {
    "GET", url, headers
  };
  auto resp = _httpclient->request(req);
}
