#include <stdio.h>
#include <string.h>
#include "driver/gpio.h"
//#include "nvs_flash.h"
//#include "esp_wifi.h"
#include "esp_event.h"
//#include "esp_http_client.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "esp_timer.h"
#include "freertos/semphr.h"

#define SLEEP_TIME_SECONDS 5

void light_sleep_task() {
    esp_sleep_enable_timer_wakeup(SLEEP_TIME_SECONDS * 1000000);
    esp_light_sleep_start();

    ESP_LOGI("Awaken", "O timer despertou placa");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}
