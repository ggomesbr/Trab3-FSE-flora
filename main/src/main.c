#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include <math.h>
#include "driver/adc.h"
#include "dht11.h"

#include "ldr.h"

#define TAG "MAIN"


void app_main()
{

    xTaskCreate(&ldr_task, "consulta_ldr", 2048, NULL, 3,NULL);
    xTaskCreate(&dht11_task, "consulta_dht11", 2048, NULL, 3, NULL);
}
