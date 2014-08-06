
#ifndef WEBCOLORS_H
#define WEBCOLORS_H

#include "RA8875.h"

const color_t WebColors[256] =
    {
    RGB(0x00,0x00,0x00), RGB(0x00,0x00,0x33), RGB(0x00,0x00,0x66), RGB(0x00,0x00,0x99), RGB(0x00,0x00,0xCC), RGB(0x00,0x00,0xFF), RGB(0x00,0x33,0x00), RGB(0x00,0x33,0x33), 
    RGB(0x00,0x33,0x66), RGB(0x00,0x33,0x99), RGB(0x00,0x33,0xCC), RGB(0x00,0x33,0xFF), RGB(0x00,0x66,0x00), RGB(0x00,0x66,0x33), RGB(0x00,0x66,0x66), RGB(0x00,0x66,0x99), 
    RGB(0x00,0x66,0xCC), RGB(0x00,0x66,0xFF), RGB(0x00,0x99,0x00), RGB(0x00,0x99,0x33), RGB(0x00,0x99,0x66), RGB(0x00,0x99,0x99), RGB(0x00,0x99,0xCC), RGB(0x00,0x99,0xFF), 
    RGB(0x00,0xCC,0x00), RGB(0x00,0xCC,0x33), RGB(0x00,0xCC,0x66), RGB(0x00,0xCC,0x99), RGB(0x00,0xCC,0xCC), RGB(0x00,0xCC,0xFF), RGB(0x00,0xFF,0x00), RGB(0x00,0xFF,0x33), 
    RGB(0x00,0xFF,0x66), RGB(0x00,0xFF,0x99), RGB(0x00,0xFF,0xCC), RGB(0x00,0xFF,0xFF), RGB(0x33,0x00,0x00), RGB(0x33,0x00,0x33), RGB(0x33,0x00,0x66), RGB(0x33,0x00,0x99), 
    RGB(0x33,0x00,0xCC), RGB(0x33,0x00,0xFF), RGB(0x33,0x33,0x00), RGB(0x33,0x33,0x33), RGB(0x33,0x33,0x66), RGB(0x33,0x33,0x99), RGB(0x33,0x33,0xCC), RGB(0x33,0x33,0xFF), 
    RGB(0x33,0x66,0x00), RGB(0x33,0x66,0x33), RGB(0x33,0x66,0x66), RGB(0x33,0x66,0x99), RGB(0x33,0x66,0xCC), RGB(0x33,0x66,0xFF), RGB(0x33,0x99,0x00), RGB(0x33,0x99,0x33), 
    RGB(0x33,0x99,0x66), RGB(0x33,0x99,0x99), RGB(0x33,0x99,0xCC), RGB(0x33,0x99,0xFF), RGB(0x33,0xCC,0x00), RGB(0x33,0xCC,0x33), RGB(0x33,0xCC,0x66), RGB(0x33,0xCC,0x99), 
    RGB(0x33,0xCC,0xCC), RGB(0x33,0xCC,0xFF), RGB(0x33,0xFF,0x00), RGB(0x33,0xFF,0x33), RGB(0x33,0xFF,0x66), RGB(0x33,0xFF,0x99), RGB(0x33,0xFF,0xCC), RGB(0x33,0xFF,0xFF), 
    RGB(0x66,0x00,0x00), RGB(0x66,0x00,0x33), RGB(0x66,0x00,0x66), RGB(0x66,0x00,0x99), RGB(0x66,0x00,0xCC), RGB(0x66,0x00,0xFF), RGB(0x66,0x33,0x00), RGB(0x66,0x33,0x33), 
    RGB(0x66,0x33,0x66), RGB(0x66,0x33,0x99), RGB(0x66,0x33,0xCC), RGB(0x66,0x33,0xFF), RGB(0x66,0x66,0x00), RGB(0x66,0x66,0x33), RGB(0x66,0x66,0x66), RGB(0x66,0x66,0x99), 
    RGB(0x66,0x66,0xCC), RGB(0x66,0x66,0xFF), RGB(0x66,0x99,0x00), RGB(0x66,0x99,0x33), RGB(0x66,0x99,0x66), RGB(0x66,0x99,0x99), RGB(0x66,0x99,0xCC), RGB(0x66,0x99,0xFF), 
    RGB(0x66,0xCC,0x00), RGB(0x66,0xCC,0x33), RGB(0x66,0xCC,0x66), RGB(0x66,0xCC,0x99), RGB(0x66,0xCC,0xCC), RGB(0x66,0xCC,0xFF), RGB(0x66,0xFF,0x00), RGB(0x66,0xFF,0x33), 
    RGB(0x66,0xFF,0x66), RGB(0x66,0xFF,0x99), RGB(0x66,0xFF,0xCC), RGB(0x66,0xFF,0xFF), RGB(0x99,0x00,0x00), RGB(0x99,0x00,0x33), RGB(0x99,0x00,0x66), RGB(0x99,0x00,0x99), 
    RGB(0x99,0x00,0xCC), RGB(0x99,0x00,0xFF), RGB(0x99,0x33,0x00), RGB(0x99,0x33,0x33), RGB(0x99,0x33,0x66), RGB(0x99,0x33,0x99), RGB(0x99,0x33,0xCC), RGB(0x99,0x33,0xFF), 
    RGB(0x99,0x66,0x00), RGB(0x99,0x66,0x33), RGB(0x99,0x66,0x66), RGB(0x99,0x66,0x99), RGB(0x99,0x66,0xCC), RGB(0x99,0x66,0xFF), RGB(0x99,0x99,0x00), RGB(0x99,0x99,0x33), 
    RGB(0x99,0x99,0x66), RGB(0x99,0x99,0x99), RGB(0x99,0x99,0xCC), RGB(0x99,0x99,0xFF), RGB(0x99,0xCC,0x00), RGB(0x99,0xCC,0x33), RGB(0x99,0xCC,0x66), RGB(0x99,0xCC,0x99), 
    RGB(0x99,0xCC,0xCC), RGB(0x99,0xCC,0xFF), RGB(0x99,0xFF,0x00), RGB(0x99,0xFF,0x33), RGB(0x99,0xFF,0x66), RGB(0x99,0xFF,0x99), RGB(0x99,0xFF,0xCC), RGB(0x99,0xFF,0xFF), 
    RGB(0xCC,0x00,0x00), RGB(0xCC,0x00,0x33), RGB(0xCC,0x00,0x66), RGB(0xCC,0x00,0x99), RGB(0xCC,0x00,0xCC), RGB(0xCC,0x00,0xFF), RGB(0xCC,0x33,0x00), RGB(0xCC,0x33,0x33), 
    RGB(0xCC,0x33,0x66), RGB(0xCC,0x33,0x99), RGB(0xCC,0x33,0xCC), RGB(0xCC,0x33,0xFF), RGB(0xCC,0x66,0x00), RGB(0xCC,0x66,0x33), RGB(0xCC,0x66,0x66), RGB(0xCC,0x66,0x99), 
    RGB(0xCC,0x66,0xCC), RGB(0xCC,0x66,0xFF), RGB(0xCC,0x99,0x00), RGB(0xCC,0x99,0x33), RGB(0xCC,0x99,0x66), RGB(0xCC,0x99,0x99), RGB(0xCC,0x99,0xCC), RGB(0xCC,0x99,0xFF), 
    RGB(0xCC,0xCC,0x00), RGB(0xCC,0xCC,0x33), RGB(0xCC,0xCC,0x66), RGB(0xCC,0xCC,0x99), RGB(0xCC,0xCC,0xCC), RGB(0xCC,0xCC,0xFF), RGB(0xCC,0xFF,0x00), RGB(0xCC,0xFF,0x33), 
    RGB(0xCC,0xFF,0x66), RGB(0xCC,0xFF,0x99), RGB(0xCC,0xFF,0xCC), RGB(0xCC,0xFF,0xFF), RGB(0xFF,0x00,0x00), RGB(0xFF,0x00,0x33), RGB(0xFF,0x00,0x66), RGB(0xFF,0x00,0x99), 
    RGB(0xFF,0x00,0xCC), RGB(0xFF,0x00,0xFF), RGB(0xFF,0x33,0x00), RGB(0xFF,0x33,0x33), RGB(0xFF,0x33,0x66), RGB(0xFF,0x33,0x99), RGB(0xFF,0x33,0xCC), RGB(0xFF,0x33,0xFF), 
    RGB(0xFF,0x66,0x00), RGB(0xFF,0x66,0x33), RGB(0xFF,0x66,0x66), RGB(0xFF,0x66,0x99), RGB(0xFF,0x66,0xCC), RGB(0xFF,0x66,0xFF), RGB(0xFF,0x99,0x00), RGB(0xFF,0x99,0x33), 
    RGB(0xFF,0x99,0x66), RGB(0xFF,0x99,0x99), RGB(0xFF,0x99,0xCC), RGB(0xFF,0x99,0xFF), RGB(0xFF,0xCC,0x00), RGB(0xFF,0xCC,0x33), RGB(0xFF,0xCC,0x66), RGB(0xFF,0xCC,0x99), 
    RGB(0xFF,0xCC,0xCC), RGB(0xFF,0xCC,0xFF), RGB(0xFF,0xFF,0x00), RGB(0xFF,0xFF,0x33), RGB(0xFF,0xFF,0x66), RGB(0xFF,0xFF,0x99), RGB(0xFF,0xFF,0xCC), RGB(0xFF,0xFF,0xFF)
    };

#endif // WEBCOLORS_H