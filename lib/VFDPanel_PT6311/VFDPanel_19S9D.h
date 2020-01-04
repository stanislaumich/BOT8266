/*
 PT6311.h - Driver for PT6311 VFD Controller
 
 Copyright (c) 2014 Dmitry Pakhomenko.
 dmitryp@magictale.com
 http://magictale.com
 
 This example code is in the public domain.
 */

#ifndef MVFD_19S9D_H
#define MVFD_19S9D_H

#include "Settings.h"
#include "PT6311.h"
#include "Charset16S.h"

#define VFD_SEGMENTS            19
#define VFD_DIGITS              9
#define VFD_BYTES_PER_DIGIT     3
#define VFD_FIRST_DISP_CHAR     0x20
#define VFD_FLASH_MODE_BIT      15
#define VFD_MAX_COLUMNS 14
#define VFD_MAX_SPECIAL_SYMBOLS 13
#define VFD_MAX_FLAME_STATES    6
#define VFD_FLAME_ADDR          12
#define VFD_TIMER1_IDX          2
#define VFD_OVEN1_IDX           4
#define VFD_FLAME_FLASH_BIT     7


#define VFD_SP_SYMB_SMOKER      0
#define VFD_SP_SYMB_DELIMETER_1 1
#define VFD_SP_SYMB_TIMER_1     2

#define VFD_SP_SYMB_DELIMETER_2 3
#define VFD_SP_SYMB_OVEN_1      4
#define VFD_SP_SYMB_TIMER_2     5

#define VFD_SP_SYMB_OVEN_2      6
#define VFD_SP_SYMB_MEAT        7
#define VFD_SP_SYMB_DEGREE_1    8
#define VFD_SP_SYMB_TEMP_1      9

#define VFD_SP_SYMB_TEMP_2      10
#define VFD_SP_SYMB_DEGREE_2    11
#define VFD_SP_SYMB_SETTING     12

#define VFD_FLAME_NONE          0
#define VFD_FLAME_TINY          1
#define VFD_FLAME_SMALL         2
#define VFD_FLAME_MEDIUM        3
#define VFD_FLAME_LARGE         4
#define VFD_FLAME_EXTRA_LARGE   5

const PROGMEM uint8_t FLAME_STATES_PGM[VFD_MAX_FLAME_STATES] = {
    0, //None
    1, //Tiny
    3, //Small
    7, //Medium
    15,//Large
    31 //Extra large
};


const PROGMEM uint8_t SPECIAL_SYMB_BIT_PGM[VFD_MAX_SPECIAL_SYMBOLS] = {
    7, //'Smoker'
    0, //Delimeter 1
    7, //'Timer1'

    0, //Delimeter 2
    7, //'Oven1'
    7, //'Timer2'

    0, //'Oven2'
    7, //'Meat'
    0, //Degree 1
    7, //'Temp1'

    7, //'Temp2'
    0, //Degree 2
    7  //'Setting'
};


const PROGMEM uint8_t SPECIAL_SYMB_ADDR_PGM[VFD_MAX_SPECIAL_SYMBOLS] = {
    24, //'Smoker'
    25, //Delimeter 1
    21, //'Timer1'

    10, //Delimeter 2
    9,  //'Oven1'
    6,  //'Timer2'

    19, //'Oven2'
    18, //'Meat'
    16, //Degree 1
    15, //'Temp1'

    3,  //'Temp2'
    1,  //Degree 2
    0   //'Setting'
};

/* Mapping between digit position and its actual address
  (all seven segment digits have logical number - position
  associated with real address)
*/

const PROGMEM uint8_t POS2ADDR_MAP_PGM[VFD_MAX_COLUMNS] = {
    24, //pos 0 (digit on top left corner at 24)
    25,
    22,
    10,
    7,
    6,

    18, //pos 6 (digit on bottom left corner at 18)
    19,
    16,
    15,
    3,
    4,
    1,
    0
};

const PROGMEM uint8_t DIGIT_SEG_MAP_VARIANT_PGM[VFD_MAX_COLUMNS]  = {
    0x0, //variant one, no conversion needed for digit on top left corner
    0x1, //variant two (character at 26)
    0x1, //same as above for 23
    0x1, //same as above for 11
    0x1, //same as above for 8
    0x0, //variant one again for 7

    0x2, //variant three for 19
    0x3, //variant four for 20
    0x3, //variant four for 17
    0x2, //variant three for 16
    0x2, //variant three for 4
    0x3, //variant four for 5
    0x3, //variant four for 2
    0x2  //variant three for 1
};

class VFD_19S9D : public Print 
{
    public:

        VFD_19S9D(uint8_t cs, uint8_t sclk, uint8_t data);

        void reset();
        void displayOnCmd(uint8_t dimming);
        void displayOffCmd();
        void setCur(uint8_t col);

        //Send ASCII value to display
        size_t write(uint8_t value);
        //Send ASCII value to intermediate buffer
        void write_f(uint8_t value, uint8_t dstIndex);

        //Transfer array to display
        uint8_t write(uint8_t* buffer, uint8_t dstIndex, uint8_t len);
        //Transfer array to intermediate buffer
        uint8_t write_f(uint8_t* buffer, uint8_t dstIndex, uint8_t len);
        //Transfer string from program memory to intermediate buffer
        void print_f_p(const char str[]);
        //Transfer string from memory to intermediate buffer
        void print_f(char str[]);
        //Transfer whole intermediate buffer to display
        void flipFrame();
        void flipFlashState();
	void clearFrame();

        uint16_t getFlashAttr(uint8_t index);
        void setFlashAttr(uint8_t index, uint16_t value);

        uint8_t convertChar(uint8_t col, uint8_t char2conv);

        void symbolOn(uint8_t symbol_idx);
        void symbolOff(uint8_t symbol_idx);
        void setFlame(uint8_t flame_idx, uint8_t isFlashing = 0);
        void renderSymbols();
        void renderFlame();

        void symbolSetFlashAttr(uint8_t symbol_idx, uint16_t value);
        uint16_t symbolGetFlashAttr(uint8_t symbol_idx);
        inline void superimposeSymbol(uint8_t* value, uint8_t idx);

protected: 
        uint8_t _vfdFrame[VFD_DIGITS * VFD_BYTES_PER_DIGIT]; 
        uint16_t _specialSmblsBitMsk;
        uint16_t _flashAttr;
        uint16_t _specialSmblsFlashAttr;
        uint8_t _flameState;
        PT6311 driver;
        uint8_t _col;
};

#endif
