/// RA8875 Display Controller Library.
/// 
/// This is being created for a specific display from buydisplay.com,
/// which is 480 x 272. It has other attributes (like display controller
/// managed backlight brightness. So, there are expectations and some
/// defined constants based on that specific display. Some initial work
/// was done to support other display resolutions (e.g. 800 x 480), but
/// this has not been tested.
///
#include "RA8875.h"

//#define DEBUG "RAIO"
// ...
// INFO("Stuff to show %d", var); // new-line is automatically appended
//
#if (defined(DEBUG) && !defined(TARGET_LPC11U24))
#define INFO(x, ...) std::printf("[INF %s %3d] "x"\r\n", DEBUG, __LINE__, ##__VA_ARGS__);
#define WARN(x, ...) std::printf("[WRN %s %3d] "x"\r\n", DEBUG, __LINE__, ##__VA_ARGS__);
#define ERR(x, ...)  std::printf("[ERR %s %3d] "x"\r\n", DEBUG, __LINE__, ##__VA_ARGS__);
#else
#define INFO(x, ...)
#define WARN(x, ...)
#define ERR(x, ...)
#endif


#define RA8875_DISPLAY_WIDTH  480
#define RA8875_DISPLAY_HEIGHT 272
#define RA8875_COLORDEPTH_BPP 16    /* Not an API */

#ifdef PERF_METRICS
#define PERFORMANCE_RESET performance.reset()
#define REGISTERPERFORMANCE(a) RegisterPerformance(a)
static const char *metricsName[] = 
{
    "Cls", "Pixel", "Pixel Stream", 
    "Read Pixel", "Read Pixel Stream",
    "Line", 
    "Rectangle", "Rounded Rectangle", "Triangle", "Circle", "Ellipse"
};
#else
#define PERFORMANCE_RESET
#define REGISTERPERFORMANCE(a)
#endif

// When it is going to poll a register for completion, how many 
// uSec should it wait between each polling activity.
#define POLLWAITuSec 10


RA8875::RA8875(PinName mosi, PinName miso, PinName sclk, PinName csel, PinName reset, const char *name)
    : spi(mosi, miso, sclk)
    , cs(csel)
    , res(reset)
    , GraphicsDisplay(name)
{
    font = NULL;                        // no external font, use internal.
    select(false);                      // deselect the display
    frequency(RA8875_DEFAULT_SPI_FREQ); // data rate
    Reset();
    Power(true);
    Backlight_u8(255);
#ifdef PERF_METRICS
    performance.start();
    ClearPerformance();
#endif
}


//RA8875::~RA8875()
//{
//}


RetCode_t RA8875::SelectDrawingLayer(uint16_t layer)
{
    unsigned char mwcr1 = ReadCommand(0x41) & ~0x01; // retain all but the currently selected layer

    if (width() >= 800 && height() >= 480 && color_bpp() == 8) {
        return bad_parameter;
    } else if (layer > 1) {
        return bad_parameter;
    } else { // layer == 0 ro 1
        WriteCommand(0x41, mwcr1 | layer);
    }
    return noerror;
}


RetCode_t RA8875::SetLayerMode(LayerMode_T mode)
{
    unsigned char ltpr0 = ReadCommand(0x52) & ~0x7; // retain all but the display layer mode
    if (mode <= (LayerMode_T)6) {
        WriteCommand(0x52, ltpr0 | (mode & 0x7));
        return noerror;
    } else {
        return bad_parameter;
    }
}


RetCode_t RA8875::SetLayerTransparency(uint8_t layer1, uint8_t layer2)
{
    if (layer1 > 8)
        layer1 = 8;
    if (layer2 > 8)
        layer2 = 8;
    WriteCommand(0x53, ((layer2 & 0xF) << 4) | (layer1 & 0xF));
    return noerror;
}


RetCode_t RA8875::SetBackgroundTransparencyColor(color_t color)
{
    WriteCommand(0x67, (color >> 11) & 0x1F);
    WriteCommand(0x68, (color >> 5) & 0x3F);
    WriteCommand(0x69, (color & 0x1F));
    return noerror;
}

// ### Touch Panel support code additions begin here

RetCode_t RA8875::TouchPanelInit(void)
{        
    //TPCR0: Set enable bit, default sample time, wakeup, and ADC clock
    WriteCommand(TPCR0, TP_ENABLE | TP_ADC_SAMPLE_DEFAULT_CLKS | TP_ADC_CLKDIV_DEFAULT);
    // TPCR1: Set auto/manual, Ref voltage, debounce, manual mode params
    WriteCommand(TPCR1, TP_MODE_DEFAULT | TP_DEBOUNCE_DEFAULT);   
    WriteCommand(INTC1, ReadCommand(INTC1) | RA8875_INT_TP);        // reg INTC1: Enable Touch Panel Interrupts (D2 = 1)
    WriteCommand(INTC2, RA8875_INT_TP);                            // reg INTC2: Clear any TP interrupt flag
    return noerror;
}

RetCode_t RA8875::TouchPanelInit(uint8_t bTpEnable, uint8_t bTpAutoManual, uint8_t bTpDebounce, uint8_t bTpManualMode, uint8_t bTpAdcClkDiv, uint8_t bTpAdcSampleTime)
{
    // Parameter bounds check
    if( \
        !(bTpEnable == TP_ENABLE || bTpEnable == TP_ENABLE) || \
        !(bTpAutoManual == TP_MODE_AUTO || bTpAutoManual == TP_MODE_MANUAL) || \
        !(bTpDebounce == TP_DEBOUNCE_OFF || bTpDebounce == TP_DEBOUNCE_ON) || \
        !(bTpManualMode <= TP_MANUAL_LATCH_Y) || \
        !(bTpAdcClkDiv <= TP_ADC_CLKDIV_128) || \
        !(bTpAdcSampleTime <= TP_ADC_SAMPLE_65536_CLKS) \
        ) return bad_parameter;
    // Construct the config byte for TPCR0 and write them
    WriteCommand(TPCR0, bTpEnable | bTpAdcClkDiv | bTpAdcSampleTime);    // Note: Wakeup is never enabled
    // Construct the config byte for TPCR1 and write them
    WriteCommand(TPCR1, bTpManualMode | bTpDebounce | bTpManualMode);    // Note: Always uses internal Vref.
    // Set up the interrupt flag and enable bits
    WriteCommand(INTC1, ReadCommand(INTC1) | RA8875_INT_TP);        // reg INTC1: Enable Touch Panel Interrupts (D2 = 1)
    WriteCommand(INTC2, RA8875_INT_TP);                            // reg INTC2: Clear any TP interrupt flag
    return noerror;    
}

