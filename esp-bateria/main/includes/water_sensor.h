#ifndef WATER_SENSOR_H
#define WATER_SENSOR_H

#include <math.h>
#include "driver/adc.h"
#include "driver/gpio.h"

#define WATER_SENSOR_CHANNEL ADC1_CHANNEL_6  // Agora usa ADC1 (GPIO34)
#define NUM_SAMPLES 64   // Número de amostras para suavizar a leitura
#define ADC_MIN 400      // Ajuste após testes para nível 0%
#define ADC_MAX 2000     // Antes era 3000, agora ajustamos para 2200 que correspondia a 45%
#define LED_GPIO GPIO_NUM_2  // LED de alerta para nível de água baixo

// Variáveis globais
extern float water_level;
extern bool led_state;

// Inicialização dos sensores e LED
void water_sensor_init();
void led_init();
void led_toggle();

// Funções de leitura e calibração do sensor de água
int get_average_water_adc();
float water_get_calibrated_level();

// Tarefa para monitoramento contínuo do sensor
void water_sensor_task(void *params);

#endif // WATER_SENSOR_H
