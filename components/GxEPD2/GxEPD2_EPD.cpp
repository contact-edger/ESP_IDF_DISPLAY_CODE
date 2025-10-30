#include "GxEPD2_EPD.h"
#include "Font5x7.h"
#include <stdlib.h>

GxEPD2_EPD::GxEPD2_EPD(int16_t cs, int16_t dc, int16_t rst, int16_t busy)
  : _cs(cs), _dc(dc), _rst(rst), _busy(busy), _rotation(0), _buffer(nullptr), _buffer_size(0)
{
    _spi = nullptr;
}

void GxEPD2_EPD::init(uint32_t serial_diag_bitrate)
{
    // Configure GPIO pins
    gpio_config_t io_conf = {};
    
    // Output pins: CS, DC, RST
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << _cs) | (1ULL << _dc) | (1ULL << _rst);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);
    
    // Input pin: BUSY
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << _busy);
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);
    
    // Set initial states
    gpio_set_level((gpio_num_t)_cs, 1);
    gpio_set_level((gpio_num_t)_dc, 1);
    gpio_set_level((gpio_num_t)_rst, 1);
    
    // Configure SPI
    spi_bus_config_t buscfg = {};
    buscfg.mosi_io_num = 23;  // DIN
    buscfg.miso_io_num = -1;  // Not used
    buscfg.sclk_io_num = 18;  // CLK
    buscfg.quadwp_io_num = -1;
    buscfg.quadhd_io_num = -1;
    buscfg.max_transfer_sz = 4096;
    
    // Initialize SPI bus
    esp_err_t ret = spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK && ret != ESP_ERR_INVALID_STATE) {
        printf("Failed to initialize SPI bus\n");
        return;
    }
    
    // Configure SPI device
    spi_device_interface_config_t devcfg = {};
    devcfg.clock_speed_hz = 10 * 1000 * 1000;  // 10 MHz
    devcfg.mode = 0;  // SPI mode 0
    devcfg.spics_io_num = -1;  // We'll handle CS manually
    devcfg.queue_size = 7;
    devcfg.flags = 0;
    
    ret = spi_bus_add_device(SPI2_HOST, &devcfg, &_spi);
    if (ret != ESP_OK) {
        printf("Failed to add SPI device\n");
    }
}

void GxEPD2_EPD::_writeCommand(uint8_t c)
{
    gpio_set_level((gpio_num_t)_dc, 0);
    gpio_set_level((gpio_num_t)_cs, 0);
    
    spi_transaction_t t = {};
    t.length = 8;
    t.tx_buffer = &c;
    spi_device_polling_transmit(_spi, &t);
    
    gpio_set_level((gpio_num_t)_cs, 1);
}

void GxEPD2_EPD::_writeData(uint8_t d)
{
    gpio_set_level((gpio_num_t)_dc, 1);
    gpio_set_level((gpio_num_t)_cs, 0);
    
    spi_transaction_t t = {};
    t.length = 8;
    t.tx_buffer = &d;
    spi_device_polling_transmit(_spi, &t);
    
    gpio_set_level((gpio_num_t)_cs, 1);
}

void GxEPD2_EPD::_writeData(const uint8_t* data, uint16_t n)
{
    gpio_set_level((gpio_num_t)_dc, 1);
    gpio_set_level((gpio_num_t)_cs, 0);
    
    spi_transaction_t t = {};
    t.length = n * 8;
    t.tx_buffer = data;
    spi_device_polling_transmit(_spi, &t);
    
    gpio_set_level((gpio_num_t)_cs, 1);
}

void GxEPD2_EPD::_waitWhileBusy(const char* comment, uint16_t busy_time)
{
    if (comment)
    {
        printf("Waiting for busy: %s (BUSY=%d)\n", comment, gpio_get_level((gpio_num_t)_busy));
    }
    
    uint32_t start = xTaskGetTickCount() * portTICK_PERIOD_MS;
    // Wait while BUSY is HIGH (display is busy)
    while (gpio_get_level((gpio_num_t)_busy) == 1)
    {
        vTaskDelay(10 / portTICK_PERIOD_MS);
        uint32_t elapsed = (xTaskGetTickCount() * portTICK_PERIOD_MS) - start;
        if (elapsed > busy_time)
        {
            if (comment)
            {
                printf("Timeout waiting for busy: %s (BUSY still at %d)\n", comment, gpio_get_level((gpio_num_t)_busy));
            }
            break;
        }
    }
    
    if (comment)
    {
        printf("Done waiting: %s (BUSY=%d)\n", comment, gpio_get_level((gpio_num_t)_busy));
    }
}

