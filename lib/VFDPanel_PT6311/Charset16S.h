/*
 PT6311.h - Driver for PT6311 VFD Controller
 
 Copyright (c) 2014 Dmitry Pakhomenko.
 dmitryp@magictale.com
 http://magictale.com
 
 This example code is in the public domain.
 */

#ifndef CHARSET16S_H
#define CHARSET16S_H

#include "settings.h"


//Done for the top left digit, the remaining digits with 
//different mapping will be converted from this 'base' value
const PROGMEM uint8_t FONT_PGM[63][1] = {
    {0x00}, // space 0x20
    {0x00}, // ! N/A
    {0x00}, // " N/A
    {0x00}, // # N/A
    {0x00}, // $ N/A
    {0x00}, // % N/A
    {0x00}, // & N/A
    {0x00}, // ' N/A
    {0x53}, // ( (1+2+10+40)
    {0x65}, // ) (1+4+20+40)
    {0x00}, // * N/A
    {0x00}, // + N/A
    {0x00}, // , N/A
    {0x08}, // - (8)
    {0x00}, // . N/A
    {0x1C}, // / (4+8+10) 

    {0x77}, // 0 (1+2+4+10+20+40)
    {0x24}, // 1 (4+20)
    {0x5D}, // 2 (1+4+8+10+40)
    {0x6D}, // 3 (1+4+8+20+40)
    {0x2E}, // 4 (2+4+8+20)
    {0x6B}, // 5 (1+2+8+20+40)
    {0x7B}, // 6 (1+2+8+10+20+40)
    {0x25}, // 7 (1+4+20)
    {0x7F}, // 8 (1+2+4+8+10+20+40)
    {0x6F}, // 9 (1+2+4+8+20+40)

    {0x00}, // : (bits ?) available on positions 1 & 3 at 0x01
    {0x00}, // ; N/A
    {0x00}, // < N/A
    {0x48}, // = (8+40)
    {0x00}, // > N/A
    {0x00}, // ? N/A
    {0x00}, // @ N/A
    {0x3F}, // A (1+2+4+8+10+20)
    {0x7A}, // b (2+8+10+20+40)
    {0x58}, // c (8+10+40)
    {0x7C}, // d (4+8+10+20+40)
    {0x5B}, // E (1+2+8+10+40)
    {0x1B}, // F (1+2+8+10)
    {0x6F}, // g (1+2+4+8+20+40)
    {0x3E}, // H (2+4+8+10+20)
    {0x24}, // I (4+20)
    {0x64}, // j (4+20+40)
    {0x00}, // K N/A
    {0x52}, // L (2+10+40)
    {0x00}, // M N/A
    {0x38}, // n (8+10+20)
    {0x78}, // o (8+10+20+40)
    {0x1F}, // P (1+2+4+8+10)
    {0x2F}, // q (1+2+4+8+20)
    {0x18}, // r (8+10)
    {0x6B}, // S (1+2+8+20+40)
    {0x5A}, // t (2+8+10+40)
    {0x70}, // u (10+20+40)
    {0x00}, // V N/A
    {0x00}, // W N/A
    {0x3E}, // X (2+4+8+10+20)
    {0x6E}, // y (2+4+8+20+40)
    {0x5D}, // Z (1+4+8+10+40)
    {0x53}, // [ (1+2+10+40)
    {0x2A}, // \ (2+8+20)
    {0x65}, // ] (1+4+20+40)     
    //Special non-standard symbols
    {0x0F}  // degree (1+2+4+8)
}; // DEL


#endif

