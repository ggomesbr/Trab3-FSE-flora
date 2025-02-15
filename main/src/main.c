#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#include "dht11.h"

#define TAG "MAIN"

void app_main()
{
    xTaskCreate(dht11_task, "DHT11 Task", 2048, NULL, 5, NULL);

}