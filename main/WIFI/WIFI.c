#include "WIFI.h"

EventGroupHandle_t s_wifi_event_group;

void Event_handler_wifi(void *arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    WIFI_STA_connection *connection = (WIFI_STA_connection *)arg;

    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        ESP_LOGI("Wifi Connection","Wifi disconnected");
        esp_wifi_connect();
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI("Wifi Connection", "Connected with SSID: %s, ID: %d.%d.%d.%d", connection -> SSID_STA, IP2STR(&event->ip_info.ip));
    }
}

void WIFI_AP_config(void) {
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    wifi_config_t apCfg = {
        .ap = {
            .ssid = SSID_AP,
            .ssid_len = strlen(SSID_AP),
            .password = PASS_AP,
            .max_connection = 1,
            .authmode = WIFI_AUTH_WPA2_PSK}};

    if (strlen(PASS_AP) == 0) {
        apCfg.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &apCfg));
    ESP_ERROR_CHECK(esp_wifi_start());
}

void WIFI_STA_config(WIFI_STA_connection connection) {
    esp_netif_create_default_wifi_sta();

    // Create even wifi connection
    s_wifi_event_group = xEventGroupCreate();
    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        WIFI_EVENT,
        ESP_EVENT_ANY_ID,
        &Event_handler_wifi,
        (void *)&connection,
        &instance_any_id
    ));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        IP_EVENT,
        IP_EVENT_STA_GOT_IP,
        &Event_handler_wifi,
        (void *)&connection,
        &instance_got_ip
    ));

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    wifi_config_t staCfg = {
        .sta = {
            .ssid = {0},
            .password = {0},
            .threshold.authmode = WIFI_AUTH_WPA2_PSK
        }
    };
    memcpy(staCfg.sta.ssid, connection.SSID_STA, sizeof(staCfg.sta.ssid) - 1);
    memcpy(staCfg.sta.password, connection.PASS_STA, sizeof(staCfg.sta.password) - 1);

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &staCfg));
    ESP_ERROR_CHECK(esp_wifi_start());
}