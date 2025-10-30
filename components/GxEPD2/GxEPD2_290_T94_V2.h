#ifndef _GxEPD2_290_T94_V2_H_
#define _GxEPD2_290_T94_V2_H_

#include "GxEPD2_EPD.h"

// Waveshare 2.9" e-Paper Display V2 (296x128)
class GxEPD2_290_T94_V2 : public GxEPD2_EPD
{
  public:
    static const uint16_t WIDTH = 128;
    static const uint16_t HEIGHT = 296;
    
    GxEPD2_290_T94_V2(int16_t cs, int16_t dc, int16_t rst, int16_t busy);
    
    void init(uint32_t serial_diag_bitrate = 0) override;
    void setFullWindow() override;
    void writeScreenBuffer() override;
    void refresh(bool partial_update_mode = false) override;
    void hibernate() override;
    
  private:
    void _InitDisplay();
    void _PowerOn();
    void _PowerOff();
    void _setPartialRamArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    
    static const unsigned char lut_full_update[];
    static const unsigned char lut_partial_update[];
};

#endif // _GxEPD2_290_T94_V2_H_
