#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_log.h"
#include "ldr.h"
#include "dht11.h"
#include "esp_http_client.h"
#include "nvs_flash.h"


#include "freertos/semphr.h"
#include "mqtt.h"
#include "led_control.h"
#include "wifi.h"

#define TAG "MAIN"

SemaphoreHandle_t connectionWifiSemaphore;
SemaphoreHandle_t connectionMQTTSemaphore;

float temp_media = 0;
float humidity_media = 0;

void wifi_connected(void * params)
{
  while(true)
  {
    if(xSemaphoreTake(connectionWifiSemaphore, portMAX_DELAY))
    {
      // Processamento Internet
      mqtt_start();
      
    }
  }
}


void app_main(void)
{
    esp_log_level_set(TAG, ESP_LOG_INFO);
    ESP_LOGI(TAG, "Iniciando o sistema de monitoramento...");


    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    
    connectionWifiSemaphore = xSemaphoreCreateBinary();
    connectionMQTTSemaphore = xSemaphoreCreateBinary();

    wifi_start();

    xTaskCreate(&wifi_connected,  "Conexão ao MQTT", 4096, NULL, 1, NULL);

    // Criando as tarefas para cada sensor
    xTaskCreate(&ldr_task, "ldr_task", 2048, NULL, 5, NULL);
    xTaskCreate(&dht11_task, "dht11_task", 2048, NULL, 5, NULL);
    led_control_init();
    xTaskCreate(&led_task, "led_task", 2048, NULL, 2, NULL);

    ESP_LOGI(TAG, "Todas as tarefas foram iniciadas");
}