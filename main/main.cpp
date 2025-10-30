#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include <GxEPD2_BW.h>

static const char *TAG = "EPD_DEMO";

// Pin definitions for Waveshare 2.9" e-paper V2
// Connections: BUSY-D4, RST-D15, DC-D19, CS-D5, CLK-D18, DIN-D23, GND-GND, VCC-3V3
#define EPD_CS      5   // D5
#define EPD_DC      19  // D19
#define EPD_RST     15  // D15
#define EPD_BUSY    4   // D4
#define EPD_CLK     18  // D18
#define EPD_DIN     23  // D23 (MOSI)

// Create display instance for Waveshare 2.9" e-paper V2 (128x296)
GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> display(GxEPD2_290_T94_V2(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));

void test_hardware_connections()
{
    ESP_LOGI(TAG, "\n>>> HARDWARE CONNECTION TEST <<<");
    ESP_LOGI(TAG, "Testing BUSY pin (should read display status)...");
    
    int busy_state = gpio_get_level((gpio_num_t)EPD_BUSY);
    ESP_LOGI(TAG, "BUSY pin state: %d (0=ready, 1=busy)", busy_state);
    
    if (busy_state == 1) {
        ESP_LOGW(TAG, "WARNING: BUSY pin is HIGH! Display may be stuck or not connected properly.");
    } else {
        ESP_LOGI(TAG, "BUSY pin OK - display is ready");
    }
    
    ESP_LOGI(TAG, "Pin connections to verify:");
    ESP_LOGI(TAG, "  BUSY -> GPIO4 (D4)");
    ESP_LOGI(TAG, "  RST  -> GPIO15 (D15)");
    ESP_LOGI(TAG, "  DC   -> GPIO19 (D19)");
    ESP_LOGI(TAG, "  CS   -> GPIO5 (D5)");
    ESP_LOGI(TAG, "  CLK  -> GPIO18 (D18)");
    ESP_LOGI(TAG, "  DIN  -> GPIO23 (D23)");
    ESP_LOGI(TAG, "  GND  -> GND");
    ESP_LOGI(TAG, "  VCC  -> 3.3V");
    ESP_LOGI(TAG, "");
}

extern "C" void app_main()
{
    ESP_LOGI(TAG, "\n\n=================================");
    ESP_LOGI(TAG, "Waveshare 2.9\" e-Paper Display");
    ESP_LOGI(TAG, "EdgeR Technologies Demo");
    ESP_LOGI(TAG, "=================================\n");
    
    // Hardware test
    ESP_LOGI(TAG, "Testing hardware...");
    test_hardware_connections();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    
    // Initialize display
    ESP_LOGI(TAG, "\nInitializing display...");
    display.init(115200);
    display.setRotation(1); // Rotate 90 degrees for horizontal/landscape mode
    ESP_LOGI(TAG, "Display initialized (size=%dx%d)", display.width(), display.height());
    
    // Display "EdgeR Technologies"
    ESP_LOGI(TAG, "\nDisplaying EdgeR Technologies...");
    display.setFullWindow();
    display.firstPage();
    do
    {
        // Clear screen (white background)
        display.fillScreen(GxEPD_WHITE);
        
        // Display "EdgeR Technologies" centered
        display.drawString(30, 50, "NiswiQ", GxEPD_BLACK, 3);
    }
    while (display.nextPage());
    
    ESP_LOGI(TAG, "\n=================================");
    ESP_LOGI(TAG, "Display updated!");
    ESP_LOGI(TAG, "Showing: EdgeR Technologies");
    ESP_LOGI(TAG, "=================================\n");
    
    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}