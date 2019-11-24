#include "Factory.h"

#include "esp_log.h"
#include "esp_err.h"
#include "driver/uart.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

#include "cryptoauthlib.h"
#include "atcacert/atcacert_client.h"
#include "atcacert/atcacert_def.h"

#include "nvscrt.h"

#include <stdio.h>
#include <string.h>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <iterator>

static char tag[] = "Factory";

using namespace lamp;
using namespace std;

Factory::Factory() {
  uint8_t rev[4];
  ATCAIfaceCfg cfg;
  cfg.iface_type             = ATCA_I2C_IFACE;
  cfg.devtype                = ATECC608A;
  cfg.atcai2c.slave_address  = 0XC0;
  cfg.atcai2c.bus            = 0;
  cfg.wake_delay             = 1500;
  cfg.rx_retries             = 20;
  if (::atcab_init(&cfg) != ATCA_SUCCESS) {
    ESP_LOGE(tag, "atcab_init failed");
    return;
  }
  // TODO remove?
  if (::atcab_info(rev) != ATCA_SUCCESS) {
    ESP_LOGE(tag, "atcab_info failed");
    return;
  }
}

bool Factory::isProvisioned() {


  // TODO centralize
  /*uint8_t rev[4];
  ATCAIfaceCfg cfg;
  cfg.iface_type             = ATCA_I2C_IFACE;
  cfg.devtype                = ATECC608A;
  cfg.atcai2c.slave_address  = 0XC0;
  cfg.atcai2c.bus            = 0;
  cfg.wake_delay             = 1500;
  cfg.rx_retries             = 20;
  if (::atcab_init(&cfg) != ATCA_SUCCESS) {
    ESP_LOGE(tag, "atcab_init failed");
    return;
  }*/


  // TODO deduplicate
  auto bin2hex = [](uint8_t* data, uint8_t len) -> string {
    stringstream ss;
    ss << hex;
    for(uint32_t i = 0; i < len; ++i) {
      ss << setw(2) << setfill('0') << (int)data[i];
    }
    return ss.str();
  };



  // 1. Check if the config zone is locked
  bool configLocked;
  auto status = ::atcab_is_locked(LOCK_ZONE_CONFIG, &configLocked);
  if (status != ATCA_SUCCESS)  {
    ESP_LOGE(tag, "atcab_is_locked LOCK_ZONE_CONFIG failed");
    return false;
  };
  ESP_LOGI(tag, "configLocked = %s", configLocked ? "yes" : "no");
  // 2. Check if the data zone is locked
  bool dataLocked;
  status = ::atcab_is_locked(LOCK_ZONE_DATA, &dataLocked);
  if (status != ATCA_SUCCESS)  {
    ESP_LOGE(tag, "atcab_is_locked LOCK_ZONE_DATA failed");
    return false;
  }
  ESP_LOGI(tag, "dataLocked = %s", dataLocked ? "yes" : "no");
  // 3. Check if a certificate with the right CN is present in NVS
  bool cnCorrect = false;
  uint8_t serData[9];
  status = ::atcab_read_serial_number(serData);
  if (status != ATCA_SUCCESS) {
    ESP_LOGE(tag, "atcab_read_serial_number failed");
    return false;
  }
  auto ser = bin2hex(serData, sizeof(serData));
  auto cn = ::readNvsCn();  
  if (cn == NULL) {
    ESP_LOGI(tag, "CN from NVS is NULL - no certificate provisioned?");
  } else {
    auto res = ::strcmp(ser.c_str(), cn);
    if (res == 0) {
      cnCorrect = true;
    }
  }
  ESP_LOGI(tag, "cnCorrect = %s - serial = %s - cn = %s", cnCorrect ? "yes" : "no", ser.c_str(), cn);
  // Return true only if all checks have passed
  return configLocked && dataLocked && cnCorrect;
}

