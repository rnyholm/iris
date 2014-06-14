#ifndef RA8875_H
#define RA8875_H
#include <mbed.h>

#include "GraphicsDisplay.h"

#define RA8875_DEFAULT_SPI_FREQ 5000000

// Define this to enable code that monitors the performance of various
// graphics commands.
//#define PERF_METRICS

// What better place for some test code than in here and the companion
// .cpp file. See also the bottom of this file.
#define TESTENABLE

/// DOS colors - slightly color enhanced
#define Black       (color_t)(RGB(0,0,0))
#define Blue        (color_t)(RGB(0,0,187))
#define Green       (color_t)(RGB(0,187,0))
#define Cyan        (color_t)(RGB(0,187,187))
#define Red         (color_t)(RGB(187,0,0))
#define Magenta     (color_t)(RGB(187,0,187))
#define Brown       (color_t)(RGB(187,187,0))
#define Gray        (color_t)(RGB(187,187,187))
#define Charcoal    (color_t)(RGB(85,85,85))
#define BrightBlue  (color_t)(RGB(85,85,255))
#define BrightGreen (color_t)(RGB(85,255,85))
#define BrightCyan  (color_t)(RGB(85,255,255))
#define Orange      (color_t)(RGB(255,85,85))
#define Pink        (color_t)(RGB(255,85,255))
#define Yellow      (color_t)(RGB(255,255,85))
#define White       (color_t)(RGB(255,255,255))

#define BrightRed   (color_t)(RGB(255,0,0))

//namespace SW_graphics
//{


/// This is a graphics library for the Raio RA8875 Display Controller chip
/// attached to a 4-wire SPI interface.
///
/// It offers both primitive and high level APIs.
///
/// Central to this API is a coordinate system, where the origin (0,0) is in
/// the top-left corner of the display, and the width (x) extends positive to the
/// right and the height (y) extends positive toward the bottom.
///
/// @caution As there are both graphics and text commands, one must take care to use
/// the proper coordinate system for each. Some of the text APIs are in units
/// of column and row, which is measured in character positions (and dependent
/// on the font size), where other text APIs permit pixel level positioning.
///
/// @code
/// #include "RA8875.h"
/// RA8875 lcd(p5, p6, p7, p12, NC, "tft");
///
/// int main()
/// {
///     lcd.printf("printing 3 x 2 = %d", 3*2);
///     lcd.circle(       400,25,  25,               BrightRed);
///     lcd.fillcircle(   400,25,  15,               RGB(128,255,128));
///     lcd.ellipse(      440,75,  35,20,            BrightBlue);
///     lcd.fillellipse(  440,75,  25,10,            Blue);
///     lcd.triangle(     440,100, 475,110, 450,125, Magenta);
///     lcd.filltriangle( 445,105, 467,111, 452,120, Cyan);
///     lcd.rect(         400,130, 475,155,          Brown);
///     lcd.fillrect(     405,135, 470,150,          Pink);
///     lcd.roundrect(    410,160, 475,190, 10,8,    Yellow);
///     lcd.fillroundrect(415,165, 470,185,  5,3,    Orange);
///     lcd.line(         430,200, 460,230,          RGB(0,255,0));
///     for (int i=0; i<=30; i+=5) 
///         lcd.pixel(435+i,200+i, White);
/// }
/// @endcode
///
/// @todo Add Scroll support for text.
/// @todo Improve sync between internal and external font support - cursor, window, scroll.
/// @todo Find out why it can't shift frequency after constructor runs.
/// @todo Add Hardware reset signal.
/// @todo Add Keypad Support.
/// @todo Add high level objects - x-y graph, meter, others... but these will
///     probably be best served in another class, since they may not
///     be needed for many uses.
/// 
class RA8875 : public GraphicsDisplay
{
public:   
    /// cursor type to be shown as the text cursor.
    typedef enum
    {
        NOCURSOR,   ///< cursor is hidden
        IBEAM,      ///< | cursor
        UNDER,      ///< _ cursor
        BLOCK       ///< Block cursor
    } cursor_t;

    /// font type selection.
    typedef enum
    {
        ISO8859_1,      ///< ISO8859-1 font
        ISO8859_2,      ///< ISO8859-2 font
        ISO8859_3,      ///< ISO8859-3 font
        ISO8859_4       ///< ISO8859-4 font
    } font_t;
    
    /// font rotation selection
    typedef enum
    {
        normal,         ///< normal orientation
        rotated         ///< rotated orientation
    } font_angle_t;
    
    /// alignment  
    typedef enum
    {
        align_none,     ///< align - none
        align_full      ///< align - full
    } alignment_t;    
    
    /// Scale factor - 1, 2, 3 4
    typedef int HorizontalScale;
    
    /// Scale factor - 1, 2, 3, 4
    typedef int VerticalScale;

    /// Clear screen region
    typedef enum
    {
        FULLWINDOW,     ///< Full screen
        ACTIVEWINDOW    ///< active window/region
    } Region_t;
    
    /// Set the Layer 1/2 Display Mode
    typedef enum
    {
        ShowLayer0,         ///< Only layer 0 is visible, layer 1 is hidden
        ShowLayer1,         ///< Only layer 1 is visible, layer 0 is hidden
        LightenOverlay,     ///< Lighten-overlay mode
        TransparentMode,    ///< Transparent mode
        BooleanOR,          ///< Boolean OR mode
        BooleanAND,         ///< Boolean AND mode
        FloatingWindow      ///< Floating Window mode
    } LayerMode_T;
    
    /// Touch Panel modes
    typedef enum
    {
        TP_Auto,               ///< Auto touch detection mode
        TP_Manual,             ///< Manual touch detection mode
    } tpmode_t;
    
    /// Constructor for a display based on the RAiO RA8875 
    /// display controller.
    ///
    /// @code
    /// #include "RA8875.h"
    /// RA8875 lcd(p5, p6, p7, p12, NC, "tft");
    ///
    /// int main()
    /// {
    ///     lcd.printf("printing 3 x 2 = %d", 3*2);
    ///     lcd.circle(400,25, 25, BrightRed);
    /// }
    /// @endcode
    ///
    /// @param mosi is the SPI master out slave in pin on the mbed.
    /// @param miso is the SPI master in slave out pin on the mbed.
    /// @param sclk is the SPI shift clock pin on the mbed.
    /// @param csel is the DigitalOut pin on the mbed to use as the
    ///         active low chip select for the display controller.
    /// @param reset is the DigitalOut pin on the mbed to use as the 
    ///         active low reset input on the display controller - 
    ///         but this is not currently used.
    /// @param name is a text name for this object, which will permit
    ///         capturing stdout and printf() directly to it.
    ///
    RA8875(PinName mosi, PinName miso, PinName sclk, PinName csel, PinName reset, const char * name = "lcd");
    
    // Destructor doesn't have much to do as this would typically be created
    // at startup, and not at runtime.
    //~RA8875();
    
