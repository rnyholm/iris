/* mbed GraphicsDisplay Display Library Base Class
 * Copyright (c) 2007-2009 sford
 * Released under the MIT License: http://mbed.org/license/mit
 *
 * Derivative work by D.Smart 2014
 */

#include "GraphicsDisplay.h"
#include "Bitmap.h"
#include "string.h"

//#define DEBUG "GD"
// ...
// INFO("Stuff to show %d", var); // new-line is automatically appended
//
#if (defined(DEBUG) && !defined(TARGET_LPC11U24))
#define INFO(x, ...) std::printf("[INF %s %3d] "x"\r\n", DEBUG, __LINE__, ##__VA_ARGS__);
#define WARN(x, ...) std::printf("[WRN %s %3d] "x"\r\n", DEBUG, __LINE__, ##__VA_ARGS__);
#define ERR(x, ...)  std::printf("[ERR %s %3d] "x"\r\n", DEBUG, __LINE__, ##__VA_ARGS__);
static void HexDump(char * title, uint8_t * p, int count)
{
    int i;
    char buf[100] = "0000: ";
    
    if (*title)
        INFO("%s", title);
    for (i=0; i<count; ) {
        sprintf(buf + strlen(buf), "%02X ", *(p+i));
        if ((++i & 0x0F) == 0x00) {
            INFO("%s", buf);
            if (i < count)
                sprintf(buf, "%04X: ", i);
            else
                buf[0] = '\0';
        }
    }
    if (strlen(buf))
        INFO("%s", buf);
}
#else
#define INFO(x, ...)
#define WARN(x, ...)
#define ERR(x, ...)
#define HexDump(a, b, c)
#endif