unsigned char RA8875::TouchPanelRead(loc_t *x, loc_t *y)
{
    unsigned char touchready;
    static int xbuf[TPBUFSIZE], ybuf[TPBUFSIZE], sample = 0;
    int i, j, temp;

    if( (ReadCommand(INTC2) & RA8875_INT_TP) ) {        // Test for TP Interrupt pending in register INTC2    
        // Get the next data samples
        ybuf[sample] =  ReadCommand(TPYH) << 2 | ( (ReadCommand(TPXYL) & 0xC) >> 2 );   // D[9:2] from reg TPYH, D[1:0] from reg TPXYL[3:2]
        xbuf[sample] =  ReadCommand(TPXH) << 2 | ( (ReadCommand(TPXYL) & 0x3)      );   // D[9:2] from reg TPXH, D[1:0] from reg TPXYL[1:0]
        // Check for a complete set
        if(++sample == TPBUFSIZE) {
           // Buffers are full, so process them using Finn's method described in Analog Dialogue No. 44, Feb 2010
           // This requires sorting the samples in order of size, then discarding the top 25% and
           //   bottom 25% as noise spikes. Finally, the middle 50% of the values are averaged to
           //   reduce Gaussian noise.
           
           // Sort the Y buffer using an Insertion Sort
            for(i = 1; i <= TPBUFSIZE; i++) {
                temp = ybuf[i];
                j = i;
                while( j && (ybuf[j-1] > temp) ) {
                    ybuf[j] = ybuf[j-1];
                    j = j-1;    
                }
                ybuf[j] = temp;             
            } // End of Y sort
            // Sort the X buffer the same way
            for(i = 1; i <= TPBUFSIZE; i++) {
                temp = xbuf[i];
                j = i;
                while( j && (xbuf[j-1] > temp) ) {
                    xbuf[j] = xbuf[j-1];
                    j = j-1;    
                }
                xbuf[j] = temp;             
            } // End of X sort
            // Average the middle half of the  Y values and report them
            j = 0;
            for(i = (TPBUFSIZE/4) - 1; i < TPBUFSIZE - TPBUFSIZE/4; i++ ) {
                j += ybuf[i];    
            }
            *y = j * (float)2/TPBUFSIZE;    // This is the average
            // Average the middle half of the  X values and report them
            j = 0;
            for(i = (TPBUFSIZE/4) - 1; i < TPBUFSIZE - TPBUFSIZE/4; i++ ) {
                j += xbuf[i];    
            }
            *x = j * (float)2/TPBUFSIZE;    // This is the average           
            // Tidy up and return 
            touchready = 1;
            sample = 0;             // Ready to start on the next set of data samples
        }
        else {
            // Buffer not yet full, so do not return any results yet
            touchready = 0;
        }
        WriteCommand(INTC2, RA8875_INT_TP);            // reg INTC2: Clear that TP interrupt flag                       
    } // End of initial if -- data has been read and processed
    else
        touchready = 0;         // Touch Panel "Int" was not set
    return touchready;
}

unsigned char RA8875::TouchPanelReadRaw(loc_t *x, loc_t *y)
{
    unsigned char touchready;

    if( (ReadCommand(INTC2) & RA8875_INT_TP) ) {        // Test for TP Interrupt pending in register INTC2    
        *y =  ReadCommand(TPYH) << 2 | ( (ReadCommand(TPXYL) & 0xC) >> 2 );   // D[9:2] from reg TPYH, D[1:0] from reg TPXYL[3:2]
        *x =  ReadCommand(TPXH) << 2 | ( (ReadCommand(TPXYL) & 0x3)      );   // D[9:2] from reg TPXH, D[1:0] from reg TPXYL[1:0]
        WriteCommand(INTC2, RA8875_INT_TP);            // reg INTC2: Clear that TP interrupt flag   
        touchready = 1;
    }
    else
        touchready = 0;  
    return touchready;
}
// #### end of touch panel code additions

#ifdef PERF_METRICS
void RA8875::ClearPerformance()
{
    for (int i=0; i<METRICCOUNT; i++)
        metrics[i] = 0;
}


void RA8875::RegisterPerformance(method_e method)
{
    unsigned long elapsed = performance.read_us();
    
    if (method < METRICCOUNT && elapsed > metrics[method])
        metrics[method] = elapsed;
}


void RA8875::ReportPerformance(Serial & pc)
{
    pc.printf("\r\nPerformance Metrics\r\n");
    for (int i=0; i<METRICCOUNT; i++) {
        pc.printf("%10d uS %s\r\n", metrics[i], metricsName[i]);
    }    
}
#endif


RetCode_t RA8875::WriteCommandW(uint8_t command, uint16_t data)
{
    #if 1
    WriteCommand(command, data & 0xFF);
    WriteCommand(command+1, data >> 8);
    #else
    // This should be a little faster, but doesn't work...
    INFO("WriteCommandW(%02X, %04X)", command, data);
    select(true);
    spiwrite(0x80);
    spiwrite(command);
    //spiwrite(0x00);     // dummy
    spiwrite(data & 0xFF);
    spiwrite(data >> 8);
    select(false);
    #endif
    return noerror;
}


RetCode_t RA8875::WriteCommand(unsigned char command, unsigned int data)
{
    select(true);
    spiwrite(0x80);         // cmd: write command
    spiwrite(command);
    if (data <= 0xFF) {   // only if in the valid range
        spiwrite(0x00);
        spiwrite(data);
    }
    select(false);
    return noerror;
}


RetCode_t RA8875::WriteDataW(uint16_t data)
{
    select(true);
    spiwrite(0x00);         // cmd: write data
    spiwrite(data & 0xFF);
    spiwrite(data >> 8);
    select(false);
    return noerror;
}


RetCode_t RA8875::WriteData(unsigned char data)
{
    select(true);
    spiwrite(0x00);
    spiwrite(data);
    select(false);
    return noerror;
}


unsigned char RA8875::ReadCommand(unsigned char command)
{
    WriteCommand(command);
    return ReadData();
}


unsigned char RA8875::ReadData(void)
{
    unsigned char data;
    
    select(true);
    spiwrite(0x40);
    data = spiread();
    select(false);
    return data;
}


uint16_t RA8875::ReadDataW(void)
{
    uint16_t data;
    
    select(true);
    spiwrite(0x40);
    data  = spiread();
    data |= (spiread() << 8);
    select(false);
    return data;
}


unsigned char RA8875::ReadStatus(void)
{
    unsigned char data;
    
    select(true);
    spiwrite(0xC0);
    data = spiread();
    select(false);
    return data;
}


dim_t RA8875::fontwidth(void)
{
    if (font == NULL)
        return (((ReadCommand(0x22) >> 2) & 0x3) + 1) * 8;
    else
        return font[1];
}


dim_t RA8875::fontheight(void)
{
    if (font == NULL)
        return (((ReadCommand(0x22) >> 0) & 0x3) + 1) * 16;
    else
        return font[2];
}


RetCode_t RA8875::locate(textloc_t column, textloc_t row)
{
    return SetTextCursor(column * fontwidth(), row * fontheight());
}


int RA8875::columns(void)
{
    return width() / fontwidth();
}


int RA8875::rows(void)
{
    return height() / fontheight();
}


dim_t RA8875::width(void)
{
    return (ReadCommand(0x14) + 1) * 8;
}


dim_t RA8875::height(void)
{
    return (ReadCommand(0x19) | (ReadCommand(0x1A) << 8)) + 1;
}


dim_t RA8875::color_bpp(void)
{
    if ((ReadCommand(0x10) & 0x0C) == 0x04)
        return 16;
    else
        return 8;
}


RetCode_t RA8875::SetTextCursor(loc_t x, loc_t y)
{
    cursor_x = x; cursor_y = y;     // for non-internal fonts
    WriteCommandW(0x2A, x);
    WriteCommandW(0x2C, y);
    return noerror;
}


loc_t RA8875::GetTextCursor_Y(void)
{
    if (font == NULL)
        return ReadCommand(0x2C) | (ReadCommand(0x2D) << 8);
    else
        return cursor_y;
}


loc_t RA8875::GetTextCursor_X(void)
{
    if (font == NULL)
        return ReadCommand(0x2A) | (ReadCommand(0x2B) << 8);
    else
        return cursor_x;
}