    /// Select the drawing layer for subsequent commands.
    ///
    /// If the screen configuration is 480 x 272, or if it is 800 x 480 
    /// and 8-bit color, the the display supports two layers, which can 
    /// be independently drawn on and shown. Additionally, complex
    /// operations involving both layers are permitted.
    ///
    /// @code
    ///     //lcd.SetLayerMode(OnlyLayer0); // default is layer 0
    ///     lcd.rect(400,130, 475,155,Brown);
    ///     lcd.SelectDrawingLayer(1);
    ///     lcd.circle(400,25, 25, BrightRed);
    ///     wait(1);
    ///     lcd.SetLayerMode(ShowLayer1);
    /// @endcode
    ///
    /// @note The user manual refers to Layer 1 and Layer 2, however the
    ///     actual register values are value 0 and 1. This and other APIs
    ///     that reference the layers use the values 0 and 1.
    ///
    /// @param layer selects the layer for subsequence commands, where
    ///     the values 0 and 1 represent layers 1 and 2 (as referred
    ///     to in the user manual).
    /// @returns success/failure code. @see RetCode_t.
    ///
    RetCode_t SelectDrawingLayer(uint16_t layer);
    
    /// Set the Layer presentation mode.
    ///
    /// This sets the presentation mode for layers, and permits showing
    /// a single layer, or applying a mode where the two layers
    /// are combined using one of the hardware methods.
    ///
    /// @code
    ///     //lcd.SetLayerMode(OnlyLayer0); // default is layer 0
    ///     lcd.rect(400,130, 475,155,Brown);
    ///     lcd.SelectDrawingLayer(1);
    ///     lcd.circle(400,25, 25, BrightRed);
    ///     wait(1);
    ///     lcd.SetLayerMode(ShowLayer1);
    /// @endcode
    ///
    /// @param mode sets the mode in the Layer Transparency Register.
    /// @returns success/failure code. @see RetCode_t.
    ///
    RetCode_t SetLayerMode(LayerMode_T mode);
    
    /// Set the layer transparency for each layer.
    ///
    /// Set the transparency, where the range of values is
    /// from zero (fully visible) to eight (fully transparent).
    /// The input value is automatically limited to this range.
    ///
    /// @code
    ///     // draw something on each layer, then step-fade across
    ///     display.SetLayerMode(RA8875::TransparentMode);
    ///     for (i=0; i<=8; i++) {
    ///         display.SetLayerTransparency(i, 8-i);
    ///         wait_ms(200);
    ///     }
    /// @endcode
    ///
    /// @param layer1 sets the layer 1 transparency.
    /// @param layer2 sets the layer 2 transparency.
    /// @returns success/failure code. @see RetCode_t.
    /// 
    RetCode_t SetLayerTransparency(uint8_t layer1, uint8_t layer2);
    
    /// Set the background color register used for transparency.
    ///
    /// This command sets the background color registers that are used
    /// in the transparent color operations involving the layers.
    /// 
    /// @param color is optional and expressed in 16-bit format. If not
    ///     supplied, a default of Black is used.
    /// @returns success/failure code. @see RetCode_t.
    ///
    RetCode_t SetBackgroundTransparencyColor(color_t color = RGB(0,0,0));
 
    /// Initialize theTouch Panel controller with default values 
    ///
    /// @returns success/failure code. @see RetCode_t.
    ///
    RetCode_t TouchPanelInit(void);
        
    /// Initialize the Touch Panel controller with detailed settings.
    ///
    /// @param[in]  bTpEnable           Touch Panel enable/disable control:
    ///                                 - TP_ENABLE: enable the touch panel
    ///                                 - TP_DISABLE: disable the touch panel
    /// @param[in]  bTpAutoManual       Touch Panel operating mode:
    ///                                 - TP_MODE_AUTO: automatic capture
    ///                                 - TP_MODE_MANUAL: manual capture
    /// @param[in]  bTpDebounce         Debounce circuit enable for touch panel interrupt:
    ///                                 - TP_DEBOUNCE_OFF: disable the debounce circuit
    ///                                 - TP_DEBOUNCE_ON: enable the debounce circuit     
    /// @param[in]  bTpManualMode       When Manual Mode is selected, this sets the mode:
    ///                                 - TP_MANUAL_IDLE: touch panel is idle   
    ///                                 - TP_MANUAL_WAIT: wait for touch panel event   
    ///                                 - TP_MANUAL_LATCH_X: latch X data  
    ///                                 - TP_MANUAL_LATCH_Y: latch Y data   
    /// @param[in]  bTpAdcClkDiv        Sets the ADC clock as a fraction of the System CLK:
    ///                                 - TP_ADC_CLKDIV_1: Use CLK   
    ///                                 - TP_ADC_CLKDIV_2: Use CLK/2   
    ///                                 - TP_ADC_CLKDIV_4: Use CLK/4   
    ///                                 - TP_ADC_CLKDIV_8: Use CLK/8   
    ///                                 - TP_ADC_CLKDIV_16: Use CLK/16   
    ///                                 - TP_ADC_CLKDIV_32: Use CLK/32   
    ///                                 - TP_ADC_CLKDIV_64: Use CLK/64   
    ///                                 - TP_ADC_CLKDIV_128: Use CLK/128   
    /// @param[in]  bTpAdcSampleTime    Touch Panel sample time delay before ADC data is ready:
    ///                                 - TP_ADC_SAMPLE_512_CLKS: Wait 512 system clocks   
    ///                                 - TP_ADC_SAMPLE_1024_CLKS: Wait 1024 system clocks   
    ///                                 - TP_ADC_SAMPLE_2048_CLKS: Wait 2048 system clocks   
    ///                                 - TP_ADC_SAMPLE_4096_CLKS: Wait 4096 system clocks   
    ///                                 - TP_ADC_SAMPLE_8192_CLKS: Wait 8192 system clocks   
    ///                                 - TP_ADC_SAMPLE_16384_CLKS: Wait 16384 system clocks   
    ///                                 - TP_ADC_SAMPLE_32768_CLKS: Wait 32768 system clocks   
    ///                                 - TP_ADC_SAMPLE_65536_CLKS: Wait 65536 system clocks
    /// @returns success/failure code. @see RetCode_t.   
    ///
    RetCode_t TouchPanelInit(uint8_t bTpEnable, uint8_t bTpAutoManual, uint8_t bTpDebounce, uint8_t bTpManualMode, uint8_t bTpAdcClkDiv, uint8_t bTpAdcSampleTime);
    
    /// Poll the TouchPanel and on a touch event return the filtered x, y coordinates.
    ///
    /// @param[inout] x is the x position where the touch was registered.
    /// @param[inout] y is the y position where the touch was registered.
    /// @returns true if touch was detected, in which case the x and y values were set.
    ///
    unsigned char TouchPanelRead(loc_t *x, loc_t *y);