#ifdef LOCALFONT
const unsigned char FONT8x8[97][8] = {
    0x08, 0x08, 0x08, 0X00, 0X00, 0X00, 0X00, 0X00, // columns, rows, num_bytes_per_char
    0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, // space 0x20
    0x30, 0x78, 0x78, 0x30, 0x30, 0X00, 0x30, 0X00, // !
    0x6C, 0x6C, 0x6C, 0X00, 0X00, 0X00, 0X00, 0X00, // "
    0x6C, 0x6C, 0xFE, 0x6C, 0xFE, 0x6C, 0x6C, 0X00, // #
    0x18, 0x3E, 0x60, 0x3C, 0x06, 0x7C, 0x18, 0X00, // $
    0X00, 0x63, 0x66, 0x0C, 0x18, 0x33, 0x63, 0X00, // %
    0x1C, 0x36, 0x1C, 0x3B, 0x6E, 0x66, 0x3B, 0X00, // &
    0x30, 0x30, 0x60, 0X00, 0X00, 0X00, 0X00, 0X00, // '
    0x0C, 0x18, 0x30, 0x30, 0x30, 0x18, 0x0C, 0X00, // (
    0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x18, 0x30, 0X00, // )
    0X00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0X00, 0X00, // *
    0X00, 0x30, 0x30, 0xFC, 0x30, 0x30, 0X00, 0X00, // +
    0X00, 0X00, 0X00, 0X00, 0X00, 0x18, 0x18, 0x30, // ,
    0X00, 0X00, 0X00, 0x7E, 0X00, 0X00, 0X00, 0X00, // -
    0X00, 0X00, 0X00, 0X00, 0X00, 0x18, 0x18, 0X00, // .
    0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x40, 0X00, // / (forward slash)
    0x3E, 0x63, 0x63, 0x6B, 0x63, 0x63, 0x3E, 0X00, // 0 0x30
    0x18, 0x38, 0x58, 0x18, 0x18, 0x18, 0x7E, 0X00, // 1
    0x3C, 0x66, 0x06, 0x1C, 0x30, 0x66, 0x7E, 0X00, // 2
    0x3C, 0x66, 0x06, 0x1C, 0x06, 0x66, 0x3C, 0X00, // 3
    0x0E, 0x1E, 0x36, 0x66, 0x7F, 0x06, 0x0F, 0X00, // 4
    0x7E, 0x60, 0x7C, 0x06, 0x06, 0x66, 0x3C, 0X00, // 5
    0x1C, 0x30, 0x60, 0x7C, 0x66, 0x66, 0x3C, 0X00, // 6
    0x7E, 0x66, 0x06, 0x0C, 0x18, 0x18, 0x18, 0X00, // 7
    0x3C, 0x66, 0x66, 0x3C, 0x66, 0x66, 0x3C, 0X00, // 8
    0x3C, 0x66, 0x66, 0x3E, 0x06, 0x0C, 0x38, 0X00, // 9
    0X00, 0x18, 0x18, 0X00, 0X00, 0x18, 0x18, 0X00, // :
    0X00, 0x18, 0x18, 0X00, 0X00, 0x18, 0x18, 0x30, // ;
    0x0C, 0x18, 0x30, 0x60, 0x30, 0x18, 0x0C, 0X00, // <
    0X00, 0X00, 0x7E, 0X00, 0X00, 0x7E, 0X00, 0X00, // =
    0x30, 0x18, 0x0C, 0x06, 0x0C, 0x18, 0x30, 0X00, // >
    0x3C, 0x66, 0x06, 0x0C, 0x18, 0X00, 0x18, 0X00, // ?
    0x3E, 0x63, 0x6F, 0x69, 0x6F, 0x60, 0x3E, 0X00, // @ 0x40
    0x18, 0x3C, 0x66, 0x66, 0x7E, 0x66, 0x66, 0X00, // A
    0x7E, 0x33, 0x33, 0x3E, 0x33, 0x33, 0x7E, 0X00, // B
    0x1E, 0x33, 0x60, 0x60, 0x60, 0x33, 0x1E, 0X00, // C
    0x7C, 0x36, 0x33, 0x33, 0x33, 0x36, 0x7C, 0X00, // D
    0x7F, 0x31, 0x34, 0x3C, 0x34, 0x31, 0x7F, 0X00, // E
    0x7F, 0x31, 0x34, 0x3C, 0x34, 0x30, 0x78, 0X00, // F
    0x1E, 0x33, 0x60, 0x60, 0x67, 0x33, 0x1F, 0X00, // G
    0x66, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66, 0X00, // H
    0x3C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0X00, // I
    0x0F, 0x06, 0x06, 0x06, 0x66, 0x66, 0x3C, 0X00, // J
    0x73, 0x33, 0x36, 0x3C, 0x36, 0x33, 0x73, 0X00, // K
    0x78, 0x30, 0x30, 0x30, 0x31, 0x33, 0x7F, 0X00, // L
    0x63, 0x77, 0x7F, 0x7F, 0x6B, 0x63, 0x63, 0X00, // M
    0x63, 0x73, 0x7B, 0x6F, 0x67, 0x63, 0x63, 0X00, // N
    0x3E, 0x63, 0x63, 0x63, 0x63, 0x63, 0x3E, 0X00, // O
    0x7E, 0x33, 0x33, 0x3E, 0x30, 0x30, 0x78, 0X00, // P 0x50
    0x3C, 0x66, 0x66, 0x66, 0x6E, 0x3C, 0x0E, 0X00, // Q
    0x7E, 0x33, 0x33, 0x3E, 0x36, 0x33, 0x73, 0X00, // R
    0x3C, 0x66, 0x30, 0x18, 0x0C, 0x66, 0x3C, 0X00, // S
    0x7E, 0x5A, 0x18, 0x18, 0x18, 0x18, 0x3C, 0X00, // T
    0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x7E, 0X00, // U
    0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x18, 0X00, // V
    0x63, 0x63, 0x63, 0x6B, 0x7F, 0x77, 0x63, 0X00, // W
    0x63, 0x63, 0x36, 0x1C, 0x1C, 0x36, 0x63, 0X00, // X
    0x66, 0x66, 0x66, 0x3C, 0x18, 0x18, 0x3C, 0X00, // Y
    0x7F, 0x63, 0x46, 0x0C, 0x19, 0x33, 0x7F, 0X00, // Z
    0x3C, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3C, 0X00, // [
    0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x01, 0X00, // \ (back slash)
    0x3C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x3C, 0X00, // ]
    0x08, 0x1C, 0x36, 0x63, 0X00, 0X00, 0X00, 0X00, // ^
    0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0xFF, // _
    0x18, 0x18, 0x0C, 0X00, 0X00, 0X00, 0X00, 0X00, // ` 0x60
    0X00, 0X00, 0x3C, 0x06, 0x3E, 0x66, 0x3B, 0X00, // a
    0x70, 0x30, 0x3E, 0x33, 0x33, 0x33, 0x6E, 0X00, // b
    0X00, 0X00, 0x3C, 0x66, 0x60, 0x66, 0x3C, 0X00, // c
    0x0E, 0x06, 0x3E, 0x66, 0x66, 0x66, 0x3B, 0X00, // d
    0X00, 0X00, 0x3C, 0x66, 0x7E, 0x60, 0x3C, 0X00, // e
    0x1C, 0x36, 0x30, 0x78, 0x30, 0x30, 0x78, 0X00, // f
    0X00, 0X00, 0x3B, 0x66, 0x66, 0x3E, 0x06, 0x7C, // g
    0x70, 0x30, 0x36, 0x3B, 0x33, 0x33, 0x73, 0X00, // h
    0x18, 0X00, 0x38, 0x18, 0x18, 0x18, 0x3C, 0X00, // i
    0x06, 0X00, 0x06, 0x06, 0x06, 0x66, 0x66, 0x3C, // j
    0x70, 0x30, 0x33, 0x36, 0x3C, 0x36, 0x73, 0X00, // k
    0x38, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0X00, // l
    0X00, 0X00, 0x66, 0x7F, 0x7F, 0x6B, 0x63, 0X00, // m
    0X00, 0X00, 0x7C, 0x66, 0x66, 0x66, 0x66, 0X00, // n
    0X00, 0X00, 0x3C, 0x66, 0x66, 0x66, 0x3C, 0X00, // o
    0X00, 0X00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x78, // p
    0X00, 0X00, 0x3B, 0x66, 0x66, 0x3E, 0x06, 0x0F, // q
    0X00, 0X00, 0x6E, 0x3B, 0x33, 0x30, 0x78, 0X00, // r
    0X00, 0X00, 0x3E, 0x60, 0x3C, 0x06, 0x7C, 0X00, // s
    0x08, 0x18, 0x3E, 0x18, 0x18, 0x1A, 0x0C, 0X00, // t
    0X00, 0X00, 0x66, 0x66, 0x66, 0x66, 0x3B, 0X00, // u
    0X00, 0X00, 0x66, 0x66, 0x66, 0x3C, 0x18, 0X00, // v
    0X00, 0X00, 0x63, 0x6B, 0x7F, 0x7F, 0x36, 0X00, // w
    0X00, 0X00, 0x63, 0x36, 0x1C, 0x36, 0x63, 0X00, // x
    0X00, 0X00, 0x66, 0x66, 0x66, 0x3E, 0x06, 0x7C, // y
    0X00, 0X00, 0x7E, 0x4C, 0x18, 0x32, 0x7E, 0X00, // z
    0x0E, 0x18, 0x18, 0x70, 0x18, 0x18, 0x0E, 0X00, // {
    0x0C, 0x0C, 0x0C, 0X00, 0x0C, 0x0C, 0x0C, 0X00, // |
    0x70, 0x18, 0x18, 0x0E, 0x18, 0x18, 0x70, 0X00, // }
    0x3B, 0x6E, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, // ~
    0x1C, 0x36, 0x36, 0x1C, 0X00, 0X00, 0X00, 0X00  // DEL
}; 
#endif // LOCALFONT

