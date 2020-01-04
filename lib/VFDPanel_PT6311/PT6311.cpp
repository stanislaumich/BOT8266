/*
 PT6311.cpp - Driver for PT6311 VFD Controller
 
 Copyright (c) 2014 Dmitry Pakhomenko.
 dmitryp@magictale.com
 http://magictale.com
 
 This example code is in the public domain.
 */

#include "settings.h"
#include "PT6311.h"

PT6311::PT6311()
{
}

void PT6311::init(uint8_t cs, uint8_t sclk, uint8_t data)
{
    _cs = cs;
    _sclk = sclk;
    _data = data;
}

void PT6311::reset(uint8_t mode) 
{
    pinMode(_cs, OUTPUT);
    pinMode(_sclk, OUTPUT);
    pinMode(_data, OUTPUT);
    digitalWrite(_cs, HIGH);
    digitalWrite(_sclk, HIGH);

    //Set display mode
    command(mode & PT6311_LOW_NIBBLES_SET, true);
    displayOnCmd(7/*VFD_DIMMING_MAX*/);//maximum brightness
}

void PT6311::displayOnCmd(uint8_t dimming)
{
    command(PT6311_DISP_ON_CMD | (dimming & VFD_DIMMING_MAX), true);
}

void PT6311::displayOffCmd()
{
    command(PT6311_DISP_OFF_CMD, true);
}

void PT6311::displayMemWriteCmd(uint8_t addr_inc, uint8_t nodata)
{
    uint8_t cmd = PT6311_MEM_WR_CMD;
    if (addr_inc > 0) cmd | (1 << PT6311_INC_ADDR_BIT);
    command(cmd, nodata);
}

void PT6311::displayLEDWriteCmd(uint8_t addr_inc, uint8_t nodata)
{
    uint8_t cmd = PT6311_LED_WR_CMD;
    if (addr_inc > 0) cmd | (1 << PT6311_INC_ADDR_BIT);
    command(cmd, nodata);
}

void PT6311::addrSetCmd(uint8_t addr)
{
    command(PT6311_ADDR_SET_CMD | (addr & PT6311_ADDR_SET_MSK), true);
}

void PT6311::command(uint8_t value, uint8_t nodata) 
{
    digitalWrite(_sclk, HIGH);
    digitalWrite(_cs, LOW);

    uint8_t i;
    for (i = 0; i < PT6311_DATALEN; i++)
    {
        if (( value >> i ) & 0x1)
            digitalWrite(_data, HIGH);
        else 
            digitalWrite(_data, LOW);
        digitalWrite(_sclk, LOW);
        //delay(10);//8266
        digitalWrite(_sclk, HIGH);
        //delay(10);//8266
    }
    if (nodata > 0) digitalWrite(_cs, HIGH);
}

uint8_t PT6311::commands(uint8_t nodata) 
{
    digitalWrite(_sclk, HIGH);
    digitalWrite(_cs, LOW);
    uint8_t t;
    uint8_t i;
    for (i = 0; i < PT6311_DATALEN; i++)
    {
        t=digitalRead(_data);
        /*
        if (( value >> i ) & 0x1)
            digitalWrite(_data, HIGH);
        else 
            digitalWrite(_data, LOW);
        */


        digitalWrite(_sclk, LOW);
        digitalWrite(_sclk, HIGH);
    }
    if (nodata > 0) digitalWrite(_cs, HIGH);
    return t;
}


void PT6311::data(uint8_t value, uint8_t init_cs, uint8_t finalise_cs) 
{
    digitalWrite(_sclk, HIGH);
    if (init_cs > 0) digitalWrite(_cs, LOW);

    uint8_t i;
    for (i = 0; i < PT6311_DATALEN; i++)
    {
        if (( value >> i ) & 0x1)
            digitalWrite(_data, HIGH);
        else 
            digitalWrite(_data, LOW);
        digitalWrite(_sclk, LOW);
        //delay(10);//8266
        digitalWrite(_sclk, HIGH);
        //delay(10);//8266
    }
    if (finalise_cs > 0) digitalWrite(_cs, HIGH);
}




