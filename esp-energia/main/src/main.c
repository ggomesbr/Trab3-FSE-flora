#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_log.h"

#include "ldr.h"
#include "dht11.h"
#include "water_sensor.h"

#define TAG "MAIN"

void app_main(void)
{
    esp_log_level_set(TAG, ESP_LOG_INFO);
    ESP_LOGI(TAG, "Iniciando o sistema de monitoramento...");

    // Criando as tarefas para cada sensor
    xTaskCreate(&ldr_task, "ldr_task", 2048, NULL, 5, NULL);
    xTaskCreate(&dht11_task, "dht11_task", 2048, NULL, 5, NULL);
    xTaskCreate(&water_sensor_task, "water_sensor_task", 2048, NULL, 5, NULL);

    ESP_LOGI(TAG, "Todas as tarefas foram iniciadas");
}
