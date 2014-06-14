#ifndef DISPLAYDEFS_H
#define DISPLAYDEFS_H

#define RGB(r,g,b) ( ((r<<8)&0xF800) | ((g<<3)&0x07E0) | (b>>3) )

typedef uint16_t color_t;

/// return values from functions
typedef enum
{
    noerror,                ///< no errors, command completed successfully
    bad_parameter,          ///< one or more parameters are invalid
    file_not_found,         ///< specified file could not be found
    not_bmp_format,         ///< file is not a .bmp file
    not_ico_format,         ///< file is not a .ico file
    not_supported_format,   ///< file format is not yet supported
    image_too_big,          ///< image is too large for the screen
    not_enough_ram,         ///< could not allocate ram for scanline
} RetCode_t;

/// type that manages locations, which is typically an x or y pixel location,
/// which can range from -N to +N (even if the screen is 0 to +n). @see textloc_t.
typedef int16_t loc_t;

/// type that manages text locations, which are row or column values in
/// units of character, not pixel. @see loc_t.
typedef uint16_t textloc_t;

/// type that manages dimensions of width or height, which range from 0 to N.
typedef uint16_t dim_t;

/// type that manages x,y pairs
typedef struct
{
    loc_t x;             ///< x value in the point
    loc_t y;             ///< y value in the point
} point_t;

/// color type definition to let the compiler help keep us honest.
/// 
/// colors can be defined with the RGB(r,g,b) macro, and there
/// are a number of predefined colors:
/// - Black,    Blue,       Green,       Cyan,
/// - Red,      Magenta,    Brown,       Gray,
/// - Charcoal, BrightBlue, BrightGreen, BrightCyan,
/// - Orange,   Pink,       Yellow,      White
///
typedef uint16_t color_t;   

/// background fill info for drawing Text, Rectangles, RoundedRectanges, Circles, Ellipses and Triangles.
typedef enum
{
    NOFILL,     ///< do not fill the object with the background color
    FILL        ///< fill the object space with the background color
} fill_t;

#endif // DISPLAYDEFS_H
