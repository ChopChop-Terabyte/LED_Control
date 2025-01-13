#ifndef WIFI_H
#define WIFI_H

#include <string.h>
#include "esp_wifi.h"
#include "esp_event.h"

#include "esp_log.h"

#define SSID_AP "Smart Clock"
#define PASS_AP "bonsochin"

typedef struct {
    uint8_t SSID_STA[32];
    uint8_t PASS_STA[64];
}WIFI_STA_connection;

void WIFI_AP_config(void);
void WIFI_STA_config(WIFI_STA_connection connection);

#endif // WIFI_H