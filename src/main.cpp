#include <Arduino.h>
#include "place.h"
#include "defs.h"
#include "common.h"
#include "myDisplay.h"
//#include "myWiFi.h"

#ifndef mySSDP
 #include "mySSDP.h"
 #endif
#ifndef myWeb 
 #include "myWebServer.h"
 #endif
#ifndef MyTeleBot
  #include "myTeleBot.h"
 #endif
/*   
#ifdef Lz428266YE
  //#include "myBMP280.h"  
 #endif 
*/
bool pingMe(void);
void showtime(void);
unsigned long prev=0;

void ticktime(){
  timeClient.update();
  hour=timeClient.getHours();
  mins=timeClient.getMinutes();
  sec=timeClient.getSeconds();
  wd=timeClient.getDay();
  isbud()?bud():dummy();
  showtime();
  if ((sec>=0&&sec<=1)&&mins==0){chime();}
  if (pingMe()){beep(150,150);}
}

void reboot(void){
  longbeep();
  ESP.restart();   
 }

void setup(){  
  initCommon();
  Ttickclock.stop();
  Treboot.stop();
  shortbeep();
  initWiFi();
  initTime();
  initmyDisplay();
  initSSDP();
  initWeb();
  MyTeleBotInit();
  bot.sendMessage(myTele, "Бот запущен: "+IP_to_String(WiFi.localIP()), "");
  dshortbeep();
 }

void loop (){
 if (Tticktime.isReady()) ticktime();
 if (Ttickclock.isReady()) tickclock();
 if (Treboot.isReady()) reboot();
 goBot();
 httpServer.handleClient();
 }// TOTAL END ///////////////////////////////////////////////////////////////////////