RetCode_t RA8875::SetTextCursorControl(cursor_t cursor, bool blink)
{
    unsigned char mwcr0 = ReadCommand(0x40) & 0x0F; // retain direction, auto-increase
    unsigned char mwcr1 = ReadCommand(0x41) & 0x01; // retain currently selected layer
    unsigned char horz = 0;
    unsigned char vert = 0;
    
    mwcr0 |= 0x80;                  // text mode
    if (cursor != NOCURSOR)
        mwcr0 |= 0x40;              // visible
    if (blink)
        mwcr0 |= 0x20;              // blink
    WriteCommand(0x40, mwcr0);      // configure the cursor
    WriteCommand(0x41, mwcr1);      // close the graphics cursor
    WriteCommand(0x44, 0x1f);       // The cursor flashing cycle
    switch (cursor) {
        case IBEAM:
            horz = 0x01;
            vert = 0x1F;
            break;
        case UNDER:
            horz = 0x07;
            vert = 0x01;
            break;
        case BLOCK:
            horz = 0x07;
            vert = 0x1F;
            break;
        case NOCURSOR:
        default:
            break;
    }
    WriteCommand(0x4e, horz);       // The cursor size horz
    WriteCommand(0x4f, vert);       // The cursor size vert
    return noerror;
}


RetCode_t RA8875::SetTextFont(RA8875::font_t font)
{
    if (/*font >= RA8875::ISO8859_1 && */ font <= RA8875::ISO8859_4) {
        WriteCommand(0x21, (unsigned int)(font));
        return noerror;
    } else {
        return bad_parameter;
    }
}


RetCode_t RA8875::SetTextFontControl(fill_t fillit,
    RA8875::font_angle_t angle, 
    RA8875::HorizontalScale hScale, 
    RA8875::VerticalScale vScale, 
    RA8875::alignment_t alignment)
{
    if (hScale >= 1 && hScale <= 4 && 
    vScale >= 1 && vScale <= 4) {
        unsigned char x = 0;
        
        if (alignment == align_full)
            x |= 0x80;
        if (fillit == NOFILL)
            x |= 0x40;
        if (angle == rotated)
            x |= 0x10;
        x |= ((hScale - 1) << 2);
        x |= ((vScale - 1) << 0);
        WriteCommand(0x22, x);
        return noerror;
    } else {
        return bad_parameter;
    }
}


RetCode_t RA8875::SetTextFontSize(RA8875::HorizontalScale hScale, RA8875::VerticalScale vScale)
{
    unsigned char reg = ReadCommand(0x22);
    
    if (vScale == -1)
        vScale = hScale;
    if (hScale >= 1 && hScale <= 4 && vScale >= 1 && vScale <= 4) {
        reg &= 0xF0;    // keep the high nibble as is.
        reg |= ((hScale - 1) << 2);
        reg |= ((vScale - 1) << 0);
        WriteCommand(0x22, reg);
        return noerror;
    } else {
        return bad_parameter;
    }
}


int RA8875::_putc(int c)
{
    if (font == NULL) {
        return _internal_putc(c);
    } else {
        return _external_putc(c);
    }
}


int RA8875::_external_putc(int c)
{
    if (c) {
        if (c == '\r') {
            cursor_x = 0;
        } else if (c == '\n') {
            cursor_y += font[2];
        } else {
            int advance = character(cursor_x, cursor_y, c);     // advance tells us how many pixels we advanced
            //INFO("x,y,advance %d,%d,%d", cursor_x, cursor_y, advance);
            if (advance) {
                cursor_x += advance;
                if (cursor_x >= width()) {
                    cursor_x = 0;
                    cursor_y += font[2];
                    if (cursor_y >= height()) {
                        cursor_y = 0;               // @todo Should it scroll?
                    }
                }
            }
        }
    }
    return c;
}


int RA8875::_internal_putc(int c)
{
    if (c) {
        unsigned char mwcr0;
        
        mwcr0 = ReadCommand(0x40);
        if ((mwcr0 & 0x80) == 0x00) {
            WriteCommand(0x40, 0x80 | mwcr0);    // Put in Text mode if not already
        }
        if (c == '\r') {
            loc_t x;
            x = ReadCommand(0x30) | (ReadCommand(0x31) << 8);   // Left edge of active window
            WriteCommandW(0x2A, x);
        } else if (c == '\n') {
            loc_t y;
            y = ReadCommand(0x2C) | (ReadCommand(0x2D) << 8);   // current y location
            y += fontheight();
            if (y >= height())               // @TODO after bottom of active window, then scroll window?
                y = 0;
            WriteCommandW(0x2C, y);
        } else {
            WriteCommand(0x02);                 // RA8875 Internal Fonts
            select(true);
            WriteData(c);
            while (ReadStatus() & 0x80)
                wait_us(POLLWAITuSec);          // Chk_Busy();
            select(false);
        }
    }
    return c;
}


RetCode_t RA8875::_StartGraphicsStream(void)
{
    WriteCommand(0x40,0x00);    // Graphics write mode
    WriteCommand(0x02);         // Prepare for streaming data
    return noerror;
}


RetCode_t RA8875::_EndGraphicsStream(void)
{
    return noerror;
}


RetCode_t RA8875::_putp(color_t pixel)
{
    WriteDataW((pixel>>8) | (pixel<<8));
    return noerror;   
}


void RA8875::puts(loc_t x, loc_t y, const char * string)
{
    SetTextCursor(x,y);
    puts(string);
}


void RA8875::puts(const char * string)
{
    unsigned char mwcr0 = ReadCommand(0x40);
    
    if (font == NULL) {
        if ((mwcr0 & 0x80) == 0x00)
            WriteCommand(0x40,0x80);    // Put in Text mode if not already
    } else {
        _StartGraphicsStream();
    }
    if (*string != '\0') {
        #if 1
        while (*string) {           // @TODO calling individual _putc is slower... optimizations?
            _putc(*string++);
        }
        #else
        WriteCommand(0x02);
        select(true);
        while (*string != '\0') {
            WriteData(*string);
            ++string;
            while (ReadStatus() & 0x80)
                wait_us(POLLWAITuSec);            // Chk_Busy();
        }
        select(false);
        #endif
    }
    if (font)
        _EndGraphicsStream();
}


RetCode_t RA8875::SetGraphicsCursor(loc_t x, loc_t y)
{
    WriteCommandW(0x46, x);
    WriteCommandW(0x48, y);
    return noerror;
}


RetCode_t RA8875::SetGraphicsCursorRead(loc_t x, loc_t y)
{
    //WriteCommand(0x40, 0);  // Graphics mode
    //WriteCommand(0x45, 0);  // left->right, top->bottom
    WriteCommandW(0x4A, x);
    WriteCommandW(0x4C, y);
    return noerror;
}


RetCode_t RA8875::window(loc_t x, loc_t y, dim_t width, dim_t height)
{
    GraphicsDisplay::window(x,y, width,height);
    WriteCommandW(0x30, x);
    WriteCommandW(0x32, y);
    WriteCommandW(0x34, (x+width-1));
    WriteCommandW(0x36, (y+height-1));
    SetGraphicsCursor(x,y);
    return noerror;
}


RetCode_t RA8875::cls(void)
{
    PERFORMANCE_RESET;
    clsw(FULLWINDOW);
    SetTextCursor(0,0);
    REGISTERPERFORMANCE(PRF_CLS);
    return noerror;
}


RetCode_t RA8875::clsw(RA8875::Region_t region)
{
    PERFORMANCE_RESET;
    WriteCommand(0x8E, (region == ACTIVEWINDOW) ? 0xC0 : 0x80);
    while (ReadCommand(0x8E) & 0x80)
        wait_us(POLLWAITuSec);
    REGISTERPERFORMANCE(PRF_CLS);
    return noerror;
}


RetCode_t RA8875::pixel(loc_t x, loc_t y, color_t color)
{
    #if 1
    return pixelStream(&color, 1, x,y);
    #else
    foreground(color);
    return pixel(x,y);
    #endif
}


RetCode_t RA8875::pixel(loc_t x, loc_t y)
{
    RetCode_t ret;
    
    PERFORMANCE_RESET;
    color_t color = GetForeColor();
    WriteCommand(0x40,0x00);    // Graphics write mode
    SetGraphicsCursor(x, y);
    WriteCommand(0x02);
    WriteDataW(color);
    ret = noerror;
    REGISTERPERFORMANCE(PRF_DRAWPIXEL);
    return ret;
}


