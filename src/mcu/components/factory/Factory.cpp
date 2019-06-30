#include "Factory.h"
#include "esp_log.h"
#include "cryptoauthlib.h"

static char tag[] = "Factory";

using namespace lamp;

bool Factory::isProvisioned() {
  return true; // TODO
}

void Factory::provision() {
  auto status = (ATCA_STATUS)init();
  if (status != ATCA_SUCCESS) {
    ESP_LOGE(tag, "Error ATCA: %d", status);
    return;
  }
  ESP_LOGI(tag, "Factory provisioning completed!");
  // esp_restart();
}

uint8_t Factory::init() {

  ATCAIfaceCfg cfg;
  cfg.iface_type             = ATCA_I2C_IFACE;
  cfg.devtype                = ATECC108A;
  cfg.atcai2c.slave_address  = 0XC0;
  cfg.atcai2c.bus            = 0;
  cfg.wake_delay             = 1500;
  cfg.rx_retries             = 20;

  ATCA_STATUS status;

  // INIT
  status = ::atcab_init(&cfg);
  if (status != ATCA_SUCCESS) return status;

  // REVISION
  uint8_t revision[4];
  status = ::atcab_info(revision);
  if (status != ATCA_SUCCESS) return status;
  ESP_LOGI(tag, "Revision = ");
  ESP_LOG_BUFFER_HEX_LEVEL(tag, revision, sizeof(revision), ESP_LOG_INFO);

  // RANDOM
  uint8_t randomnum[32];
  status = ::atcab_random(randomnum);
  if (status != ATCA_SUCCESS) return status;
  ESP_LOGI(tag, "Random = ");
  ESP_LOG_BUFFER_HEX_LEVEL(tag, randomnum, sizeof(randomnum), ESP_LOG_INFO);

  // SERIAL
  uint8_t serial[9];
  status = ::atcab_read_serial_number(serial);
  if (status != ATCA_SUCCESS) return status;
  ESP_LOGI(tag, "Serial = ");
  ESP_LOG_BUFFER_HEX_LEVEL(tag, serial, sizeof(serial), ESP_LOG_INFO);

  // CONFIG
  uint8_t config[128];
  status = ::atcab_read_config_zone(config);
  if (status != ATCA_SUCCESS) return status;
  ESP_LOGI(tag, "Config zone = ");
  ESP_LOG_BUFFER_HEX_LEVEL(tag, config, sizeof(config), ESP_LOG_INFO);

  // CONFIG LOCKED
  bool locked;
  status = ::atcab_is_locked(LOCK_ZONE_CONFIG, &locked);
  if (status != ATCA_SUCCESS) return status;
  ESP_LOGI(tag, "Config locked = %d", locked);

  // WRITE CONFIG
  // TODO?

  // LOCK CONFIG
  if(!locked) {
    status = ::atcab_lock_config_zone();
    if (status != ATCA_SUCCESS) return status;
    ESP_LOGI(tag, "Locked config");
  }

  // GENKEY
  uint8_t pub[ATCA_PUB_KEY_SIZE];
  status = ::atcab_genkey(0, pub);
  if (status != ATCA_SUCCESS) return status;
  ESP_LOGI(tag, "Public key = ");
  ESP_LOG_BUFFER_HEX_LEVEL(tag, pub, sizeof(pub), ESP_LOG_INFO);

  return ATCA_SUCCESS;

}