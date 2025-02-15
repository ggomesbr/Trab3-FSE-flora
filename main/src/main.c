#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include <math.h>
#include "driver/adc.h"
#include "dht11.h"

#define TAG "MAIN"
#define LIGHT_SENSOR_PIN 4  

#define ADC_1 1
#define ADC_2 2

typedef struct {
  uint8_t adc_device;
  uint8_t adc_channel;
  float k1;
  float k2;
} ldr_handle_t;

// Ajuste do fator de deslocamento: onde o valor atual de 0% será redefinido para 60%
#define TARGET_PERCENTAGE 60.0

float ldr_get_calibrated_illuminance(ldr_handle_t * ldr, int adc_min, int adc_max) {
    int buffer = adc1_get_raw(ldr->adc_channel);

    // Garante que a leitura está dentro dos limites
    if (buffer < adc_min) buffer = adc_min;
    if (buffer > adc_max) buffer = adc_max;

    if (adc_max == adc_min) return 0.0; // Evita divisão por zero

    // Define o ponto de referência para 60%
    int adc_ref = adc_min + ((adc_max - adc_min) * (TARGET_PERCENTAGE / 100.0));

    // Ajusta a escala para que adc_ref seja 60%
    float illuminance = ((float)(buffer - adc_ref) / (adc_max - adc_ref)) * (100.0 - TARGET_PERCENTAGE) + TARGET_PERCENTAGE;

    // Mantém os valores entre 0% e 100%
    if (illuminance < 0.0) illuminance = 0.0;
    if (illuminance > 100.0) illuminance = 100.0;

    return illuminance;
}

void app_main()
{
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11);

    ldr_handle_t ldr = {
        .adc_device = ADC_1,
        .adc_channel = ADC1_CHANNEL_7,
        .k1 = 49.95095,
        .k2 = 10.34034
    };

    int adc_min = 4095; // Começa com o maior valor possível
    int adc_max = 0;    // Começa com o menor valor possível

    xTaskCreate(&dht11_task, "consulta_sensor", 2048, NULL, 3, NULL);

    while (true) {
        int raw_adc = adc1_get_raw(ADC1_CHANNEL_7);

        // Atualiza os valores mínimo e máximo detectados dinamicamente
        if (raw_adc < adc_min) adc_min = raw_adc;
        if (raw_adc > adc_max) adc_max = raw_adc;

        // Calcula a iluminância ajustada com base nos novos valores
        float illuminance = ldr_get_calibrated_illuminance(&ldr, adc_min, adc_max);

        printf("ADC Atual: %d\n", raw_adc);
        printf("ADC Mínimo Detectado: %d\n", adc_min);
        printf("ADC Máximo Detectado: %d\n", adc_max);
        printf("Iluminância Ajustada: %.1f%%\n", illuminance);
        printf("--------------------\n");

        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}