RetCode_t RA8875::pixelStream(color_t * p, uint32_t count, loc_t x, loc_t y)
{
    PERFORMANCE_RESET;
    WriteCommand(0x40,0x00);    // Graphics write mode
    SetGraphicsCursor(x, y);
    WriteCommand(0x02);
    select(true);
    spiwrite(0x00);         // Cmd: write data
    while (count--) {
        spiwrite(*p >> 8);
        spiwrite(*p & 0xFF);
        p++;
    }
    select(false);
    REGISTERPERFORMANCE(PRF_PIXELSTREAM);
    return(noerror);
}


color_t RA8875::getPixel(loc_t x, loc_t y)
{
    color_t pixel;
    
    PERFORMANCE_RESET;
    //WriteCommand(0x45,0x00);    // read left->right, top->bottom
    WriteCommand(0x40,0x00);    // Graphics write mode
    SetGraphicsCursorRead(x, y);
    WriteCommand(0x02);
    select(true);
    spiwrite(0x40);         // Cmd: read data
    spiwrite(0x00);         // dummy read
    pixel  = spiread();
    pixel |= (spiread() << 8);
    select(false);
    REGISTERPERFORMANCE(PRF_READPIXEL);
    return pixel;
}


RetCode_t RA8875::getPixelStream(color_t * p, uint32_t count, loc_t x, loc_t y)
{
    color_t pixel;
    
    PERFORMANCE_RESET;
    //WriteCommand(0x45,0x00);    // read left->right, top->bottom
    WriteCommand(0x40,0x00);    // Graphics write mode
    SetGraphicsCursorRead(x, y);
    WriteCommand(0x02);
    select(true);
    spiwrite(0x40);         // Cmd: read data
    spiwrite(0x00);         // dummy read
    while (count--) {
        pixel  = spiread();
        pixel |= (spiread() << 8);
        *p++ = pixel;
    }
    select(false);
    REGISTERPERFORMANCE(PRF_READPIXELSTREAM);
    return noerror;
}


RetCode_t RA8875::line(loc_t x1, loc_t y1, loc_t x2, loc_t y2, color_t color)
{
    foreground(color);
    return line(x1,y1,x2,y2);
}


RetCode_t RA8875::line(loc_t x1, loc_t y1, loc_t x2, loc_t y2)
{
    PERFORMANCE_RESET;
    WriteCommandW(0x91, x1);
    WriteCommandW(0x93, y1);
    WriteCommandW(0x95, x2);
    WriteCommandW(0x97, y2);
    unsigned char drawCmd = 0x00;       // Line
    WriteCommand(0x90, drawCmd);
    WriteCommand(0x90, 0x80 + drawCmd); // Start drawing.
    while (ReadCommand(0x90) & 0x80)    // await completion.
        wait_us(POLLWAITuSec);
    REGISTERPERFORMANCE(PRF_DRAWLINE);
    return noerror;
}


RetCode_t RA8875::fillrect(loc_t x1, loc_t y1, loc_t x2, loc_t y2, 
    color_t color, fill_t fillit)
{
    return rect(x1,y1,x2,y2,color,fillit);
}


RetCode_t RA8875::rect(loc_t x1, loc_t y1, loc_t x2, loc_t y2, 
    color_t color, fill_t fillit)
{
    foreground(color);
    return rect(x1,y1,x2,y2,fillit);
}


RetCode_t RA8875::rect(loc_t x1, loc_t y1, loc_t x2, loc_t y2, 
    fill_t fillit)
{
    PERFORMANCE_RESET;
    if (x1 == x2 && y1 == y2) {
        pixel(x1, y1);
    } else if (x1 == x2) {
        line(x1, y1, x2, y2);
    } else if (y1 == y2) {
        line(x1, y1, x2, y2);
    } else {
        WriteCommandW(0x91, x1);
        WriteCommandW(0x93, y1);
        WriteCommandW(0x95, x2);
        WriteCommandW(0x97, y2);
        unsigned char drawCmd = 0x10;   // Rectangle
        if (fillit == FILL)
            drawCmd |= 0x20;
        WriteCommand(0x90, drawCmd);
        WriteCommand(0x90, 0x80 + drawCmd); // Start drawing.
        while (ReadCommand(0x90) & 0x80)    // await completion.
            wait_us(POLLWAITuSec);
    }
    REGISTERPERFORMANCE(PRF_DRAWRECTANGLE);
    return noerror;
}


RetCode_t RA8875::fillroundrect(loc_t x1, loc_t y1, loc_t x2, loc_t y2, 
    dim_t radius1, dim_t radius2, color_t color, fill_t fillit)
{
    foreground(color);
    return roundrect(x1,y1,x2,y2,radius1,radius2,fillit);
}


RetCode_t RA8875::roundrect(loc_t x1, loc_t y1, loc_t x2, loc_t y2, 
    dim_t radius1, dim_t radius2, color_t color, fill_t fillit)
{
    foreground(color);
    return roundrect(x1,y1,x2,y2,radius1,radius2,fillit);
}


RetCode_t RA8875::roundrect(loc_t x1, loc_t y1, loc_t x2, loc_t y2, 
    dim_t radius1, dim_t radius2, fill_t fillit)
{
    RetCode_t ret = noerror;
    
    PERFORMANCE_RESET;
    if (x1 > x2 || y1 > y2 || (radius1 > (x2-x1)/2) || (radius2 > (y2-y1)/2) ) {
        ret = bad_parameter;
    } else if (x1 == x2 && y1 == y2) {
        pixel(x1, y1);
    } else if (x1 == x2) {
        line(x1, y1, x2, y2);
    } else if (y1 == y2) {
        line(x1, y1, x2, y2);
    } else {
        WriteCommandW(0x91, x1);
        WriteCommandW(0x93, y1);
        WriteCommandW(0x95, x2);
        WriteCommandW(0x97, y2);
        WriteCommandW(0xA1, radius1);
        WriteCommandW(0xA3, radius2);
        // Should not need this...
        WriteCommandW(0xA5, 0);
        WriteCommandW(0xA7, 0);
        unsigned char drawCmd = 0x20;       // Rounded Rectangle
        if (fillit == FILL)
            drawCmd |= 0x40;
        WriteCommand(0xA0, drawCmd);
        WriteCommand(0xA0, 0x80 + drawCmd); // Start drawing.
        while (ReadCommand(0xA0) & 0x80) {   // await completion.
            wait_us(POLLWAITuSec);
        }
    }
    REGISTERPERFORMANCE(PRF_DRAWROUNDEDRECTANGLE);
    return ret;
}


RetCode_t RA8875::triangle(loc_t x1, loc_t y1, loc_t x2, loc_t y2, 
    loc_t x3, loc_t y3, color_t color, fill_t fillit)
{
    RetCode_t ret;
    
    foreground(color);
    ret = triangle(x1,y1,x2,y2,x3,y3,fillit);
    return ret;
}


RetCode_t RA8875::filltriangle(loc_t x1, loc_t y1, loc_t x2, loc_t y2, 
    loc_t x3, loc_t y3, color_t color, fill_t fillit)
{
    RetCode_t ret;

    foreground(color);
    ret = triangle(x1,y1,x2,y2,x3,y3,fillit);
    return ret;
}