    /// Poll the TouchPanel and on a touch event return the raw x, y coordinates.
    ///
    /// @param[inout] x is the x position where the touch was registered.
    /// @param[inout] y is the y position where the touch was registered.
    /// @returns true if touch was detected, in which case the x and y values were set.
    ///
    unsigned char TouchPanelReadRaw(loc_t *x, loc_t *y);
    
    /// Append interrupt handler for specific RA8875 interrupt source
    ///
    /// @param[in]    bISRType        Interrupt Source, should be:
    ///                                - RA8875_INT_KEYSCAN: KEYCAN interrupt
    ///                                - RA8875_INT_DMA: DMA interrupt
    ///                                - RA8875_INT_TP: Touch panel interrupt
    ///                                - RA8875_INT_BTE: BTE process complete interrupt
    ///                                - RA8875_INT_BTEMCU_FONTWR: Multi-purpose interrupt (see spec sheet)   
    /// @param[in]    fptr is a callback function to handle the interrupt event.
    /// @returns       none
    ///
    void AppendISR(uint8_t bISRType, void(*fptr)(void));

    /// Unappend interrupt handler for specific RA8875 interrupt source
    ///
    /// @param[in]    bISRType        Interrupt Source, should be:
    ///                                - RA8875_INT_KEYSCAN: KEYCAN interrupt
    ///                                - RA8875_INT_DMA: DMA interrupt
    ///                                - RA8875_INT_TP: Touch panel interrupt
    ///                                - RA8875_INT_BTE: BTE process complete interrupt
    ///                                - RA8875_INT_BTEMCU_FONTWR: Multi-purpose interrupt (see spec sheet)   
    /// @return       none
    ///
    void UnAppendISR(uint8_t bISRType);

    /// Write a command to the display with a word of data.
    ///
    /// This is a high level command, and may invoke several primitives.
    ///
    /// @param command is the command to write.
    /// @param data is data to be written to the command register.
    /// @returns success/failure code. @see RetCode_t.
    ///
    RetCode_t WriteCommandW(uint8_t command, uint16_t data);

    /// Write a command to the display
    ///
    /// This is a high level command, and may invoke several primitives.
    ///
    /// @param command is the command to write.
    /// @param data is optional data to be written to the command register
    ///     and only occurs if the data is in the range [0 - 0xFF].
    /// @returns success/failure code. @see RetCode_t.
    ///
    virtual RetCode_t WriteCommand(unsigned char command, unsigned int data = 0xFFFF);
    
    /// Write a data word to the display
    ///
    /// This is a high level command, and may invoke several primitives.
    ///
    /// @param data is the data to write.
    /// @returns success/failure code. @see RetCode_t.
    ///
    RetCode_t WriteDataW(uint16_t data);
    
    /// Write a data byte to the display
    ///
    /// This is a high level command, and may invoke several primitives.
    ///
    /// @param data is the data to write.
    /// @returns success/failure code. @see RetCode_t.
    ///
    virtual RetCode_t WriteData(unsigned char data);
    
    /// Read a command register
    ///
    /// @param command is the command register to read.
    /// @returns the value read from the register.
    ///
    unsigned char ReadCommand(unsigned char command);
    
    /// Read a data byte from the display
    ///
    /// This is a high level command, and may invoke several primitives.
    ///
    /// @returns data that was read.
    ///
    unsigned char ReadData(void);
    
    /// Read a word from the display
    ///
    /// This is a high level command, and may invoke several primitives.
    ///
    /// @returns data that was read.
    ///
    uint16_t ReadDataW(void);

    /// Read the display status
    ///
    /// This is a high level command, and may invoke several primitives.
    ///
    /// @returns data that was read.
    ///
    unsigned char ReadStatus(void);

    /// get the width in pixels of the currently active font
    ///
    /// @returns font width in pixels.
    ///    
    dim_t fontwidth(void);
    
    /// get the height in pixels of the currently active font
    ///
    /// @returns font height in pixels.
    ///    
    dim_t fontheight(void);
    
    /// get the number of colums based on the currently active font
    ///
    /// @returns number of columns.
    ///    
    virtual int columns(void);

    /// get the number of rows based on the currently active font
    ///
    /// @returns number of rows.
    ///    
    virtual int rows(void);

    /// get the screen width in pixels
    ///
    /// @returns screen width in pixels.
    ///
    virtual dim_t width(void);

    /// get the screen height in pixels
    ///
    /// @returns screen height in pixels.
    ///
    virtual dim_t height(void);

    /// get the color depth in bits per pixel.
    ///
    /// @returns 8 or 16 only.
    ///
    virtual dim_t color_bpp(void);

    /// Set cursor position based on the current font size.
    /// 
    /// @param column is the horizontal position in character positions
    /// @param row is the vertical position in character positions
    /// @returns success/failure code. @see RetCode_t.
    ///
    virtual RetCode_t locate(textloc_t column, textloc_t row);

    /// Prepare the controller to write text to the screen by positioning
    /// the cursor.
    ///
    /// @code
    ///     lcd.SetTextCursor(100, 25);
    ///     lcd.puts("Hello");
    /// @endcode
    ///
    /// @param x is the horizontal position in pixels (from the left edge)
    /// @param y is the vertical position in pixels (from the top edge)
    /// @returns success/failure code. @see RetCode_t.
    ///
    RetCode_t SetTextCursor(loc_t x, loc_t y);

    /// Get the current cursor position in pixels.
    ///
    /// @code
    ///     point_t point = GetTextCursor();
    ///     if (point.x > 100 && point.y > 150)
    ///         //...
    /// @endcode
    ///
    /// @returns cursor position.
    ///
    point_t GetTextCursor(void);
    
    /// Get the current cursor horizontal position in pixels.
    ///
    /// @returns cursor position horizontal offset.
    ///
    loc_t GetTextCursor_X(void);

    /// Get the current cursor vertical position in pixels.
    ///
    /// @returns cursor position vertical offset.
    ///
    loc_t GetTextCursor_Y(void);

    /// Configure additional Cursor Control settings.
    ///
    /// This API lets you modify other cursor control settings; 
    /// Cursor visible/hidden, Cursor blink/normal, 
    /// Cursor I-Beam/underscore/box.
    ///
    /// @param cursor can be set to NOCURSOR (default), IBEAM,
    ///         UNDER, or BLOCK.
    /// @param blink can be set to true or false (default false)
    /// @returns success/failure code. @see RetCode_t
    ///
    RetCode_t SetTextCursorControl(cursor_t cursor = NOCURSOR, bool blink = false);
    
    /// Select the ISO 8859-X font to use next.
    ///
    /// Supported fonts: ISO 8859-1, -2, -3, -4
    ///
    /// @param font selects the font for the subsequent text rendering.
    ///
    /// @note if either hScale or vScale is outside of its permitted range,
    ///     the command is not executed.
    /// @returns success/failure code. @see RetCode_t.
    ///
    RetCode_t SetTextFont(font_t font = ISO8859_1);
    
