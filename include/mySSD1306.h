#ifndef mySSD1306 // for SSD1306 0.96'' little long 96x32
#define mySSD1306
#include "SSD1306.h"
#include "font_orbitum_32.h"
#define fnt Orbitron_Medium_32 //24 w secs
#include "place.h"
#include "defs.h"
#include "common.h"

extern uint8_t hour;
extern uint8_t mins;

#define OLED_SDA    5  // pin 14 gpio5!
#define OLED_SDC    4  // pin 12 gpio4!
#define OLED_ADDR   0x3C

SSD1306  display(OLED_ADDR, OLED_SDA, OLED_SDC);
bool ssd1306ON = true;
String ssd13str="";

void clrscrT(void){
 display.clear();
 } 
void screenoffT(void){
  display.displayOff();
  ssd1306ON=false;
 }
void screenonT(void){
  display.displayOn();
  ssd1306ON=true;
 }

void tickclockT(void){
  
 }

void showtimeT(void){
   display.clear();
   display.setFont(fnt);
   display.setTextAlignment(TEXT_ALIGN_LEFT);
   //ssd13str=timeClient.getHours()+":"+timeClient.getMinutes();
   // int hour = timeClient.getHours();
   //mins = timeClient.getMinutes();
   //sec =  timeClient.getSeconds();
   //ssd13str = random(100000);
   ssd13str = (hour<10?"0"+String(hour):String(hour))+String(":")+(mins<10?"0"+String(mins):String(mins));
   //ssd13str = (hour<10?"0"+String(hour):String(hour))+String(":")+(mins<10?"0"+String(mins):String(mins))+String(":")+(sec<10?"0"+String(sec):String(sec));
   display.drawString(6, 4, ssd13str);
   //display.drawString(2, 4,"12345678");
   display.display();
}

void initmyDisplayT(void){
  display.init();
  screenonT();
  //display.flipScreenVertically();
   display.clear();
   display.setFont(fnt);
   display.setTextAlignment(TEXT_ALIGN_LEFT);
   display.drawString(1, 4, "Start");
   display.display();
   delay(2000);
 }
#endif