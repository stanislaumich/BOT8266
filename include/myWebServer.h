#ifndef meWeb
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include "FS.h"
#include <EEPROM.h>
#define myWeb
//#ifndef common
 #include "common.h"
// #endif 
#ifndef myWiFi
 #include "myWiFi.h"
 #endif
//#ifndef defs
 #include "defs.h"
// #endif
/*#ifdef Lz428266ZV 
 #ifndef myRTC
   #include "myRTC.h"
  #endif
 #endif */
//#ifndef mySSDP
   #include"mySSDP.h"
//  #endif 
#ifndef myTime
  #include "myTime.h"
 #endif 
ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;
File fsUploadFile;
String XML;

void Log(String s){
  //Serial.println(s);
 }

String getContentType(String filename) {
  if (httpServer.hasArg("download")) return "application/octet-stream";
  else if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
 }

void handlebeep(void){
  beep(httpServer.arg("t1").toInt(),httpServer.arg("t2").toInt());
  httpServer.sendHeader("Connection", "close");
  httpServer.send(200, "text/plain", "Ok beep ");
 } 

bool handleFileRead(String path) {  
  if (path.endsWith("/")) path += "index.htm";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) {
    if (SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    httpServer.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
 }

void handleFileUpload() {
  Log("Upload file");
  if (httpServer.uri() != "/edit") return;
  HTTPUpload& upload = httpServer.upload();
  if (upload.status == UPLOAD_FILE_START) {
    String filename = upload.filename;
    if (!filename.startsWith("/")) filename = "/" + filename;
    fsUploadFile = SPIFFS.open(filename, "w");
    filename = String();
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize);
  } else if (upload.status == UPLOAD_FILE_END) {
    if (fsUploadFile)
      fsUploadFile.close();
  }
 }

void handleFileDelete() {
  Log("Delete file");
  if (httpServer.args() == 0) return httpServer.send(500, "text/plain", "BAD ARGS");
  String path = httpServer.arg(0);
  if (path == "/")
    return httpServer.send(500, "text/plain", "BAD PATH");
  if (!SPIFFS.exists(path))
    return httpServer.send(404, "text/plain", "FileNotFound");
  SPIFFS.remove(path);
  httpServer.send(200, "text/plain", "");
  path = String();
 }
void handleFileCreate() {
  Log("Create file");
  if (httpServer.args() == 0)
    return httpServer.send(500, "text/plain", "BAD ARGS");
  String path = httpServer.arg(0);
  if (path == "/")
    return httpServer.send(500, "text/plain", "BAD PATH");
  if (SPIFFS.exists(path))
    return httpServer.send(500, "text/plain", "FILE EXISTS");
  File file = SPIFFS.open(path, "w");
  if (file)
    file.close();
  else
    return httpServer.send(500, "text/plain", "CREATE FAILED");
 httpServer.send(200, "text/plain", "");
  path = String();  
 }


void handleFileList() {  
  if (!httpServer.hasArg("dir")) {
    httpServer.send(500, "text/plain", "BAD ARGS");
    return;
  }
  // https://techtutorialsx.com/2019/02/24/esp32-arduino-listing-files-in-a-spiffs-file-system-specific-path/
  String path = httpServer.arg("dir");
  Dir dir = SPIFFS.openDir(path);
  path = String();
  String output = "[";
  while (dir.next()) {
    File entry = dir.openFile("r");
    if (output != "[") output += ',';
    bool isDir = false;
    output += "{\"type\":\"";
    output += (isDir) ? "dir" : "file";
    output += "\",\"name\":\"";
    output += String(entry.name()).substring(1);
    output += "\"}";
    entry.close();
  }
  output += "]";  
  httpServer.send(200, "text/json", output);
 }

void handle_Button() {
  int state = httpServer.arg("state").toInt();
  Button(state);
  httpServer.sendHeader("Connection", "close");
  httpServer.send(200, "text/plain", "Ok");  
 }

String alert_h(){
  String Time ="";
  int m=0;
  int h=0;
  h = EEPROM.read(0);
  m = EEPROM.read(1);
  Time+= (String)h+":";
  Time+= (String)m; 
  return Time;
 }

String XmlTime(void) {
  String Time= (hour<10?"0":"")+(String)hour+":";
  Time+= (mins<10?"0":"")+(String)mins; 
  return Time;
 }
void handle_Time() {
  int h = httpServer.arg("h").toInt();
  int m = httpServer.arg("m").toInt();
  String Time ="";
  Time+= (String)h+":";
  Time+= (String)m; 
  EEPROM.write(0, h); 
  EEPROM.write(1, m);
  EEPROM.commit();
  h = EEPROM.read(0);
  m = EEPROM.read(1);
  Time="";
  Time+= (String)h+":";
  Time+= (String)m; 
 }
String millis2time(); 
void buildXML(){
  XML="<?xml version='1.0'?>";
  XML+="<Donnees>"; 
    XML+="<response>";
    XML+=millis2time();
    XML+="</response>";
    XML+="<alert_time>";
    XML+=alert_h();
    XML+="</alert_time>";
    XML+="<time>";
    XML+=XmlTime();
    XML+="</time>";
    //XML+="<b0>";
    //XML+=XMLb0;
    //XML+="</b0>";
    //XML+="<b1>";
    //XML+=XMLb1;
    //XML+="</b1>";
    //XML+="<b2>";
    //XML+=XMLb2;
    //XML+="</b2>";
  XML+="</Donnees>"; 
 }
void handleXML(){
  buildXML();
  httpServer.send(200,"text/xml",XML);
 }
void handlereboot(){
  String s="Rebooting, refresh page";
  httpServer.sendHeader("Connection", "close");
  httpServer.send(200,"text/html",s);
  ESP.restart();
 }
void handleShowTime(void){
  String s="Long time ago...";
  Log(s);
  httpServer.sendHeader("Connection", "close");
  httpServer.send(200,"text/html",s);
 }

void initWeb(void){
  SPIFFS.begin();
  httpUpdater.setup(&httpServer);
  httpServer.on("/beep",handlebeep);
  httpServer.on("/reboot",handlereboot);
  httpServer.on("/xml",handleXML);
  httpServer.on("/list", HTTP_GET, handleFileList);
  httpServer.on("/Time", HTTP_GET, handle_Time);
  httpServer.on("/Button", handle_Button);
  httpServer.on("/time",handleShowTime);
  httpServer.on("/edit", HTTP_GET, []() {
    if (!handleFileRead("/edit.htm")) httpServer.send(404, "text/plain", "FileNotFound");
   });
  httpServer.on("/edit", HTTP_PUT, handleFileCreate);
  httpServer.on("/edit", HTTP_DELETE, handleFileDelete);
  httpServer.on("/edit", HTTP_POST, []() {
    httpServer.sendHeader("Connection", "close");//my
    httpServer.send(200, "text/plain", "");
   }, handleFileUpload);
  httpServer.on("/description.xml", HTTP_GET, []() {
    SSDP.schema(httpServer.client());
  });
  httpServer.onNotFound([]() {
    if (!handleFileRead(httpServer.uri()))
      httpServer.send(404, "text/plain", "FileNotFound");      
  });
  httpServer.begin();
  #ifdef Serialmy
   Serial.print("HTTPUpdateServer ready! Open http://IP/update in your browser\n");
  #endif
 }
 #endif