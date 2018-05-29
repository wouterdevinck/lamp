#include "UpgradeManager.h"

#include <sstream>
#include <algorithm>

using namespace lamp;

// TODO: logging

void UpgradeManager::boot() const {
  auto running = _updater->getRunningFpgaHash();
  auto installed = _updater->getInstalledFpgaHash();
  if (running != installed) {
    _updater->flashFpga();
  }
}

bool UpgradeManager::upgrade(string url) const {
  if (!_updater->beginUpgrade()) {
    return false;
  }
  map<string, string> headers = { { HTTP_RANGE, "" } };
  auto current = 0;
  auto total = CHUNK_SIZE;
  while (current < total) {
    ostringstream range;
    range << "bytes=" << current << "-" << min((current + CHUNK_SIZE), total) - 1;
    headers[HTTP_RANGE] = range.str();
    auto resp = _httpclient->request({ HTTP_GET, url, headers });
    if (!_updater->writeChunk(resp.body)) {
      return false;
    }
    current += CHUNK_SIZE;
    auto header = resp.headers[HTTP_CONTENT_RANGE];
    total = stoi(header.substr(header.find("/") + 1));
  }
  if (!_updater->completeUpgrade()) {
    return false;
  }
}

string UpgradeManager::getVersion() const {
  return _updater->getRunningVersion();
}