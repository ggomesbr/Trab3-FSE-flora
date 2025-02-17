#ifndef JSON_TREATMENT_H
#define JSON_TREATMENT_H

void send_water_telemetry(void *water_level);
void mqtt_event_data_parser(char *data, char *topic);

#endif