#ifndef _FreeMonoBold12pt7b_H_
#define _FreeMonoBold12pt7b_H_

// This is a placeholder font structure
// For a full implementation, you would need the complete font data

typedef struct {
    uint16_t bitmapOffset;
    uint8_t width;
    uint8_t height;
    uint8_t xAdvance;
    int8_t xOffset;
    int8_t yOffset;
} GFXglyph;

typedef struct {
    uint8_t *bitmap;
    GFXglyph *glyph;
    uint8_t first;
    uint8_t last;
    uint8_t yAdvance;
} GFXfont;

// Simplified font - just a placeholder
const uint8_t FreeMonoBold12pt7bBitmaps[] = {0x00};
const GFXglyph FreeMonoBold12pt7bGlyphs[] = {{0, 0, 0, 12, 0, 0}};

const GFXfont FreeMonoBold12pt7b = {
    (uint8_t *)FreeMonoBold12pt7bBitmaps,
    (GFXglyph *)FreeMonoBold12pt7bGlyphs,
    0x20, 0x7E, 24
};

#endif // _FreeMonoBold12pt7b_H_