RetCode_t RA8875::triangle(loc_t x1, loc_t y1 ,loc_t x2, loc_t y2, 
    loc_t x3, loc_t y3, fill_t fillit)
{
    RetCode_t ret = noerror;
    
    PERFORMANCE_RESET;
    if (x1 == x2 && y1 == y2 && x1 == x3 && y1 == y3) {
        pixel(x1, y1);
    } else {
        WriteCommandW(0x91, x1);
        WriteCommandW(0x93, y1);
        WriteCommandW(0x95, x2);
        WriteCommandW(0x97, y2);
        WriteCommandW(0xA9, x3);
        WriteCommandW(0xAB, y3);
        unsigned char drawCmd = 0x01;       // Triangle
        if (fillit == FILL)
            drawCmd |= 0x20;
        WriteCommand(0x90, drawCmd);
        WriteCommand(0x90, 0x80 + drawCmd); // Start drawing.
        while (ReadCommand(0x90) & 0x80)    // await completion.
            wait_us(POLLWAITuSec);
    }
    REGISTERPERFORMANCE(PRF_DRAWTRIANGLE);
    return ret;
}

RetCode_t RA8875::circle(loc_t x, loc_t y, dim_t radius, 
    color_t color, fill_t fillit)
{
    foreground(color);
    return circle(x,y,radius,fillit);
}


RetCode_t RA8875::fillcircle(loc_t x, loc_t y, dim_t radius, 
    color_t color, fill_t fillit)
{
    foreground(color);
    return circle(x,y,radius,fillit);
}


RetCode_t RA8875::circle(loc_t x, loc_t y, dim_t radius, fill_t fillit)
{
    RetCode_t ret = noerror;
    
    PERFORMANCE_RESET;
    if (radius <= 0) {
        ret = bad_parameter;
    } else if (radius == 1) {
        pixel(x,y);
    } else {
        WriteCommandW(0x99, x);
        WriteCommandW(0x9B, y);
        WriteCommand(0x9d, radius & 0xFF);
        unsigned char drawCmd = 0x00;       // Circle
        if (fillit == FILL)
            drawCmd |= 0x20;
        WriteCommand(0x90, drawCmd);
        WriteCommand(0x90, 0x40 + drawCmd); // Start drawing.
        while (ReadCommand(0x90) & 0x40)    // await completion.
            wait_us(POLLWAITuSec);
    }
    REGISTERPERFORMANCE(PRF_DRAWCIRCLE);
    return ret;
}


RetCode_t RA8875::ellipse(loc_t x, loc_t y, dim_t radius1, dim_t radius2, color_t color, fill_t fillit)
{
    foreground(color);
    return ellipse(x,y,radius1,radius2,fillit);
}


RetCode_t RA8875::fillellipse(loc_t x, loc_t y, dim_t radius1, dim_t radius2, color_t color, fill_t fillit)
{
    foreground(color);
    return ellipse(x,y,radius1,radius2,fillit);
}

        
RetCode_t RA8875::ellipse(loc_t x, loc_t y, dim_t radius1, dim_t radius2, fill_t fillit)
{
    RetCode_t ret = noerror;
    
    PERFORMANCE_RESET;
    if (radius1 <= 0 || radius2 <= 0) {
        ;   // do nothing
    } else if (radius1 == 1 && radius2 == 1) {
        pixel(x, y);
    } else {
        WriteCommandW(0xA5, x);
        WriteCommandW(0xA7, y);
        WriteCommandW(0xA1, radius1);
        WriteCommandW(0xA3, radius2);
        unsigned char drawCmd = 0x00;   // Ellipse
        if (fillit == FILL)
            drawCmd |= 0x40;
        WriteCommand(0xA0, drawCmd);
        WriteCommand(0xA0, 0x80 + drawCmd); // Start drawing.
        while (ReadCommand(0xA0) & 0x80)    // await completion.
            wait_us(POLLWAITuSec);
    }
    REGISTERPERFORMANCE(PRF_DRAWELLIPSE);
    return ret;
}


RetCode_t RA8875::frequency(unsigned long Hz)
{
    spi.frequency(Hz);
    //       __   ___
    // Clock   ___A     Rising edge latched
    //       ___ ____
    // Data  ___X____
    spi.format(8, 3);           // 8 bits and clock to data phase 0
    return noerror;
}


RetCode_t RA8875::Power(bool on)
{
    WriteCommand(0x01, (on) ? 0x80 : 0x00);
    return noerror;
}


RetCode_t RA8875::Reset(void)
{
    WriteCommand(0x01, 0x01);   // Apply Display Off, Reset
    wait_ms(2);                     // no idea if I need to wait, or how long
    WriteCommand(0x01, 0x00);   // Display off, Remove reset
    wait_ms(2);                     // no idea if I need to wait, or how long    
    init(RA8875_DISPLAY_WIDTH, RA8875_DISPLAY_HEIGHT, RA8875_COLORDEPTH_BPP);
    return noerror;
}


RetCode_t RA8875::Backlight_u8(unsigned char brightness)
{
    static bool is_enabled = false;
    if (brightness == 0) {
        WriteCommand(0x8a); // Disable the PWM
        WriteData(0x00);
        is_enabled = false;
    } else if (!is_enabled) {
        WriteCommand(0x8a); // Enable the PWM
        WriteData(0x80);
        WriteCommand(0x8a); // Not sure why this is needed, but following the pattern
        WriteData(0x81);    // open PWM (SYS_CLK / 2 as best I can tell)
        is_enabled = true;
    }
    WriteCommand(0x8b, brightness);  // Brightness parameter 0xff-0x00
    return noerror;
}


RetCode_t RA8875::Backlight(float brightness)
{
    unsigned char b;
    
    if (brightness >= 1.0)
        b = 255;
    else if (brightness <= 0.0)
        b = 0;
    else
        b = (unsigned char)(brightness * 255);
    return Backlight_u8(b);
}


RetCode_t RA8875::set_font(const unsigned char * _font)
{
    if (font && ! _font) {
        SetTextCursor(cursor_x, cursor_y);  // soft-font cursor -> hw cursor
    }
    font = _font;
    GraphicsDisplay::set_font(_font);
    return noerror;     // trusting them, but it might be good to put some checks in here...
}


RetCode_t RA8875::background(color_t color)
{
    GraphicsDisplay::background(color);
    WriteCommand(0x60, (color>>11));                  // BGCR0
    WriteCommand(0x61, (unsigned char)(color>>5));    // BGCR0
    WriteCommand(0x62, (unsigned char)(color));       // BGCR0
    return noerror;
}


RetCode_t RA8875::background(unsigned char r, unsigned char g, unsigned char b)
{
    background(RGB(r,g,b));
//    WriteCommand(0x60, r);
//    WriteCommand(0x61, g);
//    WriteCommand(0x62, b);
    return noerror;
}


RetCode_t RA8875::foreground(color_t color)
{
    GraphicsDisplay::foreground(color);
    WriteCommand(0x63, (unsigned char)(color>>11));
    WriteCommand(0x64, (unsigned char)(color>>5));
    WriteCommand(0x65, (unsigned char)(color));
    return noerror;
}


RetCode_t RA8875::foreground(unsigned char r, unsigned char g, unsigned char b)
{
    foreground(RGB(r,g,b));
//    WriteCommand(0x63, r);
//    WriteCommand(0x64, g);
//    WriteCommand(0x65, b);
    return noerror;
}


color_t RA8875::GetForeColor(void)
{
    color_t color;
    
    color  = (ReadCommand(0x63) & 0x1F) << 11;
    color |= (ReadCommand(0x64) & 0x3F) << 5;
    color |= (ReadCommand(0x65) & 0x1F);
    return color;
}


color_t RA8875::DOSColor(int i)
    {
    const color_t colors[16] = 
        {
        Black,    Blue,       Green,       Cyan,
        Red,      Magenta,    Brown,       Gray,
        Charcoal, BrightBlue, BrightGreen, BrightCyan,
        Orange,   Pink,       Yellow,      White
        };
    if (i < 16)
        return colors[i];
    else
        return 0;
    }


