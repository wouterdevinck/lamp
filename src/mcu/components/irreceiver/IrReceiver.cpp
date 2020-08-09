#include "IrReceiver.h"
#include "constants.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/rmt.h"
#include "esp_log.h"

using namespace lamp;

static char tag[] = "IrReceiver";

IrReceiver::IrReceiver() {}

void IrReceiver::start(IIrHandler* handler) {
  ::xTaskCreate(&rxTask, "irRxTask", 2048, NULL, 4, NULL);
}

void IrReceiver::rxTask(void* parameters) {
  uint32_t length = 0;
  RingbufHandle_t rb = NULL;
  rmt_item32_t *items = NULL;

  rmt_config_t config;
  config.rmt_mode = RMT_MODE_RX;
  config.channel = RMT_CHANNEL_0;
  config.gpio_num = (gpio_num_t)PIN_IR_RX;
  config.clk_div = 80; // 100
  config.mem_block_num = 1;
  config.rx_config.idle_threshold = 12000; // 7600
  config.rx_config.filter_ticks_thresh = 100;
  config.rx_config.filter_en = true;

  ::rmt_config(&config);
  ::rmt_driver_install(RMT_CHANNEL_0, 1000, 0);
  ::rmt_get_ringbuf_handle(RMT_CHANNEL_0, &rb);
  ::rmt_rx_start(RMT_CHANNEL_0, true);

  while (true) {
    items = (rmt_item32_t*)::xRingbufferReceive(rb, &length, 1000);
    if (items) {
      // length /= 4;
      // ESP_LOGI(tag, "%d", length);
      ESP_LOG_BUFFER_HEX_LEVEL(tag, items, length, ESP_LOG_INFO);
      // TODO parse
      ::vRingbufferReturnItem(rb, (void*)items);
    } 
  }
}