char mytolower(char a) {
    if (a >= 'A' && a <= 'Z')
        return (a - 'A' + 'a');
    else
        return a;
}
/// mystrnicmp exists because not all compiler libraries have this function.
///
/// Some have strnicmp, others _strnicmp, and others have C++ methods, which
/// is outside the scope of this C-portable set of functions.
///
/// @param l is a pointer to the string on the left
/// @param r is a pointer to the string on the right
/// @param n is the number of characters to compare
/// @returns -1 if l < r
/// @returns 0 if l == r
/// @returns +1 if l > r
///
int mystrnicmp(const char *l, const char *r, size_t n) {
    int result = 0;

    if (n != 0) {
        do {
            result = mytolower(*l++) - mytolower(*r++);
        } while ((result == 0) && (*l != '\0') && (--n > 0));
    }
    if (result < -1)
        result = -1;
    else if (result > 1)
        result = 1;
    return result;
}


GraphicsDisplay::GraphicsDisplay(const char *name) 
    : TextDisplay(name)
{
    font = NULL;
}

RetCode_t GraphicsDisplay::set_font(const unsigned char * _font)
{
    font = _font;     // trusting them, but it might be good to put some checks in here...
    return noerror;
}

#ifdef LOCALFONT
int GraphicsDisplay::character(int x, int y, int value)
{
    if (value <= 0x1F && value >= 7F)
        return 0;
    
    return blitbit(x, y, FONT8X8[0][0], FONT8X8[0][1], 
        (char *)&(FONT8x8[value - 0x1F][0]));
}
#else
int GraphicsDisplay::character(int x, int y, int c)
{
    unsigned int offset;
    const unsigned char * charRecord;
 
    if (c <= 0x1F || c >= 0x7F)
        return 0;
    offset = font[0];                    // bytes / char
    charRecord = &font[((c - ' ') * offset) + 4];      // start of char bitmap
    return fontblit(x, y, font, charRecord);
}
#endif

