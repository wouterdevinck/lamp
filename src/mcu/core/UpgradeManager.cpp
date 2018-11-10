#ifndef BASIC

#include "UpgradeManager.h"

#include <sstream>
#include <algorithm>

using namespace lamp;

void UpgradeManager::start() const {
  // TODO logs
  auto running = _updater->getRunningFpgaHash();
  auto installed = _updater->getInstalledFpgaHash();
  if (running != installed) {
    _updater->flashFpga();
  }
}

bool UpgradeManager::upgrade(string url) const {
  _logger->logInfo(_tag, "Starting upgrade");
  _logger->logDebug(_tag, "Upgrade URL: " + url);
  if (!_updater->beginUpgrade()) {
    _logger->logError(_tag, "Failed to start upgrade");
    return false;
  }
  map<string, string> headers = { { HTTP_RANGE, "" } };
  auto current = 0;
  auto total = CHUNK_SIZE;
  while (current < total) {
    ostringstream range;
    range << "bytes=" << current << "-" << min((current + CHUNK_SIZE), total) - 1;
    headers[HTTP_RANGE] = range.str();
    _logger->logDebug(_tag, "Downloading chunk: " + range.str());
    auto resp = _httpclient->request({ HTTP_GET, url, headers });
    _logger->logDebug(_tag, "Writing chunk to flash");
    if (!_updater->writeChunk(resp.body)) {
      _logger->logError(_tag, "Failed to write chunk");
      return false;
    }
    current += CHUNK_SIZE;
    auto header = resp.headers[HTTP_CONTENT_RANGE];
    total = atoi(header.substr(header.find("/") + 1).c_str());
  }
  _logger->logDebug(_tag, "Completing upgrade");
  if (!_updater->completeUpgrade()) {
    _logger->logError(_tag, "Failed to complete upgrade");
    return false;
  }
  _logger->logInfo(_tag, "Upgrade completed");
  return true;
}

string UpgradeManager::getVersion() const {
  return _updater->getRunningVersion();
}

#endif