void GxEPD2_EPD::_reset()
{
    gpio_set_level((gpio_num_t)_rst, 0);
    vTaskDelay(20 / portTICK_PERIOD_MS);
    gpio_set_level((gpio_num_t)_rst, 1);
    vTaskDelay(200 / portTICK_PERIOD_MS);
}

void GxEPD2_EPD::fillScreen(uint16_t color)
{
    if (_buffer)
    {
        memset(_buffer, color, _buffer_size);
    }
}

void GxEPD2_EPD::setFullWindow()
{
    // Base implementation - override in specific display class
}

void GxEPD2_EPD::writeScreenBuffer()
{
    // Base implementation - override in specific display class
}

void GxEPD2_EPD::refresh(bool partial_update_mode)
{
    // Base implementation - override in specific display class
}

void GxEPD2_EPD::drawPixel(int16_t x, int16_t y, uint16_t color)
{
    if (!_buffer)
    {
        printf("ERROR: drawPixel called but buffer is NULL!\n");
        return;
    }
    
    // Store original coordinates for debugging
    int16_t orig_x = x;
    int16_t orig_y = y;
    
    // Apply rotation FIRST, then check bounds
    int16_t t;
    switch (_rotation)
    {
        case 1:
            t = x;
            x = _width - 1 - y;
            y = t;
            break;
        case 2:
            x = _width - 1 - x;
            y = _height - 1 - y;
            break;
        case 3:
            t = x;
            x = y;
            y = _height - 1 - t;
            break;
    }
    
    // Now check bounds after rotation
    if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height))
    {
        printf("Pixel out of bounds: orig(%d,%d) -> rotated(%d,%d), limits(%d,%d)\n", 
               orig_x, orig_y, x, y, _width, _height);
        return;
    }
    
    uint32_t i = x / 8 + y * ((_width + 7) / 8);
    
    if (i >= _buffer_size)
    {
        printf("Buffer index out of range: %lu >= %lu\n", (unsigned long)i, (unsigned long)_buffer_size);
        return;
    }
    
    if (color == GxEPD_BLACK)
    {
        _buffer[i] &= ~(0x80 >> (x & 7));
    }
    else
    {
        _buffer[i] |= (0x80 >> (x & 7));
    }
}

void GxEPD2_EPD::hibernate()
{
    // Base implementation - override in specific display class
}

void GxEPD2_EPD::setRotation(uint8_t r)
{
    _rotation = r % 4;
}

int16_t GxEPD2_EPD::width()
{
    return (_rotation % 2 == 0) ? _width : _height;
}

int16_t GxEPD2_EPD::height()
{
    return (_rotation % 2 == 0) ? _height : _width;
}

void GxEPD2_EPD::_PowerOn()
{
    // Base implementation - override in specific display class
}

void GxEPD2_EPD::_PowerOff()
{
    // Base implementation - override in specific display class
}

void GxEPD2_EPD::_InitDisplay()
{
    // Base implementation - override in specific display class
}

void GxEPD2_EPD::_setPartialRamArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    // Base implementation - override in specific display class
}

void GxEPD2_EPD::drawChar(int16_t x, int16_t y, char c, uint16_t color, uint8_t size)
{
    if (c < 32 || c > 126) return; // Only printable ASCII
    
    const uint8_t* glyph = Font5x7 + (c - 32) * 5;
    
    for (uint8_t col = 0; col < 5; col++)
    {
        uint8_t line = glyph[col];
        for (uint8_t row = 0; row < 7; row++)
        {
            if (line & (1 << row))
            {
                if (size == 1)
                {
                    drawPixel(x + col, y + row, color);
                }
                else
                {
                    // Draw scaled pixel
                    for (uint8_t sx = 0; sx < size; sx++)
                    {
                        for (uint8_t sy = 0; sy < size; sy++)
                        {
                            drawPixel(x + col * size + sx, y + row * size + sy, color);
                        }
                    }
                }
            }
        }
    }
}

void GxEPD2_EPD::drawString(int16_t x, int16_t y, const char* text, uint16_t color, uint8_t size)
{
    int16_t cursor_x = x;
    int16_t cursor_y = y;
    
    while (*text)
    {
        if (*text == '\n')
        {
            cursor_x = x;
            cursor_y += 8 * size;
        }
        else
        {
            drawChar(cursor_x, cursor_y, *text, color, size);
            cursor_x += 6 * size; // 5 pixels + 1 space
        }
        text++;
    }
}