const char * RA8875::DOSColorNames(int i) 
    {
    const char * names[16] = 
        {
        "Black",    "Blue",       "Green",       "Cyan",
        "Red",      "Magenta",    "Brown",       "Gray",
        "Charcoal", "BrightBlue", "BrightGreen", "BrightCyan",
        "Orange",   "Pink",       "Yellow",      "White"
        };
    if (i < 16)
        return names[i];
    else
        return NULL;
    }


///////////////////////////////////////////////////////////////
// Private functions

unsigned char RA8875::spiwrite(unsigned char data)
{
    unsigned char retval;
    
    retval = spi.write(data);
    return retval;
}


unsigned char RA8875::spiread(void)
{
    unsigned char retval;
    unsigned char data = 0;
    
    retval = spi.write(data);
    return retval;
}


RetCode_t RA8875::select(bool chipsel)
{
    cs = (chipsel == true) ? 0 : 1;
    return noerror;
}


RetCode_t RA8875::init(int width, int height, int color_bpp)
{
    Backlight_u8(0);
    WriteCommand(0x88, 0x0a);                   // PLLC1 - Phase Lock Loop registers
    wait_ms(1);
    WriteCommand(0x89, 0x02);
    wait_ms(1);
    
    // System Config Register (SYSR)
    if (color_bpp == 16) {
        WriteCommand(0x10, 0x0C);               // 16-bpp (65K colors) color depth, 8-bit interface
    } else { // color_bpp == 8
        WriteCommand(0x10, 0x00);               // 8-bpp (256 colors)
    }
    // Pixel Clock Setting Register (PCSR)
    WriteCommand(0x04, 0x82);                   // PDAT on PCLK falling edge, PCLK = 4 x System Clock
    wait_ms(1);

    // Horizontal Settings
    WriteCommand(0x14, width/8 - 1);            //HDWR//Horizontal Display Width Setting Bit[6:0]
    WriteCommand(0x15, 0x02);                   //HNDFCR//Horizontal Non-Display Period fine tune Bit[3:0]
    WriteCommand(0x16, 0x03);                   //HNDR//Horizontal Non-Display Period Bit[4:0]
    WriteCommand(0x17, 0x01);                   //HSTR//HSYNC Start Position[4:0]
    WriteCommand(0x18, 0x03);                   //HPWR//HSYNC Polarity ,The period width of HSYNC.

    // Vertical Settings
    WriteCommand(0x19, (height-1)&0xFF);        //VDHR0 //Vertical Display Height Bit [7:0]
    WriteCommand(0x1a, (height-1)>>8);          //VDHR1 //Vertical Display Height Bit [8]
    WriteCommand(0x1b, 0x0F);                   //VNDR0 //Vertical Non-Display Period Bit [7:0]
    WriteCommand(0x1c, 0x00);                   //VNDR1 //Vertical Non-Display Period Bit [8]
    WriteCommand(0x1d, 0x0e);                   //VSTR0 //VSYNC Start Position[7:0]
    WriteCommand(0x1e, 0x06);                   //VSTR1 //VSYNC Start Position[8]
    WriteCommand(0x1f, 0x01);                   //VPWR //VSYNC Polarity ,VSYNC Pulse Width[6:0]

    if (width >= 800 && height >= 480 && color_bpp > 8) {
        WriteCommand(0x20, 0x00);               // DPCR - 1-layer mode when the resolution is too high
    } else {
        WriteCommand(0x20, 0x80);               // DPCR - 2-layer mode
    }
    
    // Set display image to Blue on Black as default
    window(0,0, width, height);             // Initialize to full screen
    SetTextCursorControl();
    foreground(Blue);
    background(Black);
    SelectDrawingLayer(1);
    cls();
    SelectDrawingLayer(0);
    cls();
    return noerror;
}

#ifdef TESTENABLE

#include "Arial12x12.h"
#include "Small_6.h"

//      ______________  ______________  ______________  _______________
//     /_____   _____/ /  ___________/ /  ___________/ /_____   ______/
//          /  /      /  /            /  /                  /  /
//         /  /      /  /___         /  /__________        /  /
//        /  /      /  ____/        /__________   /       /  /
//       /  /      /  /                       /  /       /  /
//      /  /      /  /__________  ___________/  /       /  /
//     /__/      /_____________/ /_____________/       /__/
//
//    Everything from here down is test code.
bool SuppressSlowStuff = false;


void TextWrapTest(RA8875 & display, Serial & pc)
{
    if (!SuppressSlowStuff)
        pc.printf("Text Wrap Test\r\n");
    display.background(Black);
    display.foreground(Blue);
    display.cls();
    display.Backlight_u8(255);
    display.puts(0,0, "Text Wrap Test.\r\n");
    for (int i=1; i<60; i++) {
        display.printf("L%2d\n", i % 17);
        if (!SuppressSlowStuff)
            wait_ms(100);
    }
    if (!SuppressSlowStuff)
        wait_ms(3000);
}

void TextCursorTest(RA8875 & display, Serial & pc)
{
    const char * iCursor = "The I-Beam cursor should be visible for this text.\r\n";
    const char * uCursor = "The Underscore cursor should be visible for this text.\r\n";
    const char * bCursor = "The Block cursor should be visible for this text.\r\n";
    const char * bbCursor = "The Blinking Block cursor should be visible for this text.\r\n";
    const char * p;
    int delay = 100;
    
    if (!SuppressSlowStuff)
        pc.printf("Text Cursor Test\r\n");
    else 
        delay = 0;
    display.background(Black);
    display.foreground(Blue);
    display.cls();
    display.Backlight_u8(255);
    display.puts(0,0, "Text Cursor Test.");
    
    // visible, non-blinking
    display.SetTextCursor(0,20);
    display.SetTextCursorControl(RA8875::IBEAM, false);
    p = iCursor;
    while (*p) {
        display._putc(*p++);
        wait_ms(delay);
    }

    display.SetTextCursorControl(RA8875::UNDER, false);
    p = uCursor;
    while (*p) {
        display._putc(*p++);
        wait_ms(delay);
    }
    
    display.SetTextCursorControl(RA8875::BLOCK, false);
    p = bCursor;
    while (*p) {
        display._putc(*p++);
        wait_ms(delay);
    }

    display.SetTextCursorControl(RA8875::BLOCK, true);
    p = bbCursor;
    while (*p) {
        display._putc(*p++);
        wait_ms(delay);
    }
    wait_ms(delay * 20);
    display.SetTextCursorControl(RA8875::NOCURSOR, false);
}


void BacklightTest(RA8875 & display, Serial & pc, float ramptime)
{
    char buf[60];
    unsigned int w = (ramptime * 1000)/ 256;
    int delay = 200;

    if (!SuppressSlowStuff)
        pc.printf("Backlight Test - ramp over %f sec.\r\n", ramptime);
    else {
        delay = 0;
        w = 0;
    }
    display.Backlight_u8(0);
    display.background(White);
    display.foreground(Blue);
    display.cls();
    wait_ms(delay);
    display.puts(0,0, "RA8875 Backlight Test - Ramp up.");
    for (int i=0; i <= 255; i++) {
        sprintf(buf, "%3d, %4d", i, w);
        display.puts(100,100,buf);
        display.Backlight_u8(i);
        wait_ms(w);
    }
}


void BacklightTest2(RA8875 & display, Serial & pc)
{
    int delay = 20;

    if (!SuppressSlowStuff)
        pc.printf("Backlight Test 2\r\n");
    else
        delay = 0;

    // Dim it out at the end of the tests.
    display.foreground(Blue);
    display.puts(0,0, "Ramp Backlight down.");
    // Ramp it off
    for (int i=255; i != 0; i--) {
        display.Backlight_u8(i);
        wait_ms(delay);
    }
    display.Backlight_u8(0);
}


