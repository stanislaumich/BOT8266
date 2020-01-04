/*
 PT6311.h - Driver for PT6311 VFD Controller
 
 Copyright (c) 2014 Dmitry Pakhomenko.
 dmitryp@magictale.com
 http://magictale.com
 
 This example code is in the public domain.
 */


#ifndef PT6311_H
#define PT6311_H

#define VFD_DIMMING_MAX 7

#define VFD_DISP_MODE_8D20S  0   //8 digits, 20 segments
#define VFD_DISP_MODE_9D19S  8   //9 digits, 19 segments
#define VFD_DISP_MODE_10D18S 9   //10 digits, 18 segments
#define VFD_DISP_MODE_11D17S 0xA //11 digits, 17 segments
#define VFD_DISP_MODE_12D16S 0xB //12 digits, 16 segments
#define VFD_DISP_MODE_13D15S 0xC //13 digits, 15 segments
#define VFD_DISP_MODE_14D14S 0xD //14 digits, 14 segments
#define VFD_DISP_MODE_15D13S 0xE //15 digits, 13 segments
#define VFD_DISP_MODE_16D12S 0xF //16 digits, 12 segments

#define PT6311_MEM_WR_CMD   0x40
#define PT6311_LED_WR_CMD   0x41
#define PT6311_DISP_ON_CMD  0x88 //display on command with minumum brightness
#define PT6311_DISP_OFF_CMD 0x80 //display off command
#define PT6311_ADDR_SET_CMD 0xC0
#define PT6311_ADDR_SET_MSK 0x3F
#define PT6311_LOW_NIBBLES_SET 0x0F
#define PT6311_DATALEN 8
#define PT6311_INC_ADDR_BIT 2    //if this bit is set, address will be autoincremented

class PT6311
{
    public:
        PT6311();
        //Harware commands
        void init(uint8_t cs, uint8_t sclk, uint8_t data);
        void reset(uint8_t mode);
        void displayOnCmd(uint8_t dimming);
        void displayOffCmd();
        void displayMemWriteCmd(uint8_t addr_inc, uint8_t nodata);
        void displayLEDWriteCmd(uint8_t addr_inc, uint8_t nodata);
        void addrSetCmd(uint8_t addr);
        //Low level function for sending commands
        void command(uint8_t value, uint8_t nodata);
        uint8_t commands(uint8_t nodata);
        //Low level function for sending data
        void data(uint8_t value, uint8_t init_cs, uint8_t finalise_cs);

protected: 
        uint8_t _cs, _sclk, _data;
};
#endif
