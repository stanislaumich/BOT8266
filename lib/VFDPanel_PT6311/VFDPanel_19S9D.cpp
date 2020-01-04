/*
 PT6311.h - Driver for PT6311 VFD Controller
 
 Copyright (c) 2014 Dmitry Pakhomenko.
 dmitryp@magictale.com
 http://magictale.com
 
 This example code is in the public domain.
 */

#include "VFDPanel_19S9D.h"


VFD_19S9D::VFD_19S9D(uint8_t cs, uint8_t sclk, uint8_t data)
{
    driver.init(cs, sclk, data);
}

void VFD_19S9D::reset()
{
    driver.reset(VFD_DISP_MODE_9D19S);
    driver.addrSetCmd(0);

    _specialSmblsBitMsk = 0;
    _flameState = 0;

    clearFrame();
    flipFrame();
}

void VFD_19S9D::displayOnCmd(uint8_t dimming)
{
    driver.displayOnCmd(dimming);
}

void VFD_19S9D::displayOffCmd()
{
    driver.displayOffCmd();
}

void VFD_19S9D::setCur(uint8_t col)
{
    if (col >= VFD_DIGITS) col = 0;
    driver.addrSetCmd(col * VFD_BYTES_PER_DIGIT);
}

uint8_t VFD_19S9D::convertChar(uint8_t col, uint8_t char2conv)
{
    uint8_t res = 0;
    uint8_t variant = pgm_read_byte(&DIGIT_SEG_MAP_VARIANT_PGM[col]);

    switch (variant)
    {
        case 0:
               res = char2conv; //no changes required
               break;

        case 1:
               if (char2conv & 0x1) res |= 0x2;
               if (char2conv & 0x2) res |= 0x4;
               if (char2conv & 0x4) res |= 0x8;
               if (char2conv & 0x8) res |= 0x10;
               if (char2conv & 0x10) res |= 0x20;
               if (char2conv & 0x20) res |= 0x40;
               if (char2conv & 0x40) res |= 0x80;

               break;

        case 2:
               if (char2conv & 0x1) res |= 0x40;
               if (char2conv & 0x2) res |= 0x20;
               if (char2conv & 0x4) res |= 0x10;
               if (char2conv & 0x8) res |= 0x8;
               if (char2conv & 0x10) res |= 0x4;
               if (char2conv & 0x20) res |= 0x2;
               if (char2conv & 0x40) res |= 0x1;
 
               break;

        case 3:
               if (char2conv & 0x1) res |= 0x80;
               if (char2conv & 0x2) res |= 0x40;
               if (char2conv & 0x4) res |= 0x20;
               if (char2conv & 0x8) res |= 0x10;
               if (char2conv & 0x10) res |= 0x8;
               if (char2conv & 0x20) res |= 0x4;
               if (char2conv & 0x40) res |= 0x2;

               break;
    }
    return res;
}

/*
  Transfers one ASCII symbol directly to VFD driver
*/
size_t VFD_19S9D::write(uint8_t value) 
{
    driver.displayMemWriteCmd(true, false);

    if (value < VFD_FIRST_DISP_CHAR) value = VFD_FIRST_DISP_CHAR;

    uint8_t character = pgm_read_byte(&FONT_PGM[(value - VFD_FIRST_DISP_CHAR)][0]);
    character = convertChar(_col, character);

    driver.data(character, false, true);
    return 1;
}

/*
  Copies array of ASCII symbols to intermediate frame buffer
*/
void VFD_19S9D::write_f(uint8_t value, uint8_t dstIndex) 
{
    if (dstIndex > ((VFD_DIGITS * VFD_BYTES_PER_DIGIT) - 1)) return;

    if (value < VFD_FIRST_DISP_CHAR) value = VFD_FIRST_DISP_CHAR;

    uint8_t character = pgm_read_byte(&FONT_PGM[(value - VFD_FIRST_DISP_CHAR)][0]);
    character = convertChar(_col, character);

    _vfdFrame[dstIndex] = character;
}