    /// Control the font behavior.
    ///
    /// This command lets you make several modifications to any text that
    /// will be written to the screen.
    ///
    /// Options can be combined:
    /// Default:
    /// @li Full alignment disabled, 
    /// @li Font with Background color, 
    /// @li Font in normal orientiation,
    /// @li Horizontal scale x 1
    /// @li Vertical scale x 1
    /// @li alignment
    /// 
    /// @param fillit defaults to FILL, but can be NOFILL
    /// @param angle defaults to normal, but can be rotated
    /// @param hScale defaults to 1, but can be 1, 2, 3, or 4,
    ///     and scales the font size by this amount.
    /// @param vScale defaults to 1, but can be 1, 2, 3, or 4,
    ///     and scales the font size by this amount.
    /// @param alignment defaults to align_none, but can be
    ///     align_full.
    /// 
    /// @note if either hScale or vScale is outside of its permitted range,
    ///     the command is not executed.
    /// @returns success/failure code. @see RetCode_t.
    ///
    RetCode_t SetTextFontControl(fill_t fillit = FILL, 
        font_angle_t angle = normal, 
        HorizontalScale hScale = 1, 
        VerticalScale vScale = 1, 
        alignment_t alignment = align_none);
    
    /// Control the font size
    ///
    /// This command lets you set the font enlargement for both horizontal
    /// and vertical, independent of the rotation, background, and 
    /// alignment. @see SetTextFontControl.
    ///
    /// @param hScale defaults to 1, but can be 1, 2, 3, or 4,
    ///     and scales the font size by this amount.
    /// @param vScale is an optional parameter that defaults to the hScale value, 
    ///     but can be 1, 2, 3, or 4, and scales the font size by this amount.
    ///
    /// @code
    ///     lcd.SetTextFontSize(2);     // Set the font to 2x normal size
    ///     lcd.puts("Two times");
    ///     lcd.SetTextFontSize(2,3);   // Set the font to 2x Width and 3x Height
    ///     lcd.puts("2*2 3*h");
    ///     lcd.SetTextFontSize();      // Restore to normal size in both dimensions
    ///     lcd.puts("normal");
    /// @endcode
    ///
    /// @note if either hScale or vScale is outside of its permitted range,
    ///     the command is not executed.
    /// @returns success/failure code. @see RetCode_t.
    ///
    RetCode_t SetTextFontSize(HorizontalScale hScale = 1, VerticalScale vScale = -1);
    
    /// put a character on the screen.
    ///
    /// @param c is the character.
    /// @returns the character, or EOF if there is an error.
    ///
    virtual int _putc(int c);

    /// Write string of text to the display
    ///
    /// @code
    ///     lcd.puts("Test STring");
    /// @endcode
    ///
    /// @param string is the null terminated string to send to the display.
    ///
    void puts(const char * string);
    
    /// Write string of text to the display at the specified location.
    ///
    /// @code
    ///     lcd.puts(10,25, "Test STring");
    /// @endcode
    ///
    /// @param x is the horizontal position in pixels (from the left edge)
    /// @param y is the vertical position in pixels (from the top edge)
    /// @param string is the null terminated string to send to the display.
    ///
    void puts(loc_t x, loc_t y, const char * string);
    
    /// Prepare the controller to write binary data to the screen by positioning
    /// the memory cursor.
    ///
    /// @param x is the horizontal position in pixels (from the left edge)
    /// @param y is the vertical position in pixels (from the top edge)
    /// @returns success/failure code. @see RetCode_t.
    ///
    virtual RetCode_t SetGraphicsCursor(loc_t x, loc_t y);
    
    /// Prepare the controller to read binary data from the screen by positioning
    /// the memory read cursor.
    ///
    /// @param x is the horizontal position in pixels (from the left edge)
    /// @param y is the vertical position in pixels (from the top edge)
    /// @returns success/failure code. @see RetCode_t.
    ///
    virtual RetCode_t SetGraphicsCursorRead(loc_t x, loc_t y);
    
    /// Set the window, which controls where items are written to the screen.
    ///
    /// When something hits the window width, it wraps back to the left side
    /// and down a row. If the initial write is outside the window, it will
    /// be captured into the window when it crosses a boundary.
    ///
    /// @code
    ///     lcd.window(10,10, 80,80);
    ///     lcd.puts("012345678901234567890123456789012345678901234567890");
    /// @endcode
    ///
    /// @param x is the left edge in pixels.
    /// @param y is the top edge in pixels.
    /// @param width is the window width in pixels.
    /// @param height is the window height in pixels.
    /// @returns success/failure code. @see RetCode_t.
    ///
    virtual RetCode_t window(loc_t x, loc_t y, dim_t width, dim_t height);
    
    /// Clear the screen.
    ///
    /// The behavior is to clear the whole screen. @see clsw().
    ///
    /// @code
    ///     lcd.cls();
    /// @endcode
    ///
    /// @returns success/failure code. @see RetCode_t.
    ///
    virtual RetCode_t cls(void);
    
    /// Clear the screen, or clear only the active window.
    ///
    /// The default behavior is to clear the whole screen. With the optional 
    /// parameter, the action can be restricted to the active window, which
    /// can be set with the @see window method.
    ///
    /// @code
    ///     lcd.window(20,20, 40,10);
    ///     lcd.clsw();
    /// @endcode
    ///
    /// @param region is an optional parameter that defaults to FULLWINDOW
    ///         or may be set to ACTIVEWINDOW.
    /// @returns success/failure code. @see RetCode_t.
    ///
    RetCode_t clsw(RA8875::Region_t region = FULLWINDOW);

    /// Set the background color.
    ///
    /// @param color is expressed in 16-bit format.
    /// @returns success/failure code. @see RetCode_t.
    ///
    virtual RetCode_t background(color_t color);
    
    /// Set the background color.
    ///
    /// @param r is the red element of the color.
    /// @param g is the green element of the color.
    /// @param b is the blue element of the color.
    /// @returns success/failure code. @see RetCode_t.
    ///
    virtual RetCode_t background(unsigned char r, unsigned char g, unsigned char b);
    
    /// Set the foreground color.
    ///
    /// @param color is expressed in 16-bit format.
    /// @returns success/failure code. @see RetCode_t.
    ///
    virtual RetCode_t foreground(color_t color);
    
    /// Set the foreground color.
    ///
    /// @param r is the red element of the color.
    /// @param g is the green element of the color.
    /// @param b is the blue element of the color.
    /// @returns success/failure code. @see RetCode_t.
    ///
    virtual RetCode_t foreground(unsigned char r, unsigned char g, unsigned char b);
    
    /// Get the current foreground color value.
    ///
    /// @returns the current foreground color.
    ///
    color_t GetForeColor(void);
        
