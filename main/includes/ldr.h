#ifndef LDR_H_
#define LDR_H_

#include "driver/gpio.h"
#include "driver/adc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// 🔹 Definições do sensor LDR
#define LIGHT_SENSOR_PIN 4
#define ADC_2 2
#define NUM_SAMPLES 10  // Número de amostras para suavizar a leitura

#define ADC_MIN 350
#define ADC_MAX 2800

extern float ldr_illuminance;

// 🔹 Estrutura de dados para armazenar informações do LDR
typedef struct {
    uint8_t adc_device;  // Qual ADC está sendo usado (ADC1 ou ADC2)
    uint8_t adc_channel; // Canal do ADC conectado ao sensor LDR
    float k1;            // Parâmetro de calibração 1 (não está sendo usado no momento)
    float k2;            // Parâmetro de calibração 2 (não está sendo usado no momento)
} ldr_handle_t;

// 🔹 Protótipos das funções do LDR
int get_average_adc(ldr_handle_t *ldr);

float ldr_get_calibrated_illuminance(ldr_handle_t *ldr);

void ldr_task(void *params);

float get_ldr_illuminance();

#endif 