/*
  Transfers a byte array directly to VFD driver starting at specific address
*/
uint8_t VFD_19S9D::write(uint8_t* buffer, uint8_t dstIndex, uint8_t len)
{
    if (len == 0 || (dstIndex >= VFD_DIGITS * VFD_BYTES_PER_DIGIT)) return 0;
    if (len > VFD_DIGITS * VFD_BYTES_PER_DIGIT - dstIndex) 
        len = VFD_DIGITS * VFD_BYTES_PER_DIGIT - dstIndex;

    driver.addrSetCmd(dstIndex);

    driver.displayMemWriteCmd(true, false);
    for (uint8_t i = 0; i < len-1; i++)
    {
         driver.data(buffer[i], false, false);
    } 
    driver.data(buffer[len-1], false, true);

    return len;
}

/*
  Copies a byte array to intermediate frame buffer
*/
uint8_t VFD_19S9D::write_f(uint8_t* buffer, uint8_t dstIndex, uint8_t len)
{
    if (len == 0 || (dstIndex >= VFD_DIGITS * VFD_BYTES_PER_DIGIT)) return 0;
    if (len > VFD_DIGITS * VFD_BYTES_PER_DIGIT - dstIndex) 
        len = VFD_DIGITS * VFD_BYTES_PER_DIGIT - dstIndex;

    memcpy(&_vfdFrame[dstIndex], buffer, len);

    return len;
}

/*
  Reads an ASCII string from PGM memory and writes it to intermediate frame buffer
*/
void VFD_19S9D::print_f_p(const char str[])
{
    char c;
    uint8_t idx = 0;
    if(!str) return;

    clearFrame();
   
    while((c = pgm_read_byte(str++)))
    {
        idx = pgm_read_byte(&POS2ADDR_MAP_PGM[_col]);

        write_f(c, idx);
        _col++;
        if (_col >= VFD_MAX_COLUMNS) return;
    }
}

/*
  Reads an ASCII string from memory and writes it to intermediate frame buffer
*/
void VFD_19S9D::print_f(char str[])
{
    char c;
    uint8_t idx = 0;
    if(!str) return;

    clearFrame();

    while (c = *str++) 
    {
        idx = pgm_read_byte(&POS2ADDR_MAP_PGM[_col]);

        write_f(c, idx);
        _col++;
        if (_col >= VFD_MAX_COLUMNS) return;
    }
}


void VFD_19S9D::flipFrame()
{
    renderSymbols();
    renderFlame();

    //No flashing, fast algorithm, sending bytearray in one go
    if (_flashAttr == 0 || ((_flashAttr >> VFD_FLASH_MODE_BIT ) & 0x1) == 0)
    {
        write((uint8_t*)&_vfdFrame, 0, VFD_DIGITS * VFD_BYTES_PER_DIGIT);
        return;
    }

    //Flashing is enabled, slow algorithm, sending digit by digit
    uint8_t idx = 0;
    uint8_t tmpBuf;
    uint8_t symbolAddr;


    for (_col = 0; _col < VFD_MAX_COLUMNS; _col++)
    {
        idx = pgm_read_byte(&POS2ADDR_MAP_PGM[_col]);
        if (((_flashAttr >> _col ) & 0x1) == 0)
        {
            //Flashing is not enabled for this digit, send to VFD without change
            tmpBuf = _vfdFrame[idx];
        }else{
            //The digit is not visible during 'off' flashing phase
            tmpBuf = 0;
        }

        //now tmpBuf contains either a digit in raw representation 
        //or zero (all segments are switched off to achieve flashing 
        //effect. Superimpose symbols if any.
        for (uint8_t i = 0; i < VFD_MAX_SPECIAL_SYMBOLS; i++)
        {
           symbolAddr = pgm_read_byte(&SPECIAL_SYMB_ADDR_PGM[i]);
           //Does current digit have a special symbol associated with it? 
           if (symbolAddr == idx)
           {
               break;
           }           
        }

        // and finally write the value to VFD controller
        write(&tmpBuf, idx, 1);
    }

    //Handle separately three special symbols which don't belong to any digit
    symbolAddr = pgm_read_byte(&SPECIAL_SYMB_ADDR_PGM[VFD_TIMER1_IDX]);
    tmpBuf = 0;
    superimposeSymbol(&tmpBuf, VFD_TIMER1_IDX);
    write(&tmpBuf, symbolAddr, 1);

    symbolAddr = pgm_read_byte(&SPECIAL_SYMB_ADDR_PGM[VFD_OVEN1_IDX]);
    tmpBuf = 0;
    superimposeSymbol(&tmpBuf, VFD_OVEN1_IDX);
    write(&tmpBuf, symbolAddr, 1);

    if ((_flameState >> VFD_FLAME_FLASH_BIT) &  0x1) tmpBuf = 0;
    else tmpBuf = pgm_read_byte(&FLAME_STATES_PGM[_flameState]);
    write(&tmpBuf, VFD_FLAME_ADDR, 1);
}