    /// Draw a pixel in the specified color.
    ///
    /// @note Unlike many other operations, this does not
    ///         set the forecolor!
    ///
    /// @param x is the horizontal offset to this pixel.
    /// @param y is the vertical offset to this pixel.
    /// @param color defines the color for the pixel.
    /// @returns success/failure code. @see RetCode_t.
    ///
    virtual RetCode_t pixel(loc_t x, loc_t y, color_t color);
    
    /// Draw a pixel in the current foreground color.
    ///
    /// @param x is the horizontal offset to this pixel.
    /// @param y is the veritical offset to this pixel.
    /// @returns success/failure code. @see RetCode_t.
    ///
    virtual RetCode_t pixel(loc_t x, loc_t y);
    
    /// Get a pixel from the display.
    ///
    /// @param x is the horizontal offset to this pixel.
    /// @param y is the vertical offset to this pixel.
    /// @returns the pixel. see @color_t
    ///
    virtual color_t getPixel(loc_t x, loc_t y);
    
    /// Write a stream of pixels to the display.
    ///
    /// @param p is a pointer to a color_t array to write.
    /// @param count is the number of pixels to write.
    /// @param x is the horizontal position on the display.
    /// @param y is the vertical position on the display.
    /// @returns success/failure code. @see RetCode_t.
    ///
    virtual RetCode_t pixelStream(color_t * p, uint32_t count, loc_t x, loc_t y);
    
    /// Get a stream of pixels from the display.
    ///
    /// @param p is a pointer to a color_t array to accept the stream.
    /// @param count is the number of pixels to read.
    /// @param x is the horizontal offset to this pixel.
    /// @param y is the vertical offset to this pixel.
    /// @returns success/failure code. @see RetCode_t.
    ///
    virtual RetCode_t getPixelStream(color_t * p, uint32_t count, loc_t x, loc_t y);
    
    /// Draw a line in the specified color
    ///
    /// @note As a side effect, this changes the current
    ///     foreground color for subsequent operations.
    ///
    /// @param x1 is the horizontal start of the line.
    /// @param y1 is the vertical start of the line.
    /// @param x2 is the horizontal end of the line.
    /// @param y2 is the vertical end of the line.
    /// @param color defines the foreground color.
    /// @returns success/failure code. @see RetCode_t.
    ///
    RetCode_t line(loc_t x1, loc_t y1, loc_t x2, loc_t y2, color_t color);

    /// Draw a line
    ///
    /// Draws a line using the foreground color setting.
    ///
    /// @param x1 is the horizontal start of the line.
    /// @param y1 is the vertical start of the line.
    /// @param x2 is the horizontal end of the line.
    /// @param y2 is the vertical end of the line.
    /// @returns success/failure code. @see RetCode_t.
    ///
    RetCode_t line(loc_t x1, loc_t y1, loc_t x2, loc_t y2);

    /// Draw a rectangle in the specified color
    ///
    /// @note As a side effect, this changes the current
    ///     foreground color for subsequent operations.
    ///
    /// @param x1 is the horizontal start of the line.
    /// @param y1 is the vertical start of the line.
    /// @param x2 is the horizontal end of the line.
    /// @param y2 is the vertical end of the line.
    /// @param color defines the foreground color.
    /// @param fillit is optional to FILL the rectangle. default is NOFILL.
    /// @returns success/failure code. @see RetCode_t.
    ///
    RetCode_t rect(loc_t x1, loc_t y1, loc_t x2, loc_t y2, 
        color_t color, fill_t fillit = NOFILL);

    /// Draw a filled rectangle in the specified color
    ///
    /// @note As a side effect, this changes the current
    ///     foreground color for subsequent operations.
    ///
    /// @param x1 is the horizontal start of the line.
    /// @param y1 is the vertical start of the line.
    /// @param x2 is the horizontal end of the line.
    /// @param y2 is the vertical end of the line.
    /// @param color defines the foreground color.
    /// @param fillit is optional to NOFILL the rectangle. default is FILL.
    /// @returns success/failure code. @see RetCode_t.
    ///
    virtual RetCode_t fillrect(loc_t x1, loc_t y1, loc_t x2, loc_t y2, 
        color_t color, fill_t fillit = FILL);

    /// Draw a rectangle
    ///
    /// Draws a rectangle using the foreground color setting.
    ///
    /// @param x1 is the horizontal start of the line.
    /// @param y1 is the vertical start of the line.
    /// @param x2 is the horizontal end of the line.
    /// @param y2 is the vertical end of the line.
    /// @param fillit is optional to FILL the rectangle. default is NOFILL.
    /// @returns success/failure code. @see RetCode_t.
    ///
    RetCode_t rect(loc_t x1, loc_t y1, loc_t x2, loc_t y2, 
        fill_t fillit = NOFILL);

    /// Draw a filled rectangle with rounded corners using the specified color.
    ///
    /// This draws a rounded rectangle. A numbers of checks are made on the values,
    /// and it could reduce this to drawing a line (if either x1 == x2, or y1 == y2),
    /// or a single point (x1 == x2 && y1 == y2). If the radius parameters are
    /// > 1/2 the length of that side (width or height), an error value is returned.
    ///
    /// @note As a side effect, this changes the current
    ///     foreground color for subsequent operations.
    ///
    /// @param x1 is the horizontal start of the line and must be <= x2.
    /// @param y1 is the vertical start of the line and must be <= y2.
    /// @param x2 is the horizontal end of the line and must be >= x1.
    /// @param y2 is the vertical end of the line and must be >= y1.
    /// @param radius1 defines the horizontal radius of the curved corner. Take care
    ///         that this value < 1/2 the width of the rectangle, or bad_parameter 
    ///         is returned.
    /// @param radius2 defines the vertical radius of the curved corner. Take care
    ///         that this value < 1/2 the height of the rectangle, or bad_parameter 
    ///         is returned.
    /// @param color defines the foreground color.
    /// @param fillit is optional to FILL the rectangle. default is NOFILL.
    /// @returns success/failure code. @see RetCode_t.
    ///
    RetCode_t fillroundrect(loc_t x1, loc_t y1, loc_t x2, loc_t y2, 
        dim_t radius1, dim_t radius2, color_t color, fill_t fillit = FILL);

    /// Draw a rectangle with rounded corners using the specified color.
    ///
    /// This draws a rounded rectangle. A numbers of checks are made on the values,
    /// and it could reduce this to drawing a line (if either x1 == x2, or y1 == y2),
    /// or a single point (x1 == x2 && y1 == y2). If the radius parameters are
    /// > 1/2 the length of that side (width or height), an error value is returned.
    ///
    /// @note As a side effect, this changes the current
    ///     foreground color for subsequent operations.
    ///
    /// @param x1 is the horizontal start of the line and must be <= x2.
    /// @param y1 is the vertical start of the line and must be <= y2.
    /// @param x2 is the horizontal end of the line and must be >= x1.
    /// @param y2 is the vertical end of the line and must be >= y1.
    /// @param radius1 defines the horizontal radius of the curved corner. Take care
    ///         that this value < 1/2 the width of the rectangle, or bad_parameter 
    ///         is returned.
    /// @param radius2 defines the vertical radius of the curved corner. Take care
    ///         that this value < 1/2 the height of the rectangle, or bad_parameter 
    ///         is returned.
    /// @param color defines the foreground color.
    /// @param fillit is optional to FILL the rectangle. default is NOFILL.
    /// @returns success/failure code. @see RetCode_t.
    ///
    RetCode_t roundrect(loc_t x1, loc_t y1, loc_t x2, loc_t y2, 
        dim_t radius1, dim_t radius2, color_t color, fill_t fillit = NOFILL);

