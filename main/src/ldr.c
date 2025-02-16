#include "ldr.h"
#include "led_control.h"
#include "esp_log.h"

#define NUM_SAMPLES 10
#define ADC_MIN -300
#define ADC_MAX 3800

// Definição da variável global
float ldr_illuminance = 0.0;

void ldr_init() {
    adc2_config_channel_atten(ADC2_CHANNEL_0, ADC_ATTEN_DB_11);
}

int get_average_adc(ldr_handle_t *ldr) {
    int sum = 0;
    for (int i = 0; i < NUM_SAMPLES; i++) {
        int raw_value;
        adc2_get_raw(ldr->adc_channel, ADC_WIDTH_BIT_12, &raw_value);
        sum += raw_value;
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    return sum / NUM_SAMPLES;
}

float ldr_get_calibrated_illuminance(ldr_handle_t *ldr) {
    int raw_adc = get_average_adc(ldr);

    if (raw_adc < ADC_MIN) raw_adc = ADC_MIN;
    if (raw_adc > ADC_MAX) raw_adc = ADC_MAX;

    if (ADC_MAX == ADC_MIN) return 0.0;

    float illuminance = ((float)(raw_adc - ADC_MIN) / (ADC_MAX - ADC_MIN)) * 100.0;

    if (illuminance < 0.0) illuminance = 0.0;
    if (illuminance > 100.0) illuminance = 100.0;

    return illuminance;
}

void ldr_task(void *params) {
    ldr_handle_t ldr = {
        .adc_device = ADC_2,  
        .adc_channel = ADC2_CHANNEL_0, 
        .k1 = 49.95095,
        .k2 = 10.34034
    };

    while (true) {
        ldr_illuminance = 100 - ldr_get_calibrated_illuminance(&ldr); // Atualiza a variável global

        ESP_LOGI("LDR_TASK", "Iluminância capturada: %.1f%%", ldr_illuminance);

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

// Função para acessar o valor da variável global
float get_ldr_illuminance() {
    return ldr_illuminance;
}