inline void VFD_19S9D::superimposeSymbol(uint8_t* value, uint8_t idx)
{
    uint8_t symbolMask;
    //Is special symbol lit up?
    if ((_specialSmblsBitMsk >> idx) & 0x1)
    {
        symbolMask = pgm_read_byte(&SPECIAL_SYMB_BIT_PGM[idx]);
        //Is flashing enabled for this symbol
        if ((_specialSmblsFlashAttr >> idx) & 0x1)
        {
            *value &= ~(1 << symbolMask);
        }else{
            //Display symbol regardles of flash phase
            *value |= 1 << symbolMask;
        }
   } else *value &= ~(1 << symbolMask);
}

void VFD_19S9D::clearFrame()
{
    memset(&_vfdFrame, 0, sizeof(_vfdFrame));
    _col = 0;
}

uint16_t VFD_19S9D::getFlashAttr(uint8_t index)
{
    if (index >= VFD_MAX_COLUMNS) return (_flashAttr & ~(1 << VFD_FLASH_MODE_BIT));
    else return (( _flashAttr >> index ) & 0x1);
}
     
void VFD_19S9D::setFlashAttr(uint8_t index, uint16_t value)
{
    if (index >= VFD_MAX_COLUMNS) 
        _flashAttr = (_flashAttr & (1 << VFD_FLASH_MODE_BIT)) | (value & ~(1 << VFD_FLASH_MODE_BIT));
    else _flashAttr |= 1 << index;
}

void VFD_19S9D::flipFlashState()
{
    if (((_flashAttr >> VFD_FLASH_MODE_BIT ) & 0x1) == 0)
    {
        _flashAttr |= 1 << VFD_FLASH_MODE_BIT;
    }else{
        _flashAttr &= ~(1 << VFD_FLASH_MODE_BIT);
    }
}

void VFD_19S9D::symbolOn(uint8_t symbol_idx)
{
    if (symbol_idx >= VFD_MAX_SPECIAL_SYMBOLS) return;
    _specialSmblsBitMsk |= 1 << symbol_idx; 

}

void VFD_19S9D::symbolOff(uint8_t symbol_idx)
{
    if (symbol_idx >= VFD_MAX_SPECIAL_SYMBOLS) return;
    _specialSmblsBitMsk &= ~(1 << symbol_idx); 

}

void VFD_19S9D::renderSymbols()
{
    uint8_t symbolMask;
    uint8_t symbolAddr;
    for (uint8_t i = 0; i < VFD_MAX_SPECIAL_SYMBOLS; i++)
    {
        if ((_specialSmblsBitMsk >> i) & 0x1)
        {
            symbolMask = pgm_read_byte(&SPECIAL_SYMB_BIT_PGM[i]);
            symbolAddr = pgm_read_byte(&SPECIAL_SYMB_ADDR_PGM[i]);

            _vfdFrame[symbolAddr] |= 1 << symbolMask;
        }
    }
}

void VFD_19S9D::setFlame(uint8_t flame_idx, uint8_t isFlashing)
{
    if (flame_idx >= VFD_MAX_FLAME_STATES) return;
    _flameState = flame_idx;
    if (isFlashing) _flameState |= (1 << VFD_FLAME_FLASH_BIT);
}

void VFD_19S9D::renderFlame()
{
    uint8_t flameState = pgm_read_byte(&FLAME_STATES_PGM[_flameState & ~(1 << VFD_FLAME_FLASH_BIT)]); 
    _vfdFrame[VFD_FLAME_ADDR] |= flameState;
}

void VFD_19S9D::symbolSetFlashAttr(uint8_t symbol_idx, uint16_t value)
{
    if (symbol_idx >= VFD_MAX_SPECIAL_SYMBOLS) _specialSmblsFlashAttr = value;
    else _specialSmblsFlashAttr |= 1 << symbol_idx; 

}

uint16_t VFD_19S9D::symbolGetFlashAttr(uint8_t symbol_idx)
{
    if (symbol_idx >= VFD_MAX_SPECIAL_SYMBOLS) return _specialSmblsFlashAttr;
    else return ((_specialSmblsFlashAttr >> symbol_idx ) & 0x1); 
}



