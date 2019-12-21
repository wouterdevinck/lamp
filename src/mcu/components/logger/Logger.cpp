#include "Logger.h"
#include "esp_log.h"

using namespace lamp;

Logger::Logger() {
  ::esp_log_level_set("phy_init", ESP_LOG_INFO); 
}

void Logger::logError(string tag, string line) {
  esp_log_write(ESP_LOG_ERROR, tag.c_str(), LOG_COLOR_E "E (%d) %s: %s" LOG_RESET_COLOR "\n", esp_log_timestamp(), tag.c_str(), line.c_str());
}

void Logger::logWarning(string tag, string line) {
  esp_log_write(ESP_LOG_WARN, tag.c_str(), LOG_COLOR_W "W (%d) %s: %s" LOG_RESET_COLOR "\n", esp_log_timestamp(), tag.c_str(), line.c_str());
}

void Logger::logInfo(string tag, string line) {
  esp_log_write(ESP_LOG_INFO, tag.c_str(), LOG_COLOR_I "I (%d) %s: %s" LOG_RESET_COLOR "\n", esp_log_timestamp(), tag.c_str(), line.c_str());
}

void Logger::logDebug(string tag, string line) {
  esp_log_write(ESP_LOG_DEBUG, tag.c_str(), LOG_COLOR_D "D (%d) %s: %s" LOG_RESET_COLOR "\n", esp_log_timestamp(), tag.c_str(), line.c_str());
}

void Logger::logVerbose(string tag, string line)  {
  esp_log_write(ESP_LOG_VERBOSE, tag.c_str(), LOG_COLOR_V "V (%d) %s: %s" LOG_RESET_COLOR "\n", esp_log_timestamp(), tag.c_str(), line.c_str());
}