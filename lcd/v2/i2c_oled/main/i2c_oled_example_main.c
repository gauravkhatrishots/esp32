/*
 * SPDX-FileCopyrightText: 2021-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/i2c_master.h"
#include "esp_lvgl_port.h"
#include "lvgl.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define DHT_GPIO GPIO_NUM_4

// static const char *TAG = "DHT22";//?/

// Delay functions for microseconds and milliseconds
static void delay_us(int us) {
    delay_us(us);
}

// Function to read DHT22 sensor
static esp_err_t dht22_read(float *temperature, float *humidity) {
    uint8_t bits[5] = {0};
    uint8_t byte_idx = 0, bit_idx = 7;

    // Step 1: Send start signal
    gpio_set_direction(DHT_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(DHT_GPIO, 0);
    vTaskDelay(pdMS_TO_TICKS(20));  // pull low for at least 1 ms (20 ms for safety)
    gpio_set_level(DHT_GPIO, 1);
    delay_us(40);
    gpio_set_direction(DHT_GPIO, GPIO_MODE_INPUT);

    // Step 2: Wait for sensor response
    int timeout = 10000;
    while (gpio_get_level(DHT_GPIO) == 1 && --timeout) {
        delay_us(1);
    }

    timeout = 10000;
    while (gpio_get_level(DHT_GPIO) == 0 && --timeout) {
        delay_us(1);
    }

    timeout = 10000;
    while (gpio_get_level(DHT_GPIO) == 1 && --timeout) {
        delay_us(1);
    }
\
    // Step 3: Read 40 bits (5 bytes)
    for (int i = 0; i < 40; i++) {
        // Wait for low signal start
        timeout = 10000;
        while (gpio_get_level(DHT_GPIO) == 0 && --timeout) {
            delay_us(1);
        }
        if (timeout == 0) {
         //   ESP_LOGE(TAG, "Timeout waiting for bit start");
            return ESP_FAIL;
        }

        // Measure length of high signal
        int pulse_length = 0;
        timeout = 10000;
        while (gpio_get_level(DHT_GPIO) == 1 && --timeout) {
            delay_us(1);
            pulse_length++;
        }
        if (timeout == 0) {
           // ESP_LOGE(TAG, "Timeout waiting for bit end");
            return ESP_FAIL;
        }

        // If high pulse > 40 us => bit = 1 else 0 (typical threshold)
        if (pulse_length > 40) {
            bits[byte_idx] |= (1 << bit_idx);
        }

        if (bit_idx == 0) {
            bit_idx = 7;
            byte_idx++;
        } else {
            bit_idx--;
        }
    }

    // Step 4: Checksum verification
    uint8_t checksum = bits[0] + bits[1] + bits[2] + bits[3];
    if (checksum != bits[4]) {
    //    ESP_LOGE(TAG, "Checksum failed");
        return ESP_FAIL;
    }

    // Step 5: Convert data
    uint16_t raw_humidity = (bits[0] << 8) | bits[1];
    uint16_t raw_temperature = (bits[2] << 8) | bits[3];

    *humidity = raw_humidity / 10.0;
    if (raw_temperature & 0x8000) {  // negative temperature
        raw_temperature &= 0x7FFF;
        *temperature = -((float)raw_temperature) / 10.0;
    } else {
        *temperature = ((float)raw_temperature) / 10.0;
    }

    return ESP_OK;
}

#if CONFIG_EXAMPLE_LCD_CONTROLLER_SH1107
#include "esp_lcd_sh1107.h"
#else
#include "esp_lcd_panel_vendor.h"
#endif

static const char *TAG = "example";

#define I2C_BUS_PORT  0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your LCD spec //////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define EXAMPLE_LCD_PIXEL_CLOCK_HZ    (400 * 1000)
#define EXAMPLE_PIN_NUM_SDA           21
#define EXAMPLE_PIN_NUM_SCL           22
#define EXAMPLE_PIN_NUM_RST           -1
#define EXAMPLE_I2C_HW_ADDR           0x3C

// The pixel number in horizontal and vertical
#if CONFIG_EXAMPLE_LCD_CONTROLLER_SSD1306
#define EXAMPLE_LCD_H_RES              128
#define EXAMPLE_LCD_V_RES              CONFIG_EXAMPLE_SSD1306_HEIGHT
#elif CONFIG_EXAMPLE_LCD_CONTROLLER_SH1107
#define EXAMPLE_LCD_H_RES              64
#define EXAMPLE_LCD_V_RES              128
#endif
// Bit number used to represent command and parameter
#define EXAMPLE_LCD_CMD_BITS           8
#define EXAMPLE_LCD_PARAM_BITS         8

extern void example_lvgl_demo_ui(lv_disp_t *disp ,float temp, float humids);

void app_main(void)
{
    ESP_LOGI(TAG, "Initialize I2C bus");
    i2c_master_bus_handle_t i2c_bus = NULL;
    i2c_master_bus_config_t bus_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .i2c_port = I2C_BUS_PORT,
        .sda_io_num = EXAMPLE_PIN_NUM_SDA,
        .scl_io_num = EXAMPLE_PIN_NUM_SCL,
        .flags.enable_internal_pullup = true,
    };
    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &i2c_bus));

    ESP_LOGI(TAG, "Install panel IO");
    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_io_i2c_config_t io_config = {
        .dev_addr = EXAMPLE_I2C_HW_ADDR,
        .scl_speed_hz = EXAMPLE_LCD_PIXEL_CLOCK_HZ,
        .control_phase_bytes = 1,               // According to SSD1306 datasheet
        .lcd_cmd_bits = EXAMPLE_LCD_CMD_BITS,   // According to SSD1306 datasheet
        .lcd_param_bits = EXAMPLE_LCD_CMD_BITS, // According to SSD1306 datasheet
#if CONFIG_EXAMPLE_LCD_CONTROLLER_SSD1306
        .dc_bit_offset = 6,                     // According to SSD1306 datasheet
#elif CONFIG_EXAMPLE_LCD_CONTROLLER_SH1107
        .dc_bit_offset = 0,                     // According to SH1107 datasheet
        .flags =
        {
            .disable_control_phase = 1,
        }
#endif
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c(i2c_bus, &io_config, &io_handle));

    ESP_LOGI(TAG, "Install SSD1306 panel driver");
    esp_lcd_panel_handle_t panel_handle = NULL;
    esp_lcd_panel_dev_config_t panel_config = {
        .bits_per_pixel = 1,
        .reset_gpio_num = EXAMPLE_PIN_NUM_RST,
    };
#if CONFIG_EXAMPLE_LCD_CONTROLLER_SSD1306
    esp_lcd_panel_ssd1306_config_t ssd1306_config = {
        .height = EXAMPLE_LCD_V_RES,
    };
    panel_config.vendor_config = &ssd1306_config;
    ESP_ERROR_CHECK(esp_lcd_new_panel_ssd1306(io_handle, &panel_config, &panel_handle));
#elif CONFIG_EXAMPLE_LCD_CONTROLLER_SH1107
    ESP_ERROR_CHECK(esp_lcd_new_panel_sh1107(io_handle, &panel_config, &panel_handle));
#endif

    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));

#if CONFIG_EXAMPLE_LCD_CONTROLLER_SH1107
    ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_handle, true));
#endif

    ESP_LOGI(TAG, "Initialize LVGL");
    const lvgl_port_cfg_t lvgl_cfg = ESP_LVGL_PORT_INIT_CONFIG();
    lvgl_port_init(&lvgl_cfg);

    const lvgl_port_display_cfg_t disp_cfg = {
        .io_handle = io_handle,
        .panel_handle = panel_handle,
        .buffer_size = EXAMPLE_LCD_H_RES * EXAMPLE_LCD_V_RES,
        .double_buffer = true,
        .hres = EXAMPLE_LCD_H_RES,
        .vres = EXAMPLE_LCD_V_RES,
        .monochrome = true,
        .rotation = {
            .swap_xy = false,
            .mirror_x = false,
            .mirror_y = false,
        }
    };
    lv_disp_t *disp = lvgl_port_add_disp(&disp_cfg);

    /* Rotation of the screen */
    lv_disp_set_rotation(disp, LV_DISP_ROT_NONE);

    float temperature = 0.0, humidity = 0.0;

    while (1) {
        if (dht22_read(&temperature, &humidity) == ESP_OK) {
            ESP_LOGI(TAG, "Temperature: %.1f °C, Humidity: %.1f %%", temperature, humidity);
        } else {
            ESP_LOGW(TAG, "Failed to read from DHT sensor");
        }
        vTaskDelay(pdMS_TO_TICKS(2000));
        ESP_LOGI(TAG, "Display LVGL Scroll Text");
        // Lock the mutex due to the LVGL APIs are not thread-safe
        if (lvgl_port_lock(0)) {
           example_lvgl_demo_ui(disp,temperature,humidity);
           // Release the mutex
          lvgl_port_unlock();
    }
    }

    
}