RetCode_t GraphicsDisplay::window(loc_t x, loc_t y, dim_t w, dim_t h)
{
    // current pixel location
    _x = x;
    _y = y;
    // window settings
    _x1 = x;
    _x2 = x + w - 1;
    _y1 = y;
    _y2 = y + h - 1;
    return noerror;
}

void GraphicsDisplay::WindowMax(void)
{
    window(0,0, width(),height());
}

RetCode_t GraphicsDisplay::_putp(color_t color)
{
    pixel(_x, _y, color);
    // update pixel location based on window settings
    _x++;
    if(_x > _x2) {
        _x = _x1;
        _y++;
        if(_y > _y2) {
            _y = _y1;
        }
    }
    return noerror;
}

void GraphicsDisplay::fill(int x, int y, int w, int h, color_t color)
{
    fillrect(x,y, x+w, y+h, color);
}

RetCode_t GraphicsDisplay::cls()
{
    fill(0, 0, width(), height(), _background);
    return noerror;
}

void GraphicsDisplay::blit(int x, int y, int w, int h, const int * color)
{
    window(x, y, w, h);
    _StartGraphicsStream();
    for (int i=0; i<w*h; i++) {
        _putp(color[i]);
    }
    _EndGraphicsStream();
    WindowMax();
}

#ifdef LOCALFONT
int GraphicsDisplay::blitbit(int x, int y, int w, int h, const char * color)
{
    _foreground = 0xFFFF;
    INFO("blitbit(%d,%d, %d,%d, %02X) [%04X,%04X]", x,y, w,h, *color, _foreground, _background);
    INFO("%lu  %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
        color,
        color[0], color[1], color[2], color[3], color[4], color[5], color[6], color[7], 
        color[8], color[9], color[10], color[11], color[12], color[13], color[14], color[15]);
    window(x, y, w, h);
    _StartGraphicsStream();
    for (int i = 0; i < w*h; i++) {
        char byte = color[i >> 3];
        int offset = i & 0x7;
        if (offset == 0)
            INFO(" %2d = %02X", i>>3, byte);
        int c = ((byte << offset) & 0x80) ? _foreground : _background;
        _putp(c);
    }
    _EndGraphicsStream();
    WindowMax();
    return w;
}
#endif


int GraphicsDisplay::fontblit(int x, int y, const unsigned char * fontTable, const unsigned char * fontChar)
{
    //int fontWidth = font[1];                       // get hor size of font
    int fontHeight = font[2];                      // get vert size of font
    int bytesPerLine = font[3];                       // bytes per line
    int charWidth = fontChar[0];        // width of this character
    int px, py;
    
    //INFO("(%d,%d) %lu, %lu %X/%X", x,y, fontTable, fontChar, _foreground, _background);
    //INFO("char size (%d,%d)", charWidth, fontHeight);
    //HexDump("char", (uint8_t *)fontChar, 32);
    //INFO("(f,b) = (%04X,%04X)", _foreground, _background)
    window(x, y, charWidth, fontHeight);
    _StartGraphicsStream();
    //INFO("(f,b) = (%04X,%04X)", _foreground, _background)
    for (py = 0; py < fontHeight; py++) {
        int bitmask = 1 << (py & 7);
        
        for (px = 0; px < charWidth; px++) {
            int offset = (py / 8) + px * bytesPerLine;
            unsigned char byte = fontChar[offset + 1];  // skip the char's # bits wide value
            color_t c = (byte & bitmask) ? _foreground : _background;
            //INFO("(%2d,%2d) %02X & %02X => %04X [%04X,%04X]", px, py, byte, bitmask, c, _foreground, _background);
            //pixel(x+px, y+py, c);
            _putp(c);
        }
    }
    _EndGraphicsStream();
    WindowMax();
    return charWidth;
}