void Factory::provision() {

  // TODO - Centralize config
  uint8_t rev[4];
  ATCAIfaceCfg cfg;
  cfg.iface_type             = ATCA_I2C_IFACE;
  cfg.devtype                = ATECC608A;
  cfg.atcai2c.slave_address  = 0XC0;
  cfg.atcai2c.bus            = 0;
  cfg.wake_delay             = 1500;
  cfg.rx_retries             = 20;
  if (::atcab_init(&cfg) != ATCA_SUCCESS) {
    ESP_LOGE(tag, "atcab_init failed");
    return;
  }
  if (::atcab_info(rev) != ATCA_SUCCESS) {
    ESP_LOGE(tag, "atcab_info failed");
    return;
  }

  ::esp_log_level_set("*", ESP_LOG_NONE); // Disable most logs
  printf(" === FACTORY PROVISIONING MODE ===\n");
  ::uart_driver_install(UART, BUF_SIZE * 2, 0, 0, NULL, 0);
  uint8_t *data = (uint8_t *)malloc(BUF_SIZE);
  ostringstream line;
  while(1) {
    int len = ::uart_read_bytes(UART, data, BUF_SIZE, 20 / portTICK_RATE_MS);
    string buffer(reinterpret_cast<char const*>(data), len);
    auto pos = buffer.find("\n");
    while(pos != string::npos) {
      line << buffer.substr(0, pos + 1);
      process(line.str());
      line.str("");
      line.clear();
      buffer.erase(0, pos + 1);
      pos = buffer.find("\n");
    }
    line << buffer;
  }
}

