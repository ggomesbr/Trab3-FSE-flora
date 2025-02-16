#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "driver/gpio.h"

#include "water_sensor.h"
#include "light_sleep.h"

#define TAG "MAIN"
#define LED_GPIO GPIO_NUM_2

float water_level;
bool led_state = false;

void led_init() {
    gpio_reset_pin(LED_GPIO);
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
}

void led_toggle() {
    led_state = !led_state;
    gpio_set_level(LED_GPIO, led_state);
}

void app_main(void)
{
    led_init();

    esp_log_level_set(TAG, ESP_LOG_INFO);
    ESP_LOGI(TAG, "Iniciando o sistema de monitoramento...");

    xTaskCreate(&water_sensor_task, "water_sensor_task", 2048, NULL, 5, NULL);

    ESP_LOGI(TAG, "Todas as tarefas foram iniciadas");

    while (1) {
        water_level = water_get_calibrated_level();
        
        if(water_level < 10.0) {
            ESP_LOGE(TAG, "Nível de água muito baixo! %.2f%%", water_level);
            led_toggle();
        } else {
            gpio_set_level(LED_GPIO, false);
            ESP_LOGI(TAG, "Nível de água: %.2f%%", water_level);
            ESP_LOGI(TAG, "Entrando em light sleep");
            light_sleep_task();
        }

        ESP_LOGI(TAG, "Acordando do light sleep");

        vTaskDelay(pdMS_TO_TICKS(1000));  // Delay de 1 segundo
    }
}
