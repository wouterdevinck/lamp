#include "Lamp.h"
#include "Platform.h"
#include "Factory.h"

using namespace lamp;

extern "C" {
  void app_main(void);
}

void app_main(void) {
  // init_crypto();
  auto factory = new Factory();
  if (factory->isProvisioned()) {
    auto platform = new Platform();
    auto lamp = new Lamp(platform);
    lamp->start();
  } else {
    factory->provision();
  }
}

/*
#include "nvscrt.h"
void test() {
  char* cn = readNvsCn();
  if (cn != NULL) {
    printf("CN = %s\n", cn);
  }
  char* pem = readNvsCertPem();
  if (pem != NULL) {
    printf("%s", pem);
  }
}
*/

//#include "cryptoauthlib.h"
//#include "esp_log.h"
/*void init_crypto(void) {
  ATCAIfaceCfg cfg;
  cfg.iface_type             = ATCA_I2C_IFACE;
  cfg.devtype                = ATECC108A;
  cfg.atcai2c.slave_address  = 0XC0;
  cfg.atcai2c.bus            = 0;
  cfg.wake_delay             = 1500;
  cfg.rx_retries             = 20;

  if (atcab_init(&cfg) != ATCA_SUCCESS) {
    ESP_LOGE("MAIN", "Failed to init ATECC608A!");
  }

  uint8_t rev[4];
  atcab_info(rev); 
  // TODO remove?

  // TEST
  uint8_t pubk[ATCA_PUB_KEY_SIZE];
  //if (atcab_read_pubkey(0, pubk) == ATCA_SUCCESS) {
  //  ESP_LOG_BUFFER_HEX("PUBLIC", pubk, ATCA_PUB_KEY_SIZE);
  //} else {
  //  ESP_LOGE("PUBLIC", "atcab_read_pubkey fail");
  //}
  if (atcab_get_pubkey(0, pubk) == ATCA_SUCCESS) {
    ESP_LOG_BUFFER_HEX("PUBLIC", pubk, ATCA_PUB_KEY_SIZE);
  } else {
    ESP_LOGE("PUBLIC", "atcab_get_pubkey fail");
  }
}*/