    /// Draw a rectangle with rounded corners.
    ///
    /// This draws a rounded rectangle. A numbers of checks are made on the values,
    /// and it could reduce this to drawing a line (if either x1 == x2, or y1 == y2),
    /// or a single point (x1 == x2 && y1 == y2). If the radius parameters are
    /// > 1/2 the length of that side (width or height), an error value is returned.
    ///
    /// @param x1 is the horizontal start of the line and must be <= x2.
    /// @param y1 is the vertical start of the line and must be <= y2.
    /// @param x2 is the horizontal end of the line and must be >= x1.
    /// @param y2 is the vertical end of the line and must be >= y1.
    /// @param radius1 defines the horizontal radius of the curved corner. Take care
    ///         that this value < 1/2 the width of the rectangle, or bad_parameter 
    ///         is returned.
    /// @param radius2 defines the vertical radius of the curved corner. Take care
    ///         that this value < 1/2 the height of the rectangle, or bad_parameter 
    ///         is returned.
    /// @param fillit is optional to FILL the rectangle. default is NOFILL.
    /// @returns success/failure code. @see RetCode_t.
    ///
    RetCode_t roundrect(loc_t x1, loc_t y1, loc_t x2, loc_t y2, 
        dim_t radius1, dim_t radius2, fill_t fillit = NOFILL);

    /// Draw a triangle in the specified color.
    ///
    /// @note As a side effect, this changes the current
    ///     foreground color for subsequent operations.
    ///
    /// @param x1 is the horizontal for point 1.
    /// @param y1 is the vertical for point 1. 
    /// @param x2 is the horizontal for point 2.
    /// @param y2 is the vertical for point 2.
    /// @param x3 is the horizontal for point 3.
    /// @param y3 is the vertical for point 3.
    /// @param color defines the foreground color.
    /// @param fillit is optional to FILL the rectangle. default is NOFILL.
    /// @returns success/failure code. @see RetCode_t.
    ///
    RetCode_t triangle(loc_t x1, loc_t y1, loc_t x2, loc_t y2, 
        loc_t x3, loc_t y3, color_t color, fill_t fillit = NOFILL);
    
    /// Draw a filled triangle in the specified color.
    ///
    /// @note As a side effect, this changes the current
    ///     foreground color for subsequent operations.
    ///
    /// @param x1 is the horizontal for point 1.
    /// @param y1 is the vertical for point 1.
    /// @param x2 is the horizontal for point 2.
    /// @param y2 is the vertical for point 2.
    /// @param x3 is the horizontal for point 3.
    /// @param y3 is the vertical for point 3.
    /// @param color defines the foreground color.
    /// @param fillit is optional to FILL the rectangle. default is NOFILL.
    /// @returns success/failure code. @see RetCode_t.
    ///
    RetCode_t filltriangle(loc_t x1, loc_t y1, loc_t x2, loc_t y2, 
        loc_t x3, loc_t y3, color_t color, fill_t fillit = FILL);

    /// Draw a triangle
    ///
    /// Draws a triangle using the foreground color setting.
    ///
    /// @param x1 is the horizontal for point 1.
    /// @param y1 is the vertical for point 1.
    /// @param x2 is the horizontal for point 2.
    /// @param y2 is the vertical for point 2.
    /// @param x3 is the horizontal for point 3.
    /// @param y3 is the vertical for point 3.
    /// @param fillit is optional to FILL the rectangle. default is NOFILL.
    /// @returns success/failure code. @see RetCode_t.
    ///
    RetCode_t triangle(loc_t x1, loc_t y1, loc_t x2, loc_t y2, 
        loc_t x3, loc_t y3, fill_t fillit = NOFILL);
    
    /// Draw a circle using the specified color.
    ///
    /// @note As a side effect, this changes the current
    ///     foreground color for subsequent operations.
    ///
    /// @param x is the horizontal center of the circle.
    /// @param y is the vertical center of the circle.
    /// @param radius defines the size of the circle.
    /// @param color defines the foreground color.
    /// @returns success/failure code. @see RetCode_t.
    ///
    RetCode_t circle(loc_t x, loc_t y, dim_t radius, color_t color, fill_t fillit = NOFILL);

    /// Draw a filled circle using the specified color.
    ///
    /// @note As a side effect, this changes the current
    ///     foreground color for subsequent operations.
    ///
    /// @param x is the horizontal center of the circle.
    /// @param y is the vertical center of the circle.
    /// @param radius defines the size of the circle.
    /// @param color defines the foreground color.
    /// @returns success/failure code. @see RetCode_t.
    ///
    RetCode_t fillcircle(loc_t x, loc_t y, dim_t radius, color_t color, fill_t fillit = FILL);

    /// Draw a circle.
    ///
    /// Draws a circle using the foreground color setting.
    ///
    /// @param x is the horizontal center of the circle.
    /// @param y is the vertical center of the circle.
    /// @param radius defines the size of the circle.
    /// @returns success/failure code. @see RetCode_t.
    ///
    RetCode_t circle(loc_t x, loc_t y, dim_t radius, fill_t fillit = NOFILL);

    /// Draw an Ellipse using the specified color
    ///
    /// @note As a side effect, this changes the current
    ///     foreground color for subsequent operations.
    ///
    /// @param x is the horizontal center of the ellipse.
    /// @param y is the vertical center of the ellipse.
    /// @param radius1 defines the horizontal radius of the ellipse.
    /// @param radius2 defines the vertical radius of the ellipse.
    /// @param color defines the foreground color.
    /// @param fillit defines whether the circle is filled or not.
    /// @returns success/failure code. @see RetCode_t.
    ///
    RetCode_t ellipse(loc_t x, loc_t y, dim_t radius1, dim_t radius2, 
        color_t color, fill_t fillit = NOFILL);

    /// Draw a filled Ellipse using the specified color
    ///
    /// @note As a side effect, this changes the current
    ///     foreground color for subsequent operations.
    ///
    /// @param x is the horizontal center of the ellipse.
    /// @param y is the vertical center of the ellipse.
    /// @param radius1 defines the horizontal radius of the ellipse.
    /// @param radius2 defines the vertical radius of the ellipse.
    /// @param color defines the foreground color.
    /// @param fillit defines whether the circle is filled or not.
    /// @returns success/failure code. @see RetCode_t.
    ///
    RetCode_t fillellipse(loc_t x, loc_t y, dim_t radius1, dim_t radius2, 
        color_t color, fill_t fillit = FILL);

