/* mbed GraphicsDisplay Display Library Base Class
 * Copyright (c) 2007-2009 sford
 * Released under the MIT License: http://mbed.org/license/mit
 *
 * A library for providing a common base class for Graphics displays
 * To port a new display, derive from this class and implement
 * the constructor (setup the display), pixel (put a pixel
 * at a location), width and height functions. Everything else
 * (locate, printf, putc, cls, window, putp, fill, blit, blitbit) 
 * will come for free. You can also provide a specialised implementation
 * of window and putp to speed up the results
 */

#ifndef MBED_GRAPHICSDISPLAY_H
#define MBED_GRAPHICSDISPLAY_H
#include "Bitmap.h"
#include "TextDisplay.h"

// GraphicsDisplay has one "soft font" which is in a different format
// then the primary font rendering api - see set_font(...). This is
// therefore deprecated, but preserved for a time for backward 
// compatibility.
// #define LOCALFONT


/// The GraphicsDisplay class 
/// 
/// This graphics display class supports both graphics and text operations.
/// Typically, a subclass is derived from this which has localizations to
/// adapt to a specific hardware platform (e.g. a display controller chip),
/// that overrides methods in here to either add more capability or perhaps 
/// to improve performance, by leveraging specific hardware capabilities.
///
class GraphicsDisplay : public TextDisplay 
{
public:
    /// The constructor
    GraphicsDisplay(const char* name);
    
    /// Draw a pixel in the specified color.
    ///
    /// @note this method must be supported in the derived class.
    ///
    /// @param x is the horizontal offset to this pixel.
    /// @param y is the vertical offset to this pixel.
    /// @param color defines the color for the pixel.
    /// @returns success/failure code. @see RetCode_t.
    ///
    virtual RetCode_t pixel(loc_t x, loc_t y, color_t color) = 0;
    
    /// Write a stream of pixels to the display.
    ///
    /// @note this method must be supported in the derived class.
    ///
    /// @param p is a pointer to a color_t array to write.
    /// @param count is the number of pixels to write.
    /// @param x is the horizontal position on the display.
    /// @param y is the vertical position on the display.
    /// @returns success/failure code. @see RetCode_t.
    ///
    virtual RetCode_t pixelStream(color_t * p, uint32_t count, loc_t x, loc_t y) = 0;

    /// Get a pixel from the display.
    ///
    /// @note this method must be supported in the derived class.
    ///
    /// @param x is the horizontal offset to this pixel.
    /// @param y is the vertical offset to this pixel.
    /// @returns the pixel. see @color_t
    ///
    virtual color_t getPixel(loc_t x, loc_t y) = 0;

    /// Get a stream of pixels from the display.
    ///
    /// @note this method must be supported in the derived class.
    ///
    /// @param p is a pointer to a color_t array to accept the stream.
    /// @param count is the number of pixels to read.
    /// @param x is the horizontal offset to this pixel.
    /// @param y is the vertical offset to this pixel.
    /// @returns success/failure code. @see RetCode_t.
    ///
    virtual RetCode_t getPixelStream(color_t * p, uint32_t count, loc_t x, loc_t y) = 0;
    
    /// get the screen width in pixels
    ///
    /// @note this method must be supported in the derived class.
    ///
    /// @returns screen width in pixels.
    ///
    virtual uint16_t width() = 0;
    
    /// get the screen height in pixels
    ///
    /// @note this method must be supported in the derived class.
    ///
    /// @returns screen height in pixels.
    ///
    virtual uint16_t height() = 0;

    /// Prepare the controller to write binary data to the screen by positioning
    /// the memory cursor.
    ///
    /// @note this method must be supported in the derived class.
    ///
    /// @param x is the horizontal position in pixels (from the left edge)
    /// @param y is the vertical position in pixels (from the top edge)
    /// @returns success/failure code. @see RetCode_t.
    ///
    virtual RetCode_t SetGraphicsCursor(loc_t x, loc_t y) = 0;
    
    /// Prepare the controller to read binary data from the screen by positioning
    /// the memory read cursor.
    ///
    /// @param x is the horizontal position in pixels (from the left edge)
    /// @param y is the vertical position in pixels (from the top edge)
    /// @returns success/failure code. @see RetCode_t.
    ///
    virtual RetCode_t SetGraphicsCursorRead(loc_t x, loc_t y) = 0;
    
