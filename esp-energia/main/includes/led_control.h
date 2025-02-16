#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "driver/gpio.h"

// Definições dos pinos
#define LED_1 23
#define BOOT_BUTTON GPIO_NUM_0

// Função para iniciar o LED e o botão BOOT
void led_control_init(void);

// Task que controla o LED
void led_task(void *pvParameter);

#endif // LED_CONTROL_H