    /// Draw an Ellipse
    ///
    /// Draws it using the foreground color setting.
    ///
    /// @param x is the horizontal center of the ellipse.
    /// @param y is the vertical center of the ellipse.
    /// @param radius1 defines the horizontal radius of the ellipse.
    /// @param radius2 defines the vertical radius of the ellipse.
    /// @param fillit defines whether the circle is filled or not.
    /// @returns success/failure code. @see RetCode_t.
    ///
    RetCode_t ellipse(loc_t x, loc_t y, dim_t radius1, dim_t radius2, fill_t fillit = NOFILL);
    
    /// Control display power
    ///
    /// @param on when set to true will turn on the display, when false it is turned off.
    /// @returns success/failure code. @see RetCode_t.
    ///
    RetCode_t Power(bool on);

    /// Reset the display controller via the Software Reset interface.
    ///
    /// @returns success/failure code. @see RetCode_t.
    ///
    RetCode_t Reset(void);
    
    /// Set backlight brightness.
    ///
    /// When the built-in PWM is used to control the backlight, this 
    /// API can be used to set the brightness.
    /// 
    /// @param brightness ranges from 0 (off) to 255 (full on)
    /// @returns success/failure code. @see RetCode_t.
    ///
    RetCode_t Backlight_u8(unsigned char brightness);
    
    /// Set backlight brightness.
    ///
    /// When the built-in PWM is used to control the backlight, this 
    /// API can be used to set the brightness.
    /// 
    /// @param brightness ranges from 0.0 (off) to 1.0 (full on)
    /// @returns success/failure code. @see RetCode_t.
    ///
    RetCode_t Backlight(float brightness);

    /// Select a bitmap font (provided by the user) for all subsequent text.
    ///
    /// @note Tool to create the fonts is accessible from its creator
    ///     available at http://www.mikroe.com. 
    ///     Change the data to an array of type char[].
    ///
    /// @param font is a pointer to a specially formed font array.
    ///     This special font array has a 4-byte header, followed by 
    ///     the data:
    ///   - the number of bytes per char
    ///   - the vertical size in pixels for each character
    ///   - the horizontal size in pixels for each character
    ///   - the number of bytes per vertical line (width of the array)
    /// @returns error code.
    ///
    virtual RetCode_t set_font(const unsigned char * font = NULL);

    /// Get the RGB value for a DOS color.
    ///
    /// @param i is the color, in the range 0 to 15;
    /// @returns the RGB color of the selected index, or 0 
    ///     if the index is out of bounds.
    ///
    color_t DOSColor(int i);

    /// Get the color name (string) for a DOS color.
    ///
    /// @param i is the color, in the range 0 to 15;
    /// @returns a pointer to a string with the color name,
    ///     or NULL if the index is out of bounds.
    /// 
    const char * DOSColorNames(int i);

    /// Advanced method indicating the start of a graphics stream.
    ///
    /// This is called prior to a stream of pixel data being sent.
    /// This may cause register configuration changes in the derived
    /// class in order to prepare the hardware to accept the streaming
    /// data.
    ///
    /// Following this command, a series of @see _putp() commands can
    /// be used to send individual pixels to the screen.
    ///
    /// To conclude the graphics stream, @see _EndGraphicsStream should
    /// be callled.
    ///
    /// @returns error code.
    ///
    virtual RetCode_t _StartGraphicsStream(void);
    
    /// Advanced method to put a single color pixel to the screen.
    ///
    /// This method may be called as many times as necessary after 
    /// @see _StartGraphicsStream() is called, and it should be followed 
    /// by _EndGraphicsStream.
    ///
    /// @param pixel is a color value to be put on the screen.
    /// @returns error code.
    ///
    virtual RetCode_t _putp(color_t pixel);
    
    /// Advanced method indicating the end of a graphics stream.
    ///
    /// This is called to conclude a stream of pixel data that was sent.
    /// This may cause register configuration changes in the derived
    /// class in order to stop the hardware from accept the streaming
    /// data.
    ///
    /// @returns error code.
    ///
    virtual RetCode_t _EndGraphicsStream(void);

    /// Set the SPI port frequency (in Hz).
    ///
    /// @note attempts to call this API at runtime, with the display
    ///         already online, cause the system to lockup. 
    ///         Investigation continues.
    ///
    /// This uses the mbed SPI driver, and is therefore dependent on
    /// its capabilities. Limited tests were performed for the display
    /// in the range of 1,000,000 to 10,000,000 Hz. The display was
    /// a bit erratic above 5,000,000 Hz, so this became the default,
    /// even though it might have been the bench-level wiring that posed
    /// the limit.
    ///
    /// @returns success/failure code. @see RetCode_t.
    ///
    RetCode_t frequency(unsigned long Hz = RA8875_DEFAULT_SPI_FREQ);
    

#ifdef PERF_METRICS
    /// Clear the performance metrics to zero.
    void ClearPerformance();
    
    /// Report the performance metrics for drawing functions using
    /// the available serial channel.
    ///
    /// @param pc is the serial channel to write to.
    ///
    void ReportPerformance(Serial & pc);
#endif

// Touch Panel public macros

/* Touch Panel Enable/Disable Reg TPCR0[7] */
#define TP_ENABLE   ((uint8_t)(1<<7))
#define TP_DISABLE  ((uint8_t)(0<<7))

/* Touch Panel operating mode Reg TPCR1[6] */
#define TP_MODE_AUTO    ((uint8_t)(0<<6))   
#define TP_MODE_MANUAL  ((uint8_t)(1<<6))

/* Touch Panel debounce Reg TPCR1[2]    */
#define TP_DEBOUNCE_OFF ((uint8_t)(0<<2))
#define TP_DEBOUNCE_ON  ((uint8_t)(1<<2))

/* Touch Panel manual modes Reg TPCR1[1:0]  */
#define TP_MANUAL_IDLE      0
#define TP_MANUAL_WAIT      1
#define TP_MANUAL_LATCH_X   2
#define TP_MANUAL_LATCH_Y   3

/* Touch Panel ADC Clock modes Reg TPCR0[2:0] */
#define TP_ADC_CLKDIV_1            0
#define TP_ADC_CLKDIV_2            1        
#define TP_ADC_CLKDIV_4            2        
#define TP_ADC_CLKDIV_8            3      
#define TP_ADC_CLKDIV_16           4        
#define TP_ADC_CLKDIV_32           5        
#define TP_ADC_CLKDIV_64           6        
#define TP_ADC_CLKDIV_128          7
        

/* Touch Panel Sample Time Reg TPCR0[6:4] */
#define TP_ADC_SAMPLE_512_CLKS     ((uint8_t)(0<<4))
#define TP_ADC_SAMPLE_1024_CLKS    ((uint8_t)(1<<4))
#define TP_ADC_SAMPLE_2048_CLKS    ((uint8_t)(2<<4))
#define TP_ADC_SAMPLE_4096_CLKS    ((uint8_t)(3<<4))
#define TP_ADC_SAMPLE_8192_CLKS    ((uint8_t)(4<<4))
#define TP_ADC_SAMPLE_16384_CLKS   ((uint8_t)(5<<4))
#define TP_ADC_SAMPLE_32768_CLKS   ((uint8_t)(6<<4))
#define TP_ADC_SAMPLE_65536_CLKS   ((uint8_t)(7<<4))

/* RA8875 interrupt enable/flag/clear masks */
#define RA8875_INT_KEYSCAN          ((uint8_t)(1<<4))    /**< KEYSCAN interrupts  */
#define RA8875_INT_DMA              ((uint8_t)(1<<3))    /**< DMA interrupts  */
#define RA8875_INT_TP               ((uint8_t)(1<<2))    /**< Touch panel interrupts  */
#define RA8875_INT_BTE              ((uint8_t)(1<<1))    /**< BTE process complete interrupts  */
#define RA8875_INT_BTEMCU_FONTWR    ((uint8_t)(1<<0))    /**< BTE-MCU-R/W or Font-Write interrupts  */


private:
    /// Touch Panel register name definitions
#define TPCR0   0x70
#define TPCR1   0x71
#define TPXH    0x72
#define TPYH    0x73
#define TPXYL   0x74
#define INTC1   0xF0
#define INTC2   0xF1

