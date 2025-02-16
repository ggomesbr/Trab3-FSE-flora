#ifndef WATER_SENSOR_H
#define WATER_SENSOR_H

#include <math.h>
#include "driver/adc.h"

#define WATER_SENSOR_CHANNEL ADC2_CHANNEL_2  
#define NUM_SAMPLES 64   // Número de amostras para suavizar a leitura
#define ADC_MIN 1000     // Ajuste após testes para nível 0%
#define ADC_MAX 2000     // Antes era 3000, agora ajustamos para 2200 que correspondia a 45%

void water_sensor_init();

int get_average_water_adc();

float water_get_calibrated_level();

void water_sensor_task(void *params);

#endif // WATER_SENSOR_H
