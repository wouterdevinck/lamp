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
  _updater->beginUpgrade(); // TODO check result

  // TODO loop
  auto headers = map<string, string>();
  headers.insert({ "Range", "bytes=0-100" });
  auto resp = _httpclient->request({ "GET", url, headers });
  _updater->writeChunk(resp.body);

  _updater->completeUpgrade(); // TODO check result
}
