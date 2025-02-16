#include "water_sensor.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

void water_sensor_init() {
    esp_err_t ret = adc2_config_channel_atten(WATER_SENSOR_CHANNEL, ADC_ATTEN_DB_11);
    if (ret != ESP_OK) {
        printf("Erro na configuração do canal ADC2: %d\n", ret);
    }
}

int get_average_water_adc() {
    int sum = 0;
    int valid_samples = 0;
    for (int i = 0; i < NUM_SAMPLES; i++) {
        int raw_value;
        esp_err_t ret = adc2_get_raw(WATER_SENSOR_CHANNEL, ADC_WIDTH_BIT_12, &raw_value);
        if (ret == ESP_OK) {
            sum += raw_value;
            valid_samples++;
        }
        vTaskDelay(pdMS_TO_TICKS(10)); // Pequeno delay para reduzir ruído
    }
    return valid_samples > 0 ? sum / valid_samples : 0;
}

float water_get_calibrated_level() {
    int raw_adc = get_average_water_adc(); 

    // Garante que o ADC fique dentro dos valores calibrados
    raw_adc = (raw_adc < ADC_MIN) ? ADC_MIN : (raw_adc > ADC_MAX) ? ADC_MAX : raw_adc;

    if (ADC_MAX == ADC_MIN) return 0.0; // Evita divisão por zero

    // Mapeia os valores ADC para percentual de 0% a 100%
    float water_level = ((float)(raw_adc - ADC_MIN) / (ADC_MAX - ADC_MIN)) * 100.0;

    // Mantém os valores dentro dos limites aceitáveis
    return (water_level < 0.0) ? 0.0 : (water_level > 100.0) ? 100.0 : water_level;
}

void water_sensor_task(void *params) {
    
    water_sensor_init();

    while (true) {
        float water_level = water_get_calibrated_level();

        printf("Nível de Água Ajustado: %.1f%%\n", water_level);
        printf("--------------------\n");

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