    /// Specify the default settings for the Touch Panel, where different from the chip defaults
#define TP_MODE_DEFAULT             TP_MODE_AUTO
#define TP_DEBOUNCE_DEFAULT         TP_DEBOUNCE_ON
#define TP_ADC_CLKDIV_DEFAULT       TP_ADC_CLKDIV_8

#define TP_ADC_SAMPLE_DEFAULT_CLKS  TP_ADC_SAMPLE_8192_CLKS

    /// Other Touch Panel params
#define TPBUFSIZE   16       // Depth of the averaging buffers for x and y data


    /// Initialize the chip, which is normally done as part of the
    /// constructor, so not called by the user.
    ///
    /// @note This API permits configuration, however it is not [yet]
    ///     available to the end user. Be sure the parameters
    ///     are consistent with each other - see the RA8875 user
    ///     manual.
    ///
    /// @param width in pixels to configure the display for.
    /// @param height in pixels to configure the display for.
    /// @param color_bpp can be either 8 or 16, but must be consistent
    ///     with the width and height parameters.
    /// @returns success/failure code. @see RetCode_t.
    ///
    RetCode_t init(int width, int height, int color_bpp);
    
    /// Internal function to put a character using the built-in (internal) font engine
    ///
    /// @param is the character to put to the screen.
    /// @returns the character put.
    ///
    int _internal_putc(int c);
    
    /// Internal function to put a character using the external font engine
    ///
    /// @param is the character to put to the screen.
    /// @returns the character put.
    ///
    int _external_putc(int c);
    
    /// Select the peripheral to use it.
    ///
    /// @param chipsel when true will select the peripheral, and when false
    ///     will deselect the chip. This is the logical selection, and
    ///     the pin selection is the invert of this.
    /// @returns success/failure code. @see RetCode_t.
    ///
    RetCode_t select(bool chipsel);

    /// The most primitive - to write a data value to the SPI interface.
    ///
    /// @param data is the value to write.
    /// @returns a value read from the port, since SPI is often shift
    ///     in while shifting out.
    ///
    unsigned char spiwrite(unsigned char data);
    
    /// The most primitive - to read a data value to the SPI interface.
    ///
    /// This is really just a specialcase of the write command, where
    /// the value zero is written in order to read.
    ///
    /// @returns a value read from the port, since SPI is often shift
    ///     in while shifting out.
    ///
    unsigned char spiread();
    
    SPI spi;                        ///< spi port
    DigitalOut cs;                  ///< chip select pin, assumed active low
    DigitalOut res;                 ///< reset pin, assumed active low
    const unsigned char * font;     ///< reference to an external font somewhere in memory
    loc_t cursor_x, cursor_y;    ///< used for external fonts only
    
    #ifdef PERF_METRICS
    typedef enum
    {
        PRF_CLS,
        PRF_DRAWPIXEL,
        PRF_PIXELSTREAM,
        PRF_READPIXEL,
        PRF_READPIXELSTREAM,
        PRF_DRAWLINE,
        PRF_DRAWRECTANGLE,
        PRF_DRAWROUNDEDRECTANGLE,
        PRF_DRAWTRIANGLE,
        PRF_DRAWCIRCLE,
        PRF_DRAWELLIPSE,
        METRICCOUNT
    } method_e;
    unsigned long metrics[METRICCOUNT];
    void RegisterPerformance(method_e method);
    Timer performance;
    #endif
};

//}     // namespace

//using namespace SW_graphics;


#ifdef TESTENABLE
//      ______________  ______________  ______________  _______________
//     /_____   _____/ /  ___________/ /  ___________/ /_____   ______/
//          /  /      /  /            /  /                  /  /
//         /  /      /  /___         /  /__________        /  /
//        /  /      /  ____/        /__________   /       /  /
//       /  /      /  /                       /  /       /  /
//      /  /      /  /__________  ___________/  /       /  /
//     /__/      /_____________/ /_____________/       /__/

#include "WebColors.h"
#include "Arial12x12.h"
#include <algorithm>

extern "C" void mbed_reset();

/// This activates a small set of tests for the graphics library. 
///
/// Call this API and pass it the reference to the display class.
/// It will then run a series of tests. It accepts interaction via
/// stdin to switch from automatic test mode to manual, run a specific
/// test, or to exit the test mode.
///
/// @param lcd is a reference to the display class.
/// @param pc is a reference to a serial interface, typically the USB to PC.
///
void RunTestSet(RA8875 & lcd, Serial & pc);


// To enable the test code, uncomment this section, or copy the
// necessary pieces to your "main()".
//
// #include "mbed.h"
// #include "RA8875.h"
// RA8875 lcd(p5, p6, p7, p12, NC, "tft");    // MOSI, MISO, SCK, /ChipSelect, /reset, name
// Serial pc(USBTX, USBRX);
// extern "C" void mbed_reset();
// int main()
// {
//     pc.baud(460800);    // I like a snappy terminal, so crank it up!
//     pc.printf("\r\nRA8875 Test - Build " __DATE__ " " __TIME__ "\r\n");
// 
//     pc.printf("Turning on display\r\n");
//     lcd.Reset();
//     lcd.Power(true);  // display power is on, but the backlight is independent
//     lcd.Backlight(0.5);
//     RunTestSet(lcd, pc);
// }

#endif // TESTENABLE

#endif
