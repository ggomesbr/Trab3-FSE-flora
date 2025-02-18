#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "esp_http_client.h"
#include "nvs_flash.h"

#include "water_sensor.h"
#include "freertos/semphr.h"
#include "mqtt.h"
#include "wifi.h"

#define TAG "MAIN"

SemaphoreHandle_t connectionWifiSemaphore;
SemaphoreHandle_t connectionMQTTSemaphore;

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

void handle_server_communication(void * params)
{
  char mensagem[50];
  char jsonAtributos[200];void handle_server_communication(void * params)
  {
    char mensagem[50];
    char jsonAtributos[200];
    if(xSemaphoreTake(connectionMQTTSemaphore, portMAX_DELAY))
    {
      while(true)
      {
         float temp = 20.0 + (float)rand()/(float)(RAND_MAX/10.0);
         
        //  sprintf(mensagem, "{\"temperature\": %f}", temp);
        //  mqtt_envia_mensagem("v1/devices/me/telemetry", mensagem);
  
        //  sprintf(jsonAtributos, "{\"quantidade de pinos\": 5, \n\"umidade\": 20}");
        //  mqtt_envia_mensagem("v1/devices/me/attributes", jsonAtributos);
  
         vTaskDelay(3000 / portTICK_PERIOD_MS);
      }
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
    //xTaskCreate(&handle_server_communication, "Comunicação com Broker", 4096, NULL, 1, NULL);
    xTaskCreate(&water_sensor_task, "water_sensor_task", 2048, NULL, 2, NULL);
  
    ESP_LOGI(TAG, "Todas as tarefas foram iniciadas");
}