void ExternalFontTest(RA8875 & display, Serial & pc)
{
    if (!SuppressSlowStuff)
        pc.printf("External Font Test\r\n");
    display.background(Black);
    display.foreground(Blue);
    display.cls();
    display.Backlight(1);
    display.puts(0,0, "External Font Test.");

    display.set_font(Small_6);
    display.puts(0,30, "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz\r\n");    

    display.set_font(Arial12x12);
    display.puts("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz\r\n");
    display.set_font();     // restore to internal
    
    display.puts("Normal font again.");
    //display.window(0,0, display.width(), display.height());
}


void DOSColorTest(RA8875 & display, Serial & pc)
{
    if (!SuppressSlowStuff)
        pc.printf("DOS Color Test\r\n");
    display.background(Black);
    display.foreground(Blue);
    display.cls();
    display.puts(0,0, "DOS Colors - Fore");
    display.puts(280,0, "Back");
    display.background(Gray);
    for (int i=0; i<16; i++) {
        display.foreground(display.DOSColor(i));
        display.puts(160, i*16, display.DOSColorNames(i));
        display.background(Black);
    }
    display.foreground(White);
    for (int i=0; i<16; i++) {
        display.background(display.DOSColor(i));
        display.puts(360, i*16, display.DOSColorNames(i));
        display.foreground(White);
    }
}


void WebColorTest(RA8875 & display, Serial & pc)
{
    if (!SuppressSlowStuff)
        pc.printf("Web Color Test\r\n");
    display.background(Black);
    display.foreground(Blue);
    display.window(0,0, display.width(), display.height());
    display.cls();
    display.puts(0,0, "Web Color Test\r\n");
    display.SetTextFontSize(1,2);
    for (int i=0; i<sizeof(WebColors)/sizeof(WebColors[0]); i++) {
        display.background(WebColors[i]);
        display.puts(" ");
        if (i % 36 == 35)
            display.puts("\r\n");
    }
    display.SetTextFontSize(1,1);
}


void PixelTest(RA8875 & display, Serial & pc)
{
    int i, c, x, y;

    if (!SuppressSlowStuff)
        pc.printf("Pixel Test\r\n");
    display.background(Black);
    display.foreground(Blue);
    display.cls();
    display.puts(0,0, "Pixel Test");
    for (i=0; i<1000; i++) {
        x = rand() % 480;
        y = 16 + rand() % (272-16);
        c = rand() % 16;
        //pc.printf("  (%d,%d) - %d\r\n", x,y,r1);
        display.pixel(x,y, display.DOSColor(c));
    }
}


void LineTest(RA8875 & display, Serial & pc)
{
    int i, x, y, x2, y2;

    if (!SuppressSlowStuff)
        pc.printf("Line Test\r\n");
    display.background(Black);
    display.foreground(Blue);
    display.cls();
    display.puts(0,0, "Line Test");
    for (i=0; i<16; i++) {
        // Lines
        x = rand() % 480;
        y = rand() % 272;
        x2 = rand() % 480;
        y2 = rand() % 272;
        display.line(x,y, x2,y2, display.DOSColor(i));
    }
}


void RectangleTest(RA8875 & display, Serial & pc)
{
    int i, x1,y1, x2,y2;

    if (!SuppressSlowStuff)
        pc.printf("Rectangle Test\r\n");
    display.background(Black);
    display.foreground(Blue);
    display.cls();
    display.puts(0,0, "Rectangle Test");
    for (i=0; i<16; i++) {
        x1 = rand() % 240;
        y1 = 50 + rand() % 200;
        x2 = rand() % 240;
        y2 = 50 + rand() % 200;
        display.rect(x1,y1, x2,y2, display.DOSColor(i));

        x1 = 240 + rand() % 240;
        y1 = 50 + rand() % 200;
        x2 = 240 + rand() % 240;
        y2 = 50 + rand() % 200;
        display.rect(x1,y1, x2,y2, FILL);
    }
}


void LayerTest(RA8875 & display, Serial & pc)
{
    loc_t i, x1,y1, x2,y2, r1,r2;

    if (!SuppressSlowStuff)
        pc.printf("Layer Test\r\n");

    display.SelectDrawingLayer(0);
    display.background(Black);
    display.foreground(Blue);
    display.cls();
    display.puts(0,0, "Layer 0");
    for (i=0; i<16; i++) {
        x1 = rand() % 240;
        y1 = 50 + rand() % 200;
        x2 = x1 + rand() % 100;
        y2 = y1 + rand() % 100;
        r1 = rand() % (x2 - x1)/2;
        r2 = rand() % (y2 - y1)/2;
        display.roundrect(x1,y1, x2,y2, r1,r2, display.DOSColor(i));
        if (!SuppressSlowStuff)
            wait_ms(20);
    }
    if (!SuppressSlowStuff)
        wait_ms(1000);

    display.SelectDrawingLayer(1);
    display.background(Black);
    display.foreground(Yellow);
    display.cls();
    display.puts(240,0, "Layer 1");
    for (i=0; i<16; i++) {
        x1 = 300 + rand() % 100;
        y1 = 70 + rand() % 200;
        r1 = rand() % min(y1 - 20, 100);
        display.circle(x1,y1,r1, display.DOSColor(i));
        if (!SuppressSlowStuff)
            wait_ms(20);
    }
    display.SetLayerMode(RA8875::ShowLayer1);        // Show it after the build-up
    if (!SuppressSlowStuff)
        wait_ms(2000);

    display.SelectDrawingLayer(0);
    display.SetLayerMode(RA8875::ShowLayer0);        // Show Layer 0 again
    if (!SuppressSlowStuff)
        wait_ms(1000);
    display.SetLayerMode(RA8875::TransparentMode);        // Transparent mode
    if (!SuppressSlowStuff)
        wait_ms(1000);
    for (i=0; i<=8; i++) {
        display.SetLayerTransparency(i, 8-i);
        if (!SuppressSlowStuff)
            wait_ms(200);
    }
    
    // Restore before we exit
    display.SetLayerTransparency(0, 0);
    display.SetLayerMode(RA8875::ShowLayer0);        // Restore to layer 0
}


void RoundRectTest(RA8875 & display, Serial & pc)
{
    loc_t i, x1,y1, x2,y2, r1,r2;

    if (!SuppressSlowStuff)
        pc.printf("Round Rectangle Test\r\n");
    display.background(Black);
    display.foreground(Blue);
    display.cls();
    display.puts(0,0, "Rounded Rectangle Test");
    
    for (i=0; i<16; i++) {
        x1 = rand() % 240;
        y1 = 50 + rand() % 200;
        x2 = x1 + rand() % 100;
        y2 = y1 + rand() % 100;
        r1 = rand() % (x2 - x1)/2;
        r2 = rand() % (y2 - y1)/2;
        display.roundrect(x1,y1, x2,y2, 5,8, display.DOSColor(i));

        x1 = 240 + rand() % 240;
        y1 = 50 + rand() % 200;
        x2 = x1 + rand() % 100;
        y2 = y1 + rand() % 100;
        r1 = rand() % (x2 - x1)/2;
        r2 = rand() % (y2 - y1)/2;
        display.roundrect(x1,y1, x2,y2, r1,r2, FILL);
    }
}