// BMP Color Palette is BGRx
//      BBBB BBBB GGGG GGGG RRRR RRRR 0000 0000
// RGB16 is
//      RRRR RGGG GGGB BBBB
// swap to little endian
//      GGGB BBBB RRRR RGGG
color_t GraphicsDisplay::RGBQuadToRGB16(RGBQUAD * colorPalette, uint16_t i)
{
    color_t c;
 
    c  = ((colorPalette[i].rgbBlue  >> 3) <<  0);
    c |= ((colorPalette[i].rgbGreen >> 2) <<  5);
    c |= ((colorPalette[i].rgbRed   >> 3) << 11);
    return c;
}

/// RRRR RGGG GGGB BBBB
RGBQUAD GraphicsDisplay::RGB16ToRGBQuad(color_t c)
{
    RGBQUAD q;
    
    memset(&q, 0, sizeof(q));
    q.rgbBlue  = (c & 0x001F) << 3;      /* Blue value */
    q.rgbGreen = (c & 0x07E0) >> 3;      /* Green value */
    q.rgbRed   = (c & 0xF800) >> 8;      /* Red value */
    q.rgbReserved = 0;
    return q;
}


RetCode_t GraphicsDisplay::PrintScreen(loc_t x, loc_t y, dim_t w, dim_t h, const char *Name_BMP)
{
    BITMAPFILEHEADER BMP_Header;
    BITMAPINFOHEADER BMP_Info;
    
    INFO("(%d,%d) - (%d,%d) %s", x,y,w,h,Name_BMP);
    if (x >= 0 && x < width()
    && y >= 0 && y < height()
    && w > 0 && x + w <= width()
    && h > 0 && y + h <= height()) {

        BMP_Header.bfType = BF_TYPE;
        BMP_Header.bfSize = (w * h * sizeof(RGBQUAD)) + sizeof(BMP_Header) + sizeof(BMP_Header);
        BMP_Header.bfReserved1 = 0;
        BMP_Header.bfReserved2 = 0;
        BMP_Header.bfOffBits = sizeof(BMP_Header) + sizeof(BMP_Header);
        
        BMP_Info.biSize = sizeof(BMP_Info);
        BMP_Info.biWidth = w;
        BMP_Info.biHeight = h;
        BMP_Info.biPlanes = 1;
        BMP_Info.biBitCount = 24;
        BMP_Info.biCompression = BI_RGB;
        BMP_Info.biSizeImage = 0;
        BMP_Info.biXPelsPerMeter = 0;
        BMP_Info.biYPelsPerMeter = 0;
        BMP_Info.biClrUsed = 0;
        BMP_Info.biClrImportant = 0;

        INFO("Writing {%s}", Name_BMP);
        FILE *Image = fopen(Name_BMP, "wb");
        if (!Image) {
            ERR("File not found");
            return(file_not_found);
        }

        // Be optimistic - don't check for errors.
        //HexDump("BMP_Header", (uint8_t *)&BMP_Header, sizeof(BMP_Header));
        fwrite(&BMP_Header, sizeof(char), sizeof(BMP_Header), Image);
        //INFO("fwrite returned %d", r);
        
        //HexDump("BMP_Info", (uint8_t *)&BMP_Info, sizeof(BMP_Info));
        fwrite(&BMP_Info, sizeof(char), sizeof(BMP_Info), Image);
        //INFO("fwrite returned %d", r);
        
        int lineBufSize = ((24 * w + 7)/8);
        uint8_t * lineBuffer = (uint8_t *)malloc(lineBufSize);
        if (lineBuffer == NULL) {
            fclose(Image);
            ERR("Not enough RAM for lineBuffer");
            return(not_enough_ram);
        }
        color_t * pixelBuffer = (color_t *)malloc(w * sizeof(color_t));
        if (pixelBuffer == NULL) {
            fclose(Image);
            free(lineBuffer);
            ERR("Not enough RAM for pixelBuffer");
            return(not_enough_ram);
        }
        
        // Read the display from the last line toward the top
        // so we can write the file in one pass.
        for (int j = h - 1; j >= 0; j--) {
            // Read one line of pixels to a local buffer
            if (getPixelStream(pixelBuffer, w, x,y+j) != noerror) {
                ERR("getPixelStream error, and no recovery handler...");
            }
            // Convert the local buffer to RGBQUAD format
            int lb = 0;
            for (int i=0; i<w; i++) {
                color_t pixel = pixelBuffer[x+i];
                // Scale to 24-bits
                RGBQUAD q = RGB16ToRGBQuad(pixel);
                lineBuffer[lb++] = q.rgbBlue;
                lineBuffer[lb++] = q.rgbGreen;
                lineBuffer[lb++] = q.rgbRed;
            }
            // Write to disk
            //HexDump("Line", lineBuffer, lineBufSize);
            fwrite(lineBuffer, sizeof(char), lb, Image);
        }
        fclose(Image);       
        INFO("Image closed"); 
        return noerror;
    } else {
        return bad_parameter;
    }
}

