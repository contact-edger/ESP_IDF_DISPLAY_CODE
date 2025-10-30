#include "GxEPD2_290_T94_V2.h"
#include <stdlib.h>

// LUT for full update
const unsigned char GxEPD2_290_T94_V2::lut_full_update[] =
{
    0x80, 0x60, 0x40, 0x00, 0x00, 0x00, 0x00,
    0x10, 0x60, 0x20, 0x00, 0x00, 0x00, 0x00,
    0x80, 0x60, 0x40, 0x00, 0x00, 0x00, 0x00,
    0x10, 0x60, 0x20, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x03, 0x03, 0x00, 0x00, 0x02,
    0x09, 0x09, 0x00, 0x00, 0x02,
    0x03, 0x03, 0x00, 0x00, 0x02,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
};

// LUT for partial update
const unsigned char GxEPD2_290_T94_V2::lut_partial_update[] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0A, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
};

GxEPD2_290_T94_V2::GxEPD2_290_T94_V2(int16_t cs, int16_t dc, int16_t rst, int16_t busy)
  : GxEPD2_EPD(cs, dc, rst, busy)
{
    _width = WIDTH;
    _height = HEIGHT;
    _buffer_size = WIDTH * HEIGHT / 8;
    _buffer = (uint8_t*)malloc(_buffer_size);
    if (_buffer)
    {
        memset(_buffer, 0xFF, _buffer_size);
    }
}

void GxEPD2_290_T94_V2::init(uint32_t serial_diag_bitrate)
{
    GxEPD2_EPD::init(serial_diag_bitrate);
    _InitDisplay();
}

void GxEPD2_290_T94_V2::_InitDisplay()
{
    printf("Starting display initialization...\n");
    printf("Display: Waveshare 2.9\" Rev 2.1 (296x128)\n");
    
    _reset();
    vTaskDelay(10 / portTICK_PERIOD_MS);
    
    printf("Sending SWRESET command...\n");
    _writeCommand(0x12); // SWRESET
    vTaskDelay(20 / portTICK_PERIOD_MS);
    _waitWhileBusy("SWRESET", 300);
    
    printf("Configuring display settings for Rev 2.1...\n");
    
    // Driver output control - CRITICAL FOR REV 2.1
    _writeCommand(0x01); 
    _writeData(0x27); // HEIGHT - 1 = 295 (0x127)
    _writeData(0x01); // HEIGHT MSB
    _writeData(0x00); // GD=0, SM=0, TB=0
    
    // Data entry mode - X increment, Y increment
    _writeCommand(0x11);
    _writeData(0x03);
    
    // Set RAM X address start/end
    _writeCommand(0x44);
    _writeData(0x00); // X start at 0
    _writeData(0x0F); // X end at 15 (128 pixels / 8 = 16 bytes, 0-15)
    
    // Set RAM Y address start/end  
    _writeCommand(0x45);
    _writeData(0x00); // Y start LOW
    _writeData(0x00); // Y start HIGH
    _writeData(0x27); // Y end LOW (295)
    _writeData(0x01); // Y end HIGH
    
    // Border waveform control
    _writeCommand(0x3C);
    _writeData(0x05);
    
    // Read built-in temperature sensor
    _writeCommand(0x18);
    _writeData(0x80);
    
    // Display update control 1
    _writeCommand(0x21);
    _writeData(0x00);
    _writeData(0x80);
    
    // Set RAM X address counter to 0
    _writeCommand(0x4E);
    _writeData(0x00);
    
    // Set RAM Y address counter to 0
    _writeCommand(0x4F);
    _writeData(0x00);
    _writeData(0x00);
    
    _waitWhileBusy("_Init_Full", 300);
    
    printf("Display initialization complete\n");
}

void GxEPD2_290_T94_V2::setFullWindow()
{
    _setPartialRamArea(0, 0, WIDTH, HEIGHT);
}

void GxEPD2_290_T94_V2::_setPartialRamArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    _writeCommand(0x44); // Set RAM X address start/end
    _writeData(x / 8);
    _writeData((x + w - 1) / 8);
    
    _writeCommand(0x45); // Set RAM Y address start/end
    _writeData(y % 256);
    _writeData(y / 256);
    _writeData((y + h - 1) % 256);
    _writeData((y + h - 1) / 256);
    
    _writeCommand(0x4E); // Set RAM X address counter
    _writeData(x / 8);
    
    _writeCommand(0x4F); // Set RAM Y address counter
    _writeData(y % 256);
    _writeData(y / 256);
}

void GxEPD2_290_T94_V2::writeScreenBuffer()
{
    printf("Writing screen buffer to display RAM...\n");
    
    if (!_buffer)
    {
        printf("ERROR: Buffer is NULL!\n");
        return;
    }
    
    // Check buffer content
    uint32_t black_pixels = 0;
    uint32_t white_pixels = 0;
    for (uint32_t i = 0; i < _buffer_size; i++)
    {
        uint8_t byte = _buffer[i];
        for (int bit = 0; bit < 8; bit++)
        {
            if (byte & (0x80 >> bit))
                white_pixels++;
            else
                black_pixels++;
        }
    }
    printf("Buffer stats: %lu black pixels, %lu white pixels\n", 
           (unsigned long)black_pixels, (unsigned long)white_pixels);
    
    setFullWindow();
    
    // Write to 0x24 (Black/White RAM) WITHOUT inversion
    // This display: buffer 0x00=black, 0xFF=white → RAM 0x00=black, 0xFF=white
    _writeCommand(0x24);
    for (uint32_t i = 0; i < _buffer_size; i++)
    {
        _writeData(_buffer[i]); // Direct write: no inversion needed
    }
    printf("Wrote %lu bytes to BLACK/WHITE RAM (0x24)\n", (unsigned long)_buffer_size);
}

void GxEPD2_290_T94_V2::refresh(bool partial_update_mode)
{
    printf("Starting display refresh (partial=%d)...\n", partial_update_mode);
    
    // Display Update Control 2 (0x22) - Standard SSD1680 sequence
    _writeCommand(0x22);
    _writeData(0xF7); // Full update: Load temperature, Load LUT, Display, Disable clock
    
    // Master Activation (0x20) - Trigger the update
    _writeCommand(0x20);
    
    vTaskDelay(10 / portTICK_PERIOD_MS);
    _waitWhileBusy("refresh", 10000); // Longer timeout for full refresh
    
    printf("Display refresh complete\n");
}

void GxEPD2_290_T94_V2::_PowerOn()
{
    _writeCommand(0x22);
    _writeData(0xF8);
    _writeCommand(0x20);
    _waitWhileBusy("_PowerOn", 100);
}

void GxEPD2_290_T94_V2::_PowerOff()
{
    printf("Powering off display...\n");
    _writeCommand(0x22);
    _writeData(0x83);
    _writeCommand(0x20);
    _waitWhileBusy("_PowerOff", 1000);
    printf("Display powered off\n");
}

void GxEPD2_290_T94_V2::hibernate()
{
    _PowerOff();
    _writeCommand(0x10); // Deep sleep mode
    _writeData(0x01);
    vTaskDelay(100 / portTICK_PERIOD_MS);
}