void TriangleTest(RA8875 & display, Serial & pc)
{
    int i, x1, y1, x2, y2, x3, y3;

    if (!SuppressSlowStuff)
        pc.printf("Triangle Test\r\n");
    display.background(Black);
    display.foreground(Blue);
    display.cls();
    display.puts(0,0, "Triangle Test");

    x1 = 150;
    y1 = 2;
    x2 = 190;
    y2 = 7;
    x3 = 170;
    y3 = 16;
    display.triangle(x1,y1, x2,y2, x3,y3);

    x1 = 200;
    y1 = 2;
    x2 = 240;
    y2 = 7;
    x3 = 220;
    y3 = 16;
    display.filltriangle(x1,y1, x2,y2, x3,y3, BrightRed);

    x1 = 300;
    y1 = 2;
    x2 = 340;
    y2 = 7;
    x3 = 320;
    y3 = 16;
    display.triangle(x1,y1, x2,y2, x3,y3, NOFILL);

    x1 = 400;
    y1 = 2;
    x2 = 440;
    y2 = 7;
    x3 = 420;
    y3 = 16;
    display.triangle(x1,y1, x2,y2, x3,y3, Blue);

    for (i=0; i<16; i++) {
        x1 = rand() % 240;
        y1 = 50 + rand() % 200;
        x2 = rand() % 240;
        y2 = 50 + rand() % 200;
        x3 = rand() % 240;
        y3 = 50 + rand() % 200;
        display.triangle(x1,y1, x2,y2, x3,y3, display.DOSColor(i));
        x1 = 240 + rand() % 240;
        y1 = 50 + rand() % 200;
        x2 = 240 + rand() % 240;
        y2 = 50 + rand() % 200;
        x3 = 240 + rand() % 240;
        y3 = 50 + rand() % 200;
        display.triangle(x1,y1, x2,y2, x3,y3, FILL);
    }
}


void CircleTest(RA8875 & display, Serial & pc)
{
    int i, x, y, r1;

    if (!SuppressSlowStuff)
        pc.printf("Circle Test\r\n");
    display.background(Black);
    display.foreground(Blue);
    display.cls();
    display.puts(0,0, "Circle Test");
    for (i=0; i<16; i++) {
        x = 100 + rand() % 100;
        y = 70 + rand() % 200;
        r1 = rand() % min(y - 20, 100);
        //pc.printf("  (%d,%d) - %d\r\n", x,y,r1);
        display.circle(x,y,r1, display.DOSColor(i));

        x = 300 + rand() % 100;
        y = 70 + rand() % 200;
        r1 = rand() % min(y - 20, 100);
        //pc.printf("  (%d,%d) - %d FILL\r\n", x,y,r1);
        display.circle(x,y,r1, display.DOSColor(i), FILL);
    }
}


void EllipseTest(RA8875 & display, Serial & pc)
{
    int i,x,y,r1,r2;

    if (!SuppressSlowStuff)
        pc.printf("Ellipse Test\r\n");
    display.background(Black);
    display.foreground(Blue);
    display.cls();
    display.puts(0,0, "Ellipse Test");
    for (i=0; i<16; i++) {
        x = 100 + rand() % 100;
        y = 70 + rand() % 200;
        r1 = rand() % min(y - 20, 100);
        r2 = rand() % min(y - 20, 100);
        display.ellipse(x,y,r1,r2, display.DOSColor(i));

        x = 300 + rand() % 100;
        y = 70 + rand() % 200;
        r1 = rand() % min(y - 20, 100);
        r2 = rand() % min(y - 20, 100);
        display.ellipse(x,y,r1,r2, FILL);
    }
}


void TestGraphicsBitmap(RA8875 & display, Serial & pc)
{
    LocalFileSystem local("local");
    if (!SuppressSlowStuff)
        pc.printf("Bitmap File Load\r\n");    
    display.background(Black);
    display.foreground(Blue);
    display.cls();
    display.puts(0,0, "Graphics Test, loading /local/TestPat.bmp");
    wait(3);

    int r = display.RenderBitmapFile(0,0, "/local/TestPat.bmp");
}


void SpeedTest(RA8875 & display, Serial & pc)
{
    Timer t;
    SuppressSlowStuff = true;
    pc.printf("\r\nSpeedTest disables delays, runs tests, reports overall time.\r\n");
    t.start();
    // do stuff fast
    TextCursorTest(display, pc);
    TextWrapTest(display, pc);
    BacklightTest(display, pc, 0);
    BacklightTest2(display, pc);
    ExternalFontTest(display, pc);
    DOSColorTest(display, pc);
    WebColorTest(display, pc);
    PixelTest(display, pc);
    LineTest(display, pc);
    RectangleTest(display, pc);
    RoundRectTest(display, pc);
    TriangleTest(display, pc);
    CircleTest(display, pc);
    EllipseTest(display, pc);
    LayerTest(display, pc);
    //TestGraphicsBitmap(display, pc);
    pc.printf("SpeedTest completed in %d msec\r\n", t.read_ms());
    #ifdef PERF_METRICS
    display.ReportPerformance(pc);
    #endif
    SuppressSlowStuff = false;
}


void PrintScreen(RA8875 & display, Serial & pc)
{
    LocalFileSystem local("local");
    if (!SuppressSlowStuff)
        pc.printf("PrintScreen\r\n");    
    display.PrintScreen( 0,0, 480,272, "/local/Capture.bmp");
}


void RunTestSet(RA8875 & lcd, Serial & pc)
{
    int q = 0;
    int automode = 0;
    const unsigned char modelist[] = "BDWtGLlFROTPCEbw";   // auto-test in this order.

    while(1) {
        pc.printf("\r\n"
                  "B - Backlight up      b - backlight dim\r\n"
                  "D - DOS Colors        W - Web Colors\r\n"
                  "t - text cursor       G - Graphics Bitmap\r\n"
                  "L - Lines             F - external Font\r\n"
                  "R - Rectangles        O - rOund rectangles\r\n"
                  "T - Triangles         P - Pixels  \r\n"
                  "C - Circles           E - Ellipses\r\n"
                  "A - Auto Test mode    S - Speed Test\r\n"
                  "p - print screen      r - reset  \r\n"
                  "l - layer test        w - wrapping text \r\n"
                  #ifdef DEBUG
                  "0 - clear performance 1 - report performance\r\n"
                  #endif
                  "> ");
        if (automode == -1 || pc.readable()) {
            automode = -1;
            q = pc.getc();
            while (pc.readable())
                pc.getc();
        } else if (automode >= 0) {
            q = modelist[automode];
        }
        switch(q) {
            #ifdef DEBUG
            case '0':
                lcd.ClearPerformance();
                break;
            case '1':
                lcd.ReportPerformance(pc);
                break;
            #endif
            case 'A':
                automode = 0;
                break;
            case 'B':
                BacklightTest(lcd, pc, 2);
                break;
            case 'b':
                BacklightTest2(lcd, pc);
                break;
            case 'D':
                DOSColorTest(lcd, pc);
                break;
            case 'W':
                WebColorTest(lcd, pc);
                break;
            case 't':
                TextCursorTest(lcd, pc);
                break;
            case 'w':
                TextWrapTest(lcd, pc);
                break;
            case 'F':
                ExternalFontTest(lcd, pc);
                break;
            case 'L':
                LineTest(lcd, pc);
                break;
            case 'l':
                LayerTest(lcd, pc);
                break;
            case 'R':
                RectangleTest(lcd, pc);
                break;
            case 'O':
                RoundRectTest(lcd, pc);
                break;
            case 'p':
                PrintScreen(lcd, pc);
                break;
            case 'S':
                SpeedTest(lcd, pc);
                break;
            case 'T':
                TriangleTest(lcd, pc);
                break;
            case 'P':
                PixelTest(lcd, pc);
                break;
            case 'G':
                TestGraphicsBitmap(lcd, pc);
                break;
            case 'C':
                CircleTest(lcd, pc);
                break;
            case 'E':
                EllipseTest(lcd, pc);
                break;
            case 'r':
                pc.printf("Resetting ...\r\n");
                wait_ms(20);
                mbed_reset();
                break;
            default:
                printf("huh?\n");
                break;
        }
        if (automode >= 0) {
            automode++;
            if (automode >= sizeof(modelist))
                automode = 0;
            wait_ms(2000);
        }
        wait_ms(200);
    }
}

#endif // TESTENABLE