RetCode_t GraphicsDisplay::_RenderBitmap(loc_t x, loc_t y, uint32_t fileOffset, FILE * Image)
{
    BITMAPINFOHEADER BMP_Info;
    RGBQUAD * colorPalette = NULL;
    int colorCount;
    uint8_t * lineBuffer = NULL;
    uint16_t BPP_t;
    uint32_t PixelWidth, PixelHeight;
    //uint32_t start_data;
    unsigned int    i, offset;
    int padd,j;

    // Now, Read the bitmap info header
    fread(&BMP_Info, 1, sizeof(BMP_Info), Image);
    //HexDump("BMP_Info", (uint8_t *)&BMP_Info, sizeof(BMP_Info));
    BPP_t = BMP_Info.biBitCount;
    INFO("biBitCount %04X", BPP_t);
    if (BPP_t != 4 && BPP_t != 8 && BPP_t != 16 && BPP_t != 24) { // Support 4, 8, 16, 24-bits per pixel
        fclose(Image);
        return(not_supported_format);
    }

    PixelHeight = BMP_Info.biHeight;
    PixelWidth = BMP_Info.biWidth;
    if (PixelHeight > height() + y || PixelWidth > width() + x) {
        fclose(Image);
        return(image_too_big);
    }
    INFO("(%d,%d) (%d,%d)", x,y, PixelWidth,PixelHeight);
    if (BMP_Info.biBitCount <= 8) {
        int paletteSize;
        // Read the color palette
        colorCount = 1 << BMP_Info.biBitCount;
        paletteSize = sizeof(RGBQUAD) * colorCount;
        colorPalette = (RGBQUAD *)malloc(paletteSize);
        if (colorPalette == NULL) {
            fclose(Image);
            return(not_enough_ram);
        }
        fread(colorPalette, 1, paletteSize, Image);
        //HexDump("Color Palette", (uint8_t *)colorPalette, paletteSize);
    }

    int lineBufSize = ((BPP_t * PixelWidth + 7)/8);
    //INFO("BPP_t %d, PixelWidth %d, lineBufSize %d", BPP_t, PixelWidth, lineBufSize);
    lineBuffer = (uint8_t *)malloc(lineBufSize);
    if (lineBuffer == NULL) {
        free(colorPalette);
        fclose(Image);
        return(not_enough_ram);
    }
    color_t * pixelBuffer = (color_t *)malloc(PixelWidth * sizeof(color_t));
    if (pixelBuffer == NULL) {
        free(colorPalette);
        free(lineBuffer);
        fclose(Image);
        return(not_enough_ram);
    }

    // the Raw Data records are padded to a multiple of 4 bytes
    int recordSize = 2;
    if (BPP_t == 4) {
        recordSize = 1;
    } else if (BPP_t == 8) {
        recordSize = 1;
    } else if (BPP_t == 16) {
        recordSize = 2;
    } else if (BPP_t == 24) {
        recordSize = 3;
    }
    padd = -1;
    do {
        padd++;
    } while ((PixelWidth * recordSize + padd) % 4 != 0);

    // Define window for top to bottom and left to right so writing auto-wraps
    window(x,y, PixelWidth,PixelHeight);
    SetGraphicsCursor(x, y);
    _StartGraphicsStream();
    
    //start_data = BMP_Header.bfOffBits;
    //HexDump("Raw Data", (uint8_t *)&start_data, 32);
    //INFO("(%d,%d) (%d,%d), [%d,%d]", x,y, PixelWidth,PixelHeight, lineBufSize, padd);
    for (j = PixelHeight - 1; j >= 0; j--) {                //Lines bottom up
        offset = fileOffset + j * (lineBufSize + padd);     // start of line
        fseek(Image, offset, SEEK_SET);
        fread(lineBuffer, 1, lineBufSize, Image);           // read a line - slow !
        //INFO("offset: %6X", offset);
        for (i = 0; i < PixelWidth; i++) {                  // copy pixel data to TFT
            if (BPP_t == 4) {
                uint8_t dPix = lineBuffer[i/2];
                if ((i & 1) == 0)
                    dPix >>= 4;
                dPix &= 0x0F;
                pixelBuffer[i] = RGBQuadToRGB16(colorPalette, dPix);
                //_putp(RGBQuadToRGB16(colorPalette, dPix));
            } else if (BPP_t == 8) {
                pixelBuffer[i] = RGBQuadToRGB16(colorPalette, lineBuffer[i]);
                //_putp(RGBQuadToRGB16(colorPalette, lineBuffer[i]));
            } else if (BPP_t == 16) {
                pixelBuffer[i] = lineBuffer[i];
                //_putp(lineBuffer[i]);
            } else if (BPP_t == 24) {
                color_t color;
                color = RGB(lineBuffer[i*3+2], lineBuffer[i*3+1], lineBuffer[i*3+0]);
                pixelBuffer[i] = color;
                //putp(color);
            }
        }
        pixelStream(pixelBuffer, PixelWidth, x, y++);
    }
    _EndGraphicsStream();
    WindowMax();
    free(lineBuffer);
    free(colorPalette);
    return (noerror);
}


