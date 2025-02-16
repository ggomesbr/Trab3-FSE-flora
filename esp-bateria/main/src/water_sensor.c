#include "water_sensor.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "esp_log.h"

#define TAG "WATER_SENSOR"

static esp_adc_cal_characteristics_t adc_chars;

void water_sensor_init() {
    adc2_config_channel_atten(WATER_SENSOR_CHANNEL, ADC_ATTEN_DB_11);
    esp_adc_cal_characterize(ADC_UNIT_2, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 0, &adc_chars);
}

int get_average_water_adc() {
    int sum = 0;
    int valid_samples = 0;
    for (int i = 0; i < NUM_SAMPLES; i++) {
        int raw_value;
        if (adc2_get_raw(WATER_SENSOR_CHANNEL, ADC_WIDTH_BIT_12, &raw_value) == ESP_OK) {
            sum += raw_value;
            valid_samples++;
        }
    }
    return valid_samples > 0 ? sum / valid_samples : 0;
}

float water_get_calibrated_level() {
    int raw_adc = get_average_water_adc();
    raw_adc = (raw_adc < ADC_MIN) ? ADC_MIN : (raw_adc > ADC_MAX) ? ADC_MAX : raw_adc;

    if (ADC_MAX == ADC_MIN) return 0.0;

    float water_level = ((float)(raw_adc - ADC_MIN) / (ADC_MAX - ADC_MIN)) * 100.0;
    return (water_level < 0.0) ? 0.0 : (water_level > 100.0) ? 100.0 : water_level;
}

void water_sensor_task(void *pvParameters) {
    water_sensor_init();

    while (1) {
        float water_level = water_get_calibrated_level();
        ESP_LOGI(TAG, "Nível de Água: %.2f%%", water_level);

        // Aqui você pode adicionar código para enviar os dados via MQTT, se necessário

        // Aguardar até o próximo ciclo
        vTaskDelay(pdMS_TO_TICKS(100));  // Pequeno delay para evitar sobrecarga da CPU
    }
}
