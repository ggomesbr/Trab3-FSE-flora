#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_log.h"
#include "ldr.h"
#include "dht11.h"

#include "mqtt.h"
#include "freertos/semphr.h"
#include "mqtt.h"
#include "led_control.h"

#define TAG "MAIN"

SemaphoreHandle_t conexaoMQTTSemaphore;


void app_main(void)
{
    esp_log_level_set(TAG, ESP_LOG_INFO);
    ESP_LOGI(TAG, "Iniciando o sistema de monitoramento...");

    conexaoMQTTSemaphore = xSemaphoreCreateBinary();

    xTaskCreate(&comunicacao_servidor_task, "comunicacao_broker", 4096, NULL, 1, NULL);

    // Criando as tarefas para cada sensor
    xTaskCreate(&ldr_task, "ldr_task", 2048, NULL, 5, NULL);
    xTaskCreate(&dht11_task, "dht11_task", 2048, NULL, 5, NULL);

    led_control_init();
    xTaskCreate(&led_task, "led_task", 2048, NULL, 5, NULL);

    ESP_LOGI(TAG, "Todas as tarefas foram iniciadas");
}