RetCode_t GraphicsDisplay::RenderImageFile(loc_t x, loc_t y, const char *FileName)
{
    if (mystrnicmp(FileName + strlen(FileName) - 4, ".bmp", 4) == 0) {
        return RenderBitmapFile(x,y,FileName);
    } else if (mystrnicmp(FileName + strlen(FileName) - 4, ".ico", 4) == 0) {
        return RenderIconFile(x,y,FileName);
    } else {
        return not_supported_format;
    }
}

RetCode_t GraphicsDisplay::RenderBitmapFile(loc_t x, loc_t y, const char *Name_BMP)
{
    BITMAPFILEHEADER BMP_Header;

    INFO("Opening {%s}", Name_BMP);
    FILE *Image = fopen(Name_BMP, "rb");
    if (!Image) {
        return(file_not_found);
    }

    fread(&BMP_Header, 1, sizeof(BMP_Header), Image);      // get the BMP Header
    INFO("bfType %04X", BMP_Header.bfType);
    //HexDump("BMP_Header", (uint8_t *)&BMP_Header, sizeof(BMP_Header));
    if (BMP_Header.bfType != BF_TYPE) {
        fclose(Image);
        return(not_bmp_format);
    }
    RetCode_t rt = _RenderBitmap(x, y, BMP_Header.bfOffBits, Image);
    if (rt != noerror) {
        return rt;
    } else {
        fclose(Image);
        return (noerror);
    }
}

RetCode_t GraphicsDisplay::RenderIconFile(loc_t x, loc_t y, const char *Name_ICO)
{
    ICOFILEHEADER ICO_Header;
    ICODIRENTRY ICO_DirEntry;

    INFO("Opening {%s}", Name_ICO);
    FILE *Image = fopen(Name_ICO, "rb");
    if (!Image) {
        return(file_not_found);
    }

    fread(&ICO_Header, 1, sizeof(ICO_Header), Image);      // get the BMP Header
    HexDump("ICO_Header", (uint8_t *)&ICO_Header, sizeof(ICO_Header));
    if (ICO_Header.Reserved_zero != 0
    || ICO_Header.icType != IC_TYPE
    || ICO_Header.icImageCount == 0) {
        fclose(Image);
        return(not_ico_format);
    }

    // Read ONLY the first of n possible directory entries.
    fread(&ICO_DirEntry, 1, sizeof(ICO_DirEntry), Image);
    HexDump("ICO_DirEntry", (uint8_t *)&ICO_DirEntry, sizeof(ICO_DirEntry));
    INFO("biBitCount %04X", ICO_DirEntry.biBitCount);
    if (ICO_DirEntry.biBitCount != 0) {     // Expecting this to be zero for ico
        fclose(Image);
        return(not_supported_format);
    }

    RetCode_t rt = _RenderBitmap(x, y, ICO_DirEntry.bfOffBits, Image);
    if (rt == noerror) {
        fclose(Image);
        return (noerror);
    } else {
        return rt;
    }
}

int GraphicsDisplay::columns()
{
    return width() / 8;
}

int GraphicsDisplay::rows()
{
    return height() / 8;
}


