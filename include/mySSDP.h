#ifndef mySSDP
  #define mySSDP
#include <ESP8266SSDP.h>
#include "myWiFi.h"
void initSSDP(void) {
    //Если версия  2.0.0 закoментируйте следующую строчку
  SSDP.setDeviceType("upnp:rootdevice");
  SSDP.setSchemaURL("description.xml");
  SSDP.setHTTPPort(80);
  SSDP.setName(myName);
  SSDP.setSerialNumber("280119780945");
  SSDP.setURL("/");
  SSDP.setModelName(myName);
  SSDP.setModelNumber("000000000002");
  SSDP.setModelURL("http://"+IP_to_String(WiFi.localIP()));
  SSDP.setManufacturer("Lz42");
  SSDP.setManufacturerURL("http://"+IP_to_String(WiFi.localIP()));
  SSDP.begin();
}
 #endif 