void Factory::process(string line) {
  auto bin2hex = [](uint8_t* data, uint8_t len) -> string {
    stringstream ss;
    ss << hex;
    for(uint32_t i = 0; i < len; ++i) {
      ss << setw(2) << setfill('0') << (int)data[i];
    }
    return ss.str();
  };
  auto hex2bin = [](string hex) -> vector<char> {
    vector<char> bytes;
    for (unsigned int i = 0; i < hex.length(); i += 2) {
      string byteString = hex.substr(i, 2);
      char byte = (char)strtol(byteString.c_str(), NULL, 16);
      bytes.push_back(byte);
    }
    return bytes;
  };
  istringstream iss(line);
  vector<string> parts(istream_iterator<string>{iss}, istream_iterator<string>());
  string cmd = parts[0];
  ostringstream resp;
  resp << cmd << " ";
  if(cmd.compare("test") == 0) {
    resp << "ok";
  } else if(cmd.compare("revision") == 0) {
    uint8_t data[4];
    auto status = ::atcab_info(data);
    if (status != ATCA_SUCCESS) {
      resp << "fail " << (unsigned)status;
    } else {
      resp << "ok " << bin2hex(data, sizeof(data));
    }
  } else if(cmd.compare("random") == 0) {
    uint8_t data[32];
    auto status = ::atcab_random(data);
    if (status != ATCA_SUCCESS) {
      resp << "fail " << (unsigned)status;
    } else {
      resp << "ok " << bin2hex(data, sizeof(data));
    }
  } else if(cmd.compare("serial") == 0) {
    uint8_t data[9];
    auto status = ::atcab_read_serial_number(data);
    if (status != ATCA_SUCCESS) {
      resp << "fail " << (unsigned)status;
    } else {
      resp << "ok " << bin2hex(data, sizeof(data));
    }
  } else if(cmd.compare("readconfig") == 0) {
    uint8_t data[128];
    auto status = ::atcab_read_config_zone(data);
    if (status != ATCA_SUCCESS) {
      resp << "fail " << (unsigned)status;
    } else {
      resp << "ok " << bin2hex(data, sizeof(data));
    }
  } else if(cmd.compare("configlocked") == 0) {
    bool locked;
    auto status = ::atcab_is_locked(LOCK_ZONE_CONFIG, &locked);
    if (status != ATCA_SUCCESS) {
      resp << "fail " << (unsigned)status;
    } else {
      resp << "ok " << (locked ? "true" : "false");
    }
  } else if(cmd.compare("writeconfig") == 0) {
    if (parts.size() != 2) {
      resp << "fail input";
    } else {
      auto config = hex2bin(parts[1]);
      auto status = ::atcab_write_bytes_zone(ATCA_ZONE_CONFIG, 0, 16, &config[0], (uint16_t)config.size());
      if (status != ATCA_SUCCESS) {
        resp << "fail " << (unsigned)status;
      } else {
        resp << "ok";
      }
    }
  } else if(cmd.compare("lockconfig") == 0) {
    auto status = ::atcab_lock_config_zone();
    if (status != ATCA_SUCCESS) {
      resp << "fail " << (unsigned)status;
    } else {
      resp << "ok";
    }
  } else if(cmd.compare("genkey") == 0) {
    uint8_t data[ATCA_PUB_KEY_SIZE];
    auto status = ::atcab_genkey(1, data);
    if (status != ATCA_SUCCESS) {
      resp << "fail " << (unsigned)status;
    } else {
      resp << "ok " << bin2hex(data, sizeof(data));
    }
  } else if(cmd.compare("signcsr") == 0) {
    if (parts.size() != 8) {
      resp << "fail input";
    } else {
      uint8_t data[512];
      size_t len = sizeof(data);
      auto tmp = hex2bin(parts[1]);
      atcacert_def_t csrDef = {
        .type                 = CERTTYPE_X509,
        .template_id          = 3,
        .chain_id             = 0,
        .private_key_slot     = 1,
        .sn_source            = SNSRC_PUB_KEY_HASH,
        .cert_sn_dev_loc      = {
          .zone               = DEVZONE_NONE,
          .slot               = 0,
          .is_genkey          = 0,
          .offset             = 0,
          .count              = 0
        },
        .issue_date_format    = DATEFMT_RFC5280_UTC,
        .expire_date_format   = DATEFMT_RFC5280_UTC,
        .tbs_cert_loc         = {
          .offset             = (uint16_t)atoi(parts[2].c_str()),
          .count              = (uint16_t)atoi(parts[3].c_str())
        },
        .expire_years         = 0,
        .public_key_dev_loc   = {
          .zone               = DEVZONE_NONE,
          .slot               = 1,
          .is_genkey          = 1,
          .offset             = 0,
          .count              = 64
        },
        .comp_cert_dev_loc    = {
          .zone               = DEVZONE_NONE,
          .slot               = 0,
          .is_genkey          = 0,
          .offset             = 0,
          .count              = 0
        },
        .std_cert_elements    = {
          {   // STDCERT_PUBLIC_KEY
            .offset           = (uint16_t)atoi(parts[4].c_str()),
            .count            = (uint16_t)atoi(parts[5].c_str())
          },
          {   // STDCERT_SIGNATURE
            .offset           = (uint16_t)atoi(parts[6].c_str()),
            .count            = (uint16_t)atoi(parts[7].c_str())
          }
        },
        .cert_elements        = NULL,
        .cert_elements_count  = 0,
        .cert_template        = &tmp[0],
        .cert_template_size   = (uint16_t)tmp.size()
      };
      auto status = ::atcacert_create_csr(&csrDef, data, &len);
      if (status != ATCA_SUCCESS) {
        resp << "fail " << (unsigned)status;
      } else {
        resp << "ok " << bin2hex(data, len);
      }
    }
  } else if(cmd.compare("writecert") == 0) {
    if (parts.size() != 2) {
      resp << "fail input";
    } else {
      auto certData = hex2bin(parts[1]);
      auto ret = ::writeNvsCertDer(&certData[0], certData.size());
      if (ret == 0) {
        resp << "fail " << ret;
      } else {
        resp << "ok";
      }
    }
  } else if(cmd.compare("datalocked") == 0) {
    bool locked;
    auto status = ::atcab_is_locked(LOCK_ZONE_DATA, &locked);
    if (status != ATCA_SUCCESS) {
      resp << "fail " << (unsigned)status;
    } else {
      resp << "ok " << (locked ? "true" : "false");
    }
  } else if(cmd.compare("lockdata") == 0) {
    auto status = ::atcab_lock_data_zone();
    if (status != ATCA_SUCCESS) {
      resp << "fail " << (unsigned)status;
    } else {
      resp << "ok";
    }
  } else if(cmd.compare("restart") == 0) {
    auto timer = ::xTimerCreate("restartHandler", 2000 / portTICK_PERIOD_MS, pdTRUE, 0, &restartHandler);
    ::xTimerStart(timer, 0);
    resp << "ok";
  }
  resp << "\n";
  auto out = resp.str();
  ::uart_write_bytes(UART, out.c_str(), out.length());
}

void Factory::restartHandler(void* parameters) {
  ::esp_restart(); 
}