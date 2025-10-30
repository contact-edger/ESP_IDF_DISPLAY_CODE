#ifndef _GxEPD2_EPD_H_
#define _GxEPD2_EPD_H_

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"

#define GxEPD_WHITE 0xFF
#define GxEPD_BLACK 0x00

class GxEPD2_EPD
{
  public:
    GxEPD2_EPD(int16_t cs, int16_t dc, int16_t rst, int16_t busy);
    
    virtual void init(uint32_t serial_diag_bitrate = 0);
    virtual void fillScreen(uint16_t color);
    virtual void setFullWindow();
    virtual void writeScreenBuffer();
    virtual void refresh(bool partial_update_mode = false);
    virtual void drawPixel(int16_t x, int16_t y, uint16_t color);
    virtual void drawChar(int16_t x, int16_t y, char c, uint16_t color, uint8_t size = 1);
    virtual void drawString(int16_t x, int16_t y, const char* text, uint16_t color, uint8_t size = 1);
    virtual void hibernate();
    virtual void setRotation(uint8_t r);
    virtual int16_t width();
    virtual int16_t height();
    
  protected:
    void _writeCommand(uint8_t c);
    void _writeData(uint8_t d);
    void _writeData(const uint8_t* data, uint16_t n);
    void _waitWhileBusy(const char* comment = 0, uint16_t busy_time = 5000);
    void _reset();
    void _PowerOn();
    void _PowerOff();
    void _InitDisplay();
    void _setPartialRamArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    
    int16_t _cs, _dc, _rst, _busy;
    uint8_t _rotation;
    int16_t _width, _height;
    uint8_t* _buffer;
    uint32_t _buffer_size;
    spi_device_handle_t _spi;
};

#endif // _GxEPD2_EPD_H_
