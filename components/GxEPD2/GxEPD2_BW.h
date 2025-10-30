#ifndef _GxEPD2_BW_H_
#define _GxEPD2_BW_H_

#include <stdint.h>
#include "GxEPD2_EPD.h"

template<typename GxEPD2_Type, const uint16_t page_height>
class GxEPD2_BW : public GxEPD2_Type
{
  public:
    GxEPD2_BW(GxEPD2_Type epd) : GxEPD2_Type(epd) {};
    
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void init(uint32_t serial_diag_bitrate = 0);
    void fillScreen(uint16_t color);
    void setFullWindow();
    void firstPage();
    bool nextPage();
    void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
    void setCursor(int16_t x, int16_t y);
    void setTextColor(uint16_t c);
    void setTextColor(uint16_t c, uint16_t bg);
    void setTextSize(uint8_t s);
    void setFont(const void* f);
    void setRotation(uint8_t r);
    size_t write(uint8_t);
    void println(const char* str);
    void print(const char* str);
    void hibernate();
    
    int16_t width();
    int16_t height();
    
  private:
    int16_t _cursor_x = 0;
    int16_t _cursor_y = 0;
    uint16_t _textcolor = 0;
    uint16_t _textbgcolor = 0xFFFF;
    uint8_t _textsize = 1;
    const void* _font = nullptr;
    uint8_t _rotation = 0;
    bool _first_page = true;
};

#include "GxEPD2_290_T94_V2.h"

// Template implementation
template<typename GxEPD2_Type, const uint16_t page_height>
void GxEPD2_BW<GxEPD2_Type, page_height>::init(uint32_t serial_diag_bitrate)
{
    GxEPD2_Type::init(serial_diag_bitrate);
}

template<typename GxEPD2_Type, const uint16_t page_height>
void GxEPD2_BW<GxEPD2_Type, page_height>::fillScreen(uint16_t color)
{
    GxEPD2_Type::fillScreen(color);
}

template<typename GxEPD2_Type, const uint16_t page_height>
void GxEPD2_BW<GxEPD2_Type, page_height>::setFullWindow()
{
    GxEPD2_Type::setFullWindow();
}

template<typename GxEPD2_Type, const uint16_t page_height>
void GxEPD2_BW<GxEPD2_Type, page_height>::firstPage()
{
    _first_page = true;
    // Don't write buffer yet - wait for drawing to happen first
}

template<typename GxEPD2_Type, const uint16_t page_height>
bool GxEPD2_BW<GxEPD2_Type, page_height>::nextPage()
{
    if (_first_page)
    {
        _first_page = false;
        // NOW write the buffer after drawing is complete
        GxEPD2_Type::writeScreenBuffer();
        GxEPD2_Type::refresh(false);
        return false;
    }
    return false;
}

template<typename GxEPD2_Type, const uint16_t page_height>
void GxEPD2_BW<GxEPD2_Type, page_height>::drawPixel(int16_t x, int16_t y, uint16_t color)
{
    GxEPD2_Type::drawPixel(x, y, color);
}

template<typename GxEPD2_Type, const uint16_t page_height>
void GxEPD2_BW<GxEPD2_Type, page_height>::setCursor(int16_t x, int16_t y)
{
    _cursor_x = x;
    _cursor_y = y;
}

template<typename GxEPD2_Type, const uint16_t page_height>
void GxEPD2_BW<GxEPD2_Type, page_height>::setTextColor(uint16_t c)
{
    _textcolor = c;
}

template<typename GxEPD2_Type, const uint16_t page_height>
void GxEPD2_BW<GxEPD2_Type, page_height>::setTextColor(uint16_t c, uint16_t bg)
{
    _textcolor = c;
    _textbgcolor = bg;
}

template<typename GxEPD2_Type, const uint16_t page_height>
void GxEPD2_BW<GxEPD2_Type, page_height>::setTextSize(uint8_t s)
{
    _textsize = (s > 0) ? s : 1;
}

template<typename GxEPD2_Type, const uint16_t page_height>
void GxEPD2_BW<GxEPD2_Type, page_height>::setFont(const void* f)
{
    _font = f;
}

template<typename GxEPD2_Type, const uint16_t page_height>
void GxEPD2_BW<GxEPD2_Type, page_height>::setRotation(uint8_t r)
{
    _rotation = r % 4;
    GxEPD2_Type::setRotation(_rotation);
}

template<typename GxEPD2_Type, const uint16_t page_height>
size_t GxEPD2_BW<GxEPD2_Type, page_height>::write(uint8_t c)
{
    if (c == '\n')
    {
        _cursor_y += _textsize * 8;
        _cursor_x = 0;
    }
    else if (c == '\r')
    {
        // skip
    }
    else
    {
        drawChar(_cursor_x, _cursor_y, c, _textcolor, _textbgcolor, _textsize);
        _cursor_x += _textsize * 6;
    }
    return 1;
}

template<typename GxEPD2_Type, const uint16_t page_height>
void GxEPD2_BW<GxEPD2_Type, page_height>::println(const char* str)
{
    print(str);
    write('\n');
}

template<typename GxEPD2_Type, const uint16_t page_height>
void GxEPD2_BW<GxEPD2_Type, page_height>::print(const char* str)
{
    while (*str)
    {
        write(*str++);
    }
}

template<typename GxEPD2_Type, const uint16_t page_height>
void GxEPD2_BW<GxEPD2_Type, page_height>::drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size)
{
    // Simple 5x7 font
    for (int8_t i = 0; i < 6; i++)
    {
        uint8_t line = (i == 5) ? 0x0 : 0x00; // Simple blank character
        for (int8_t j = 0; j < 8; j++)
        {
            if (line & 0x1)
            {
                if (size == 1)
                    drawPixel(x + i, y + j, color);
                else
                {
                    for (int8_t a = 0; a < size; a++)
                        for (int8_t b = 0; b < size; b++)
                            drawPixel(x + (i * size) + a, y + (j * size) + b, color);
                }
            }
            else if (bg != color)
            {
                if (size == 1)
                    drawPixel(x + i, y + j, bg);
                else
                {
                    for (int8_t a = 0; a < size; a++)
                        for (int8_t b = 0; b < size; b++)
                            drawPixel(x + (i * size) + a, y + (j * size) + b, bg);
                }
            }
            line >>= 1;
        }
    }
}

template<typename GxEPD2_Type, const uint16_t page_height>
void GxEPD2_BW<GxEPD2_Type, page_height>::hibernate()
{
    GxEPD2_Type::hibernate();
}

template<typename GxEPD2_Type, const uint16_t page_height>
int16_t GxEPD2_BW<GxEPD2_Type, page_height>::width()
{
    return GxEPD2_Type::width();
}

template<typename GxEPD2_Type, const uint16_t page_height>
int16_t GxEPD2_BW<GxEPD2_Type, page_height>::height()
{
    return GxEPD2_Type::height();
}

#endif // _GxEPD2_BW_H_
