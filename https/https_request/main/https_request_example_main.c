#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "mongoose.h"

static const char *TAG = "mongoose_https";

// WiFi event handler
static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data) {
  if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
    esp_wifi_connect();
  } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
    esp_wifi_connect();
    ESP_LOGI(TAG, "Retrying WiFi connection...");
  } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
    ESP_LOGI(TAG, "Got IP address");
  }
}

static void wifi_init(void) {
  esp_netif_init();
  esp_event_loop_create_default();
  esp_netif_create_default_wifi_sta();

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  esp_wifi_init(&cfg);
  esp_event_handler_instance_t instance_any_id;
  esp_event_handler_instance_t instance_got_ip;

  esp_event_handler_instance_register(WIFI_EVENT,
                                      ESP_EVENT_ANY_ID,
                                      &wifi_event_handler,
                                      NULL,
                                      &instance_any_id);
  esp_event_handler_instance_register(IP_EVENT,
                                      IP_EVENT_STA_GOT_IP,
                                      &wifi_event_handler,
                                      NULL,
                                      &instance_got_ip);

  wifi_config_t wifi_config = {
      .sta = {
          .ssid = "Pixle_4666",
          .password = "123123123123",
          .threshold.authmode = WIFI_AUTH_WPA2_PSK,
      },
  };

  esp_wifi_set_mode(WIFI_MODE_STA);
  esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
  esp_wifi_start();
}

// Mongoose event handler
static void mongoose_event_handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    ESP_LOGI(TAG, "HTTPS response:\n%.*s", (int)hm->body.len, hm->body.ptr);
    c->is_closing = 1;
  }
  (void) fn_data;
}

// FreeRTOS task to run Mongoose event loop and perform HTTPS GET
void mongoose_task(void *param) {
  struct mg_mgr mgr;
  mg_mgr_init(&mgr);

  // Connect to HTTPS server
  mg_http_connect(&mgr, "https://api.telegram.org/bot8141510479:AAE-bvquH4CMHQJ_CrNqwIE7J74NnWp6Ki4/getUpdates", mongoose_event_handler, NULL);

  while (1) {
    mg_mgr_poll(&mgr, 1000);
  }

  mg_mgr_free(&mgr);
  vTaskDelete(NULL);
}

void app_main(void) {
  // Initialize NVS (required by WiFi)
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    nvs_flash_erase();
    nvs_flash_init();
  }

  wifi_init();

  // Delay to ensure WiFi connects
  vTaskDelay(pdMS_TO_TICKS(5000));

  // Start Mongoose task
  xTaskCreate(mongoose_task, "mongoose_task", 8192, NULL, 5, NULL);
}
