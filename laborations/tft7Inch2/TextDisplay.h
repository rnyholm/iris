/* mbed TextDisplay Library Base Class
 * Copyright (c) 2007-2009 sford
 * Released under the MIT License: http://mbed.org/license/mit
 *
 * A common base class for Text displays
 * To port a new display, derive from this class and implement
 * the constructor (setup the display), character (put a character
 * at a location), rows and columns (number of rows/cols) functions.
 * Everything else (locate, printf, putc, cls) will come for free
 *
 * The model is the display will wrap at the right and bottom, so you can
 * keep writing and will always get valid characters. The location is
 * maintained internally to the class to make this easy
 */

#ifndef MBED_TEXTDISPLAY_H
#define MBED_TEXTDISPLAY_H

#include "mbed.h"

#include "DisplayDefs.h"

/// A text display class that supports character based
/// presentation.
///
class TextDisplay : public Stream
{
public:

    // functions needing implementation in derived implementation class
    /// Create a TextDisplay interface
    ///
    /// @param name The name used in the path to access the display through 
    ///     the stdio stream.
    ///
    TextDisplay(const char *name = NULL);

    /// output a character at the given position
    ///
    /// @note this method may be overridden in a derived class.
    ///
    /// @param x position in pixels
    /// @param y position in pixels
    /// @param c the character to be written to the TextDisplay
    /// @returns number of pixels to advance the cursor which could be the cell width
    ///     for non-proportional characters, or the actual character width for
    ///     proportional characters.
    ///
    virtual int character(int x, int y, int c) = 0;

    /// return number of rows on TextDisplay
    ///
    /// @note this method may be overridden in a derived class.
    ///
    /// @returns number of text rows for the display for the currently 
    ///     active font.
    ///
    virtual int rows() = 0;

    /// return number if columns on TextDisplay
    ///
    /// @note this method may be overridden in a derived class.
    ///
    /// @returns number of text rows for the display for the currently
    ///     active font.
    ///
    virtual int columns() = 0;

    // functions that come for free, but can be overwritten

    /// redirect output from a stream (stoud, sterr) to  display
    ///
    /// @note this method may be overridden in a derived class.
    ///
    /// @param stream that shall be redirected to the TextDisplay
    /// @returns true if the claim succeeded.
    ///
    virtual bool claim (FILE *stream);

    /// clear screen
    ///
    /// @note this method may be overridden in a derived class.
    ///
    /// @returns error code.
    ///
    virtual RetCode_t cls() = 0;
    
    /// locate the cursor at a character position.
    ///
    /// Based on the currently active font, locate the cursor on screen.
    ///
    /// @note this method may be overridden in a derived class.
    ///
    /// @param column is the horizontal offset from the left side.
    /// @param row is the vertical offset from the top.
    /// @returns error code.
    ///
    virtual RetCode_t locate(textloc_t column, textloc_t row) = 0;
    
    /// set the foreground color
    ///
    /// @note this method may be overridden in a derived class.
    ///
    /// @param color is color to use for foreground drawing.
    /// @returns error code.
    ///
    virtual RetCode_t foreground(color_t color) = 0;

    /// set the background color
    ///
    /// @note this method may be overridden in a derived class.
    ///
    /// @param color is color to use for background drawing.
    /// @returns error code.
    ///
    virtual RetCode_t background(color_t color) = 0;
    // putc (from Stream)
    // printf (from Stream)

protected:    
    virtual int _putc(int value);
    virtual int _getc();

    // character location
    uint16_t _column;
    uint16_t _row;

    // colors
    color_t _foreground;
    color_t _background;
    char *_path;
};

#endif