    /// Draw a filled rectangle in the specified color
    ///
    /// @note As a side effect, this changes the current
    ///     foreground color for subsequent operations.
    ///
    /// @note this method must be supported in the derived class.
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
        color_t color, fill_t fillit = FILL) = 0;


    virtual RetCode_t WriteCommand(unsigned char command, unsigned int data = 0xFFFF) = 0;
    virtual RetCode_t WriteData(unsigned char data) = 0;

    /// Set the window, which controls where items are written to the screen.
    ///
    /// When something hits the window width, it wraps back to the left side
    /// and down a row. If the initial write is outside the window, it will
    /// be captured into the window when it crosses a boundary.
    ///
    /// @param x is the left edge in pixels.
    /// @param y is the top edge in pixels.
    /// @param w is the window width in pixels.
    /// @param h is the window height in pixels.
    /// @returns success/failure code. @see RetCode_t.
    ///
    virtual RetCode_t window(loc_t x, loc_t y, dim_t w, dim_t h);
    
    /// Clear the screen.
    ///
    /// The behavior is to clear the whole screen.
    ///
    /// @returns success/failure code. @see RetCode_t.
    ///
    virtual RetCode_t cls();
    

    virtual void WindowMax(void);
    
    /// method to put a single color pixel to the screen.
    ///
    /// This method may be called as many times as necessary after 
    /// @see _StartGraphicsStream() is called, and it should be followed 
    /// by _EndGraphicsStream.
    ///
    /// @param pixel is a color value to be put on the screen.
    /// @returns error code.
    ///
    virtual RetCode_t _putp(color_t pixel);

            
    virtual void fill(int x, int y, int w, int h, color_t color);
    virtual void blit(int x, int y, int w, int h, const int * color);    
    
    /// This method transfers one character from the external font data
    /// to the screen.
    ///
    /// @note the font data is in a special format as generate by
    ///         the mikroe font creator. \\
    ///         See http://www.mikroe.com/glcd-font-creator/
    ///
    /// @param x is the horizontal pixel coordinate
    /// @param y is the vertical pixel coordinate
    /// @param fontTable is the base of the table which has the metrics
    /// @param fontChar is the start of that record in the table for the char (e.g. 'A' - 'Z')
    /// @returns how far the cursor should advance to the right in pixels
    ///
    virtual int fontblit(int x, int y, const unsigned char * fontTable, const unsigned char * fontChar);
    
    /// This method returns the color value from a palette.
    ///
    /// This method accepts a pointer to a Bitmap color palette, which
    /// is a table in memory composed of RGB Quad values (r, g, b, 0),
    /// and an index into that table. It then extracts the color information
    /// and downsamples it to a color_t value which it returns.
    ///
    /// @note This method probably has very little value outside of
    ///         the internal methods for reading BMP files.
    ///
    /// @param colorPalette is the handle to the color palette to use.
    /// @param i is the index into the color palette.
    /// @returns the color in color_t format.
    ///
    color_t RGBQuadToRGB16(RGBQUAD * colorPalette, uint16_t i);
    
    /// This method converts a 16-bit color value into a 24-bit RGB Quad.
    ///
    /// @param c is the 16-bit color. @see color_t
    /// @returns an RGBQUAD value. @see RGBQUAD
    ///
    RGBQUAD RGB16ToRGBQuad(color_t c);

    /// This method attempts to render a specified graphics image file at
    /// the specified screen location.
    ///
    /// This supports several variants of the following file types:
    /// \li Bitmap file format,
    /// \li Icon file format.
    ///
    /// @note The specified image width and height, when adjusted for the 
    ///     x and y origin, must fit on the screen, or the image will not
    ///     be shown (it does not clip the image).
    ///
    /// @note The file extension is tested, and if it ends in a supported
    ///     format, the appropriate handler is called to render that image.
    ///
    /// @param x is the horizontal pixel coordinate
    /// @param y is the vertical pixel coordinate
    /// @param FileName refers to the fully qualified path and file on 
    ///     a mounted file system.
    /// @returns success or error code.
    ///
    RetCode_t RenderImageFile(loc_t x, loc_t y, const char *FileName);

    /// This method reads a disk file that is in bitmap format and 
    /// puts it on the screen.
    ///
    /// Supported formats:
    /// \li 4-bit color format (16 colors)
    /// \li 8-bit color format (256 colors)
    /// \li 16-bit color format (65k colors)
    /// \li compression: no.
    ///
    /// @note This is a slow operation, typically due to the use of
    ///         the file system, and partially because bmp files
    ///         are stored from the bottom up, and the memory is written
    ///         from the top down; as a result, it constantly 'seeks'
    ///         on the file system for the next row of information.
    ///
    /// As a performance test, a sample picture was timed. A family picture
    /// was converted to Bitmap format; shrunk to 352 x 272 pixels and save
    /// in 8-bit color format. The resulting file size was 94.5 KByte.
    /// The SPI port interface was set to 20 MHz.
    /// The original bitmap rendering software was purely in software, 
    /// pushing 1 pixel at a time to the write function, which did use SPI
    /// hardware (not pin wiggling) to transfer commands and data to the 
    /// display. Then, the driver was improved to leverage the capability
    /// of the derived display driver. As a final check, instead of the
    /// [known slow] local file system, a randomly chosen USB stick was 
    /// used. The performance results are impressive (but depend on the
    /// listed factors). 
    ///
    /// \li 34 seconds, LocalFileSystem, Software Rendering
    /// \li 9 seconds, LocalFileSystem, Hardware Rending for RA8875
    /// \li 3 seconds, MSCFileSystem, Hardware Rendering for RA8875
    /// 
    /// @param x is the horizontal pixel coordinate
    /// @param y is the vertical pixel coordinate
    /// @param Name_BMP is the filename on the mounted file system.
    /// @returns success or error code.
    ///
    RetCode_t RenderBitmapFile(loc_t x, loc_t y, const char *Name_BMP);
    
    
    /// This method reads a disk file that is in ico format and 
    /// puts it on the screen.
    ///
    /// Reading the disk is slow, but a typical icon file is small
    /// so it should be ok.
    ///
    /// @note An Icon file can have more than one icon in it. This
    ///     implementation only processes the first image in the file.
    ///
    /// @param x is the horizontal pixel coordinate
    /// @param y is the vertical pixel coordinate
    /// @param Name_ICO is the filename on the mounted file system.
    /// @returns success or error code.
    ///
    RetCode_t RenderIconFile(loc_t x, loc_t y, const char *Name_ICO);

    
    /// This method captures the specified area as a 24-bit bitmap file.
    ///
    /// Even though this is a 16-bit display, the stored image is in
    /// 24-bit format.
    ///
    /// @param x is the left edge of the region to capture
    /// @param y is the top edge of the region to capture
    /// @param w is the width of the region to capture
    /// @param h is the height of the region to capture.
    /// @return success or error code.
    ///
    RetCode_t PrintScreen(loc_t x, loc_t y, dim_t w, dim_t h, const char *Name_BMP);
    
    /// prints one character at the specified coordinates.
    ///
    /// This will print the character at the specified pixel coordinates.
    ///
    /// @param x is the horizontal offset in pixels.
    /// @param y is the vertical offset in pixels.
    /// @param value is the character to print.
    /// @returns number of pixels to index to the right if a character was printed, 0 otherwise.
    ///
    virtual int character(int x, int y, int value);
    
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
    
    /// Select a bitmap font (provided by the user) for all subsequent text
    /// rendering.
    ///
    /// This API permits selection of a special memory mapped font, which 
    /// enables the presentation of many font sizes and styles, including
    /// proportional fonts.
    ///
    /// @note Tool to create the fonts is accessible from its creator
    ///     available at http://www.mikroe.com.
    ///     Hint: Change the data to an array of type char[].
    ///
    ///     This special font array has a 4-byte header, followed by 
    ///     the data:
    ///   \li the number of bytes per char
    ///   \li the vertical size in pixels for each character
    ///   \li the horizontal size in pixels for each character
    ///   \li the number of bytes per vertical line (width of the array)
    ///   \li the subsequent records are the font information.
    ///
    /// @param font is a pointer to a specially formed font array. 
    ///     NULL, or the omission of this parameter will restore the default
    ///     font capability, which may use the display controllers hardware
    ///     font (if available), or no font.
    /// @returns error code.
    ///
    virtual RetCode_t set_font(const unsigned char * font = NULL);

