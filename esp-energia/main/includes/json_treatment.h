#ifndef JSON_TREATMENT_H
#define JSON_TREATMENT_H

void send_dht_telemetry(void *temperature, void *humidity);

void send_dht_media_telemetry(void *temperature, void *humidity);

void mqtt_event_data_parser(char *data, char *topic);

void send_photo_telemetry(int *luminosity, int *light);

void send_heartbeat_telemetry(int *heartbeat);

void send_board_buzzer_attribute(int *buzzer_status);

void send_board_magnetic_attribute(int *magnetic_status);

void send_board_limit_temp_attribute(int *limit);

void send_board_led_attribute(int *led_status);

void send_shock_attribute(int *shocksensor);

void send_flame_telemetry(int *flame);

void send_flame_alert(int *flame);

void send_accumulated_heat_attribute(int32_t *flame);

#endif