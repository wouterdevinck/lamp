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
  uint16_t chunkSize = _updater->getPreferredChunkSize();
  _logger->logDebug(_tag, "Chunk size: " + chunkSize); // BUG
  if (!_updater->beginUpgrade()) {
    _logger->logError(_tag, "Failed to start upgrade");
    return false;
  }
  map<string, string> headers = { { HTTPCLIENT_RANGE, "" } };
  uint32_t current = 0;
  uint32_t total = chunkSize;
  while (current < total) {
    ostringstream range;
    range << "bytes=" << current << "-" << min((uint32_t)(current + chunkSize), total) - 1;
    headers[HTTPCLIENT_RANGE] = range.str();
    _logger->logDebug(_tag, "Downloading chunk: " + range.str());
    auto resp = _httpclient->request({ HTTPCLIENT_GET, url, headers });
    // TODO Check status code (expect 206 Partial Content)
    _logger->logDebug(_tag, "Writing chunk to flash");
    if (!_updater->writeChunk(resp.body)) {
      _logger->logError(_tag, "Failed to write chunk");
      return false;
    }
    current += chunkSize;
    auto header = resp.headers[HTTPCLIENT_CONTENT_RANGE];
    // TODO Check header existence
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