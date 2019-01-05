#include "MemMon.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

static char tag[] = "MemMon";

using namespace lamp;

MemMon::MemMon(uint16_t periodMs) {
  auto timer = ::xTimerCreate("memMonTask", periodMs / portTICK_PERIOD_MS, pdTRUE, 0, &memMonTask);
  ::xTimerStart(timer, 0);
}

void MemMon::memMonTask(void* parameters) {
  ESP_LOGD(tag, "Free heap: %d bytes", ::esp_get_free_heap_size());
}