#ifndef defs
#define defs
#include "place.h"
#include <ESP8266WiFi.h>
#define myTele "357390016"
#define LzName "Lz42Stas"

#define S868 "519049530"

#ifdef Lz428266WR
 String myID="947749033";
 String myName="Lz428266WR";
 String myToken="947749033:AAF00_fgJ0JTYF2XsZE_0zbz-8aZwtdHb-M";
 #define pinbeep D2
 #ifdef home
  IPAddress ip(192,168,1,238); 
 #endif
 #ifdef work
  IPAddress ip(192,168,0,234);
 #endif  
 /*#ifndef myComDisplay
   #include "myComDisplay.h"
  #endif
 */  
#endif //#ifdef Lz428266

#ifdef Lz428266YE
 String myID="1009154974";
 String myName="Lz428266YE";
 String myToken="1009154974:AAH10HBoPPbVbUvyainn_UyDmGve3YEZNEQ";
 #define redpin 15 // 15 RED, 14 - рядом маленький 16 - рядом ещё ближе  13 - синий 12 зелёный
 #define bluepin 13
 #define greenpin 12
 #define pinbeep 14
 #ifdef home
  IPAddress ip(192,168,1,237); 
 #endif
 #ifdef work
  IPAddress ip(192,168,0,235);
 #endif
 /*#ifndef mySSD1306
  #include "mySSD1306.h"
  #endif*/
#endif //#ifdef Lz428266YE

#ifdef Lz428266ZV
 String myID="1024759432";
 String myName="Lz428266ZV";
 String myToken="1024759432:AAEqPfDzbySApHVRp5cyqKg2uz6HRGGak9U";
 #define pinbeep 14
 #define bluepin 13
 #define greenpin 12
 #define redpin 15
 #define relay1pin 16
 #ifdef home
  IPAddress ip(192,168,1,238); 
 #endif
 #ifdef work
  IPAddress ip(192,168,0,235);
 #endif
  /*#ifndef myRTC
  #include "myRTC.h"
  #endif*/
 /*#ifndef lcd1602
  #include "lcd1602.h"
  #endif*/
#endif // #ifdef Lz428266ZV

#ifdef Lz428266VFD
 String myID="686030737";
 String myName="Lz428266VFD";
 String myToken="686030737:AAGN2_GY_wem10zGwNWYP7KyajTta5qpb2Y";
 #define pinbeep D7
  #ifdef home
  IPAddress ip(192,168,1,235);//235 
 #endif
 #ifdef work
  IPAddress ip(192,168,0,235);
 #endif
/* #ifndef myTime
   #include "myTime.h"
  #endif
  */
 /*#ifndef myVFD
  #include "myVFD.h"
  #endif*/
#endif // #ifdef Lz428266VFD

#ifdef home
 #ifndef STASSID
  #define STASSID "ZTE54"
  #define STAPSK  "121211119"
 #endif
 IPAddress gateway(192,168,1,1);
 IPAddress subnet(255,255,255,0);
 IPAddress dns1(1,1,1,1);
 IPAddress dns2(8,8,8,8);
#endif

#ifdef work
 #ifndef STASSID
  #define STASSID "SAN"
  #define STAPSK  "37212628"
 #endif
 IPAddress gateway(192,168,0,1);
 IPAddress subnet(255,255,255,0);
 IPAddress dns1(194,158,196,137);
 IPAddress dns2(194,158,196,141);
#endif 

#endif