protected:

    /// Pure virtual method indicating the start of a graphics stream.
    ///
    /// This is called prior to a stream of pixel data being sent.
    /// This may cause register configuration changes in the derived
    /// class in order to prepare the hardware to accept the streaming
    /// data.
    ///
    /// @note this method must be supported in the derived class.
    ///
    /// @returns error code.
    ///
    virtual RetCode_t _StartGraphicsStream(void) = 0;
    
    /// Pure virtual method indicating the end of a graphics stream.
    ///
    /// This is called to conclude a stream of pixel data that was sent.
    /// This may cause register configuration changes in the derived
    /// class in order to stop the hardware from accept the streaming
    /// data.
    ///
    /// @note this method must be supported in the derived class.
    ///
    /// @returns error code.
    ///
    virtual RetCode_t _EndGraphicsStream(void) = 0;

    /// Protected method to render an image given a file handle and 
    /// coordinates.
    ///
    /// @param x is the horizontal pixel coordinate
    /// @param y is the vertical pixel coordinate
    /// @param w is the image width restriction, or zero to permit full image width.
    /// @param h is the image height restriction, or zero to permit full image height.
    /// @param fileOffset is the offset into the file where the image data starts
    /// @param Image is the filename stream already opened for the data.
    /// @returns success or error code.
    ///
    RetCode_t _RenderBitmap(loc_t x, loc_t y, uint32_t fileOffset, FILE * Image);

    #ifdef LOCALFONT
    virtual int blitbit(int x, int y, int w, int h, const char * color);
    #endif
    
    const unsigned char * font;     ///< reference to an external font somewhere in memory
    
    // pixel location
    short _x;
    short _y;
    
    // window location
    short _x1;
    short _x2;
    short _y1;
    short _y2;
};

#endif

