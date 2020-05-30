#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
#include <EEPROM.h>
#include <ESP32CAN.h>
#include <CAN_config.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

CAN_device_t CAN_cfg;               // CAN Config
const int rx_queue_size = 10;       // Receive Queue size

//const char* host = "esp32_1";
const char* ssid = "billboard 1";
const char* password = "avylady123";


#define L1 23
#define L2 22
#define L3 19
#define L4 18
#define L5 17
#define L6 4
#define L7 25
#define L8 12
#define L9 13
#define LED_WIFI 27

#define EEPROM_delayMode1 1
#define EEPROM_delayMode2 2
#define EEPROM_delayMode3 3
#define EEPROM_delayMode4 4
#define EEPROM_delayMode5 5
#define EEPROM_delayMode6 6
#define EEPROM_delayMode7 7
#define EEPROM_delayMode8 8
#define EEPROM_delayMode9 9

#define RESPONSE_LENGTH 512

int delayMode1 = 100;
int delayMode2 = 100;
int delayMode3 = 100;
int delayMode4 = 80;
int delayMode5 = 100;
int delayMode6 = 80;
int delayMode7 = 100;
int delayMode8 = 100;
int delayMode9 = 100;

unsigned long countTimeMillis;
int countStep = 0;
int countStepMode3 = 0;
int countStepMode4 = 1;
int countStepMode6 = 1;
int statusModeRunning = 1;
int modeRun = 10;

#define HTTP_PORT 80
WebServer server(HTTP_PORT);


const char* loginIndex = 
 "<form name='loginForm'>"
    "<table width='20%' bgcolor='A09F9F' align='center'>"
        "<tr>"
            "<td colspan=2>"
                "<center><font size=4><b>ESP32_1 Login Page</b></font></center>"
                "<br>"
            "</td>"
            "<br>"
            "<br>"
        "</tr>"
        "<td>Username:</td>"
        "<td><input type='text' size=25 name='userid'><br></td>"
        "</tr>"
        "<br>"
        "<br>"
        "<tr>"
            "<td>Password:</td>"
            "<td><input type='Password' size=25 name='pwd'><br></td>"
            "<br>"
            "<br>"
        "</tr>"
        "<tr>"
            "<td><input type='submit' onclick='check(this.form)' value='Login'></td>"
        "</tr>"
    "</table>"
  "</form>"
  "<script>"
    "function check(form)"
    "{"
    "if(form.userid.value=='admin' && form.pwd.value=='admin')"
    "{"
    "window.open('/serverIndex')"
    "}"
    "else"
    "{"
    " alert('Error Password or Username')/*displays error message*/"
    "}"
    "}"
  "</script>";
 
/*
 * Server Index Page
 */
 
const char* serverIndex = 
  "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
    "<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
      "<input type='file' name='update'>"
            "<input type='submit' value='Update'>"
        "</form>"
    "<div id='prg'>progress: 0%</div>"
    "<script>"
      "$('form').submit(function(e){"
      "e.preventDefault();"
      "var form = $('#upload_form')[0];"
      "var data = new FormData(form);"
      " $.ajax({"
      "url: '/update',"
      "type: 'POST',"
      "data: data,"
      "contentType: false,"
      "processData:false,"
      "xhr: function() {"
      "var xhr = new window.XMLHttpRequest();"
      "xhr.upload.addEventListener('progress', function(evt) {"
      "if (evt.lengthComputable) {"
      "var per = evt.loaded / evt.total;"
      "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
      "}"
      "}, false);"
      "return xhr;"
      "},"
      "success:function(d, s) {"
      "console.log('success!')" 
    "},"
    "error: function (a, b, c) {"
    "}"
    "});"
    "});"
  "</script>";
//-----------------//

void ledWrite(int led1, int led2, int led3, int led4, int led5, int led6, int led7, int led8, int led9){
  if(led1 == 1){
    digitalWrite(L1, HIGH);
  }else{
    digitalWrite(L1, LOW);
  }
  if(led2 == 1){
    digitalWrite(L2, HIGH);
  }else{
    digitalWrite(L2, LOW);
  }
  if(led3 == 1){
    digitalWrite(L3, HIGH);
  }else{
    digitalWrite(L3, LOW);
  }
  if(led4 == 1){
    digitalWrite(L4, HIGH);
  }else{
    digitalWrite(L4, LOW);
  }
  if(led5 == 1){
    digitalWrite(L5, HIGH);
  }else{
    digitalWrite(L5, LOW);
  }
  if(led6 == 1){
    digitalWrite(L6, HIGH);
  }else{
    digitalWrite(L6, LOW);
  }
  if(led7 == 1){
    digitalWrite(L7, HIGH);
  }else{
    digitalWrite(L7, LOW);
  }
  if(led8 == 1){
    digitalWrite(L8, HIGH);
  }else{
    digitalWrite(L8, LOW);
  }
  if(led9 == 1){
    digitalWrite(L9, HIGH);
  }else{
    digitalWrite(L9, LOW);
  }
}

void ledWriteInverse(int led1, int led2, int led3, int led4, int led5, int led6, int led7, int led8, int led9){
  if(led1 == 1){
    digitalWrite(L9, HIGH);
  }else{
    digitalWrite(L9, LOW);
  }
  if(led2 == 1){
    digitalWrite(L8, HIGH);
  }else{
    digitalWrite(L8, LOW);
  }
  if(led3 == 1){
    digitalWrite(L7, HIGH);
  }else{
    digitalWrite(L7, LOW);
  }
  if(led4 == 1){
    digitalWrite(L6, HIGH);
  }else{
    digitalWrite(L6, LOW);
  }
  if(led5 == 1){
    digitalWrite(L5, HIGH);
  }else{
    digitalWrite(L5, LOW);
  }
  if(led6 == 1){
    digitalWrite(L4, HIGH);
  }else{
    digitalWrite(L4, LOW);
  }
  if(led7 == 1){
    digitalWrite(L3, HIGH);
  }else{
    digitalWrite(L3, LOW);
  }
  if(led8 == 1){
    digitalWrite(L2, HIGH);
  }else{
    digitalWrite(L2, LOW);
  }
  if(led9 == 1){
    digitalWrite(L1, HIGH);
  }else{
    digitalWrite(L1, LOW);
  }
}

void turnOffAll(){
  digitalWrite(L1, LOW);
  digitalWrite(L2, LOW);
  digitalWrite(L3, LOW);
  digitalWrite(L4, LOW);
  digitalWrite(L5, LOW);
  digitalWrite(L6, LOW);
  digitalWrite(L7, LOW);
  digitalWrite(L8, LOW);
  digitalWrite(L9, LOW);
}

void turnOnAll(){
  digitalWrite(L1, HIGH);
  digitalWrite(L2, HIGH);
  digitalWrite(L3, HIGH);
  digitalWrite(L4, HIGH);
  digitalWrite(L5, HIGH);
  digitalWrite(L6, HIGH);
  digitalWrite(L7, HIGH);
  digitalWrite(L8, HIGH);
  digitalWrite(L9, HIGH);
}

void mode1(){
  if(countStep == 0 && millis() >= countTimeMillis + 0){
    countStep = 1;
    countTimeMillis = millis();
    digitalWrite(L1, HIGH);
  }
  if(countStep == 1 && millis() >= countTimeMillis + delayMode1){
    countStep = 2;
    countTimeMillis = millis();
    digitalWrite(L2, HIGH);
  }
  if(countStep == 2 && millis() >= countTimeMillis + delayMode1){
    countStep = 3;
    countTimeMillis = millis();
    digitalWrite(L3, HIGH);
  }
  if(countStep == 3 && millis() >= countTimeMillis + delayMode1){
    countStep = 4;
    countTimeMillis = millis();
    digitalWrite(L4, HIGH);
  }
  if(countStep == 4 && millis() >= countTimeMillis + delayMode1){
    countStep = 5;
    countTimeMillis = millis();
    digitalWrite(L5, HIGH);
  }
  if(countStep == 5 && millis() >= countTimeMillis + delayMode1){
    countStep = 6;
    countTimeMillis = millis();
    digitalWrite(L6, HIGH);
  }
  if(countStep == 6 && millis() >= countTimeMillis + delayMode1){
    countStep = 7;
    countTimeMillis = millis();
    digitalWrite(L7, HIGH);
  }
  if(countStep == 7 && millis() >= countTimeMillis + delayMode1){
    countStep = 8;
    countTimeMillis = millis();
    digitalWrite(L8, HIGH);
  }
  if(countStep == 8 && millis() >= countTimeMillis + delayMode1){
    countStep = 9;
    countTimeMillis = millis();
    digitalWrite(L9, HIGH);
  }
  if(countStep == 9 && millis() >= countTimeMillis + delayMode1 + 3*delayMode1*9){
    countStep = 10;
    countTimeMillis = millis();
  }
  //------------------------------------------------------------------------------------------------//
  //nhap nhay toan bo - 2500

  if(countStep == 10 && millis() >= countTimeMillis + 3*delayMode1){
    countStep = 11;
    countTimeMillis = millis();
    turnOffAll();
  }
  if(countStep == 11 && millis() >= countTimeMillis + 3*delayMode1){
    countStep = 12;
    countTimeMillis = millis();
    turnOnAll();
  }
  if(countStep == 12 && millis() >= countTimeMillis + 3*delayMode1){
    countStep = 13;
    countTimeMillis = millis();
    turnOffAll();
  }
  if(countStep == 13 && millis() >= countTimeMillis + 3*delayMode1){
    countStep = 14;
    countTimeMillis = millis();
    turnOnAll();
  }
  if(countStep == 14 && millis() >= countTimeMillis + 3*delayMode1){
    countStep = 15;
    countTimeMillis = millis();
    turnOffAll();
  }
  if(countStep == 15 && millis() >= countTimeMillis + 3*delayMode1){
    countStep = 16;
    countTimeMillis = millis();
  }
  //------------------------------------------------------------------------------------------------//
  //7200 + 3000 = 10200
  if(countStep == 16 && millis() >= countTimeMillis + 3*delayMode1*9){
    countStep = 17;
    countTimeMillis = millis();
  }
  if(countStep == 17 && millis() >= countTimeMillis + delayMode1){
    countStep = 18;
    countTimeMillis = millis();
    digitalWrite(L9, HIGH);
  }
  if(countStep == 18 && millis() >= countTimeMillis + delayMode1){
    countStep = 19;
    countTimeMillis = millis();
    digitalWrite(L8, HIGH);
  }
  if(countStep == 19 && millis() >= countTimeMillis + delayMode1){
    countStep = 20;
    countTimeMillis = millis();
    digitalWrite(L7, HIGH);
  }
  if(countStep == 20 && millis() >= countTimeMillis + delayMode1){
    countStep = 21;
    countTimeMillis = millis();
    digitalWrite(L6, HIGH);
  }
  if(countStep == 21 && millis() >= countTimeMillis + delayMode1){
    countStep = 22;
    countTimeMillis = millis();
    digitalWrite(L5, HIGH);
  }
  if(countStep == 22 && millis() >= countTimeMillis + delayMode1){
    countStep = 23;
    countTimeMillis = millis();
    digitalWrite(L4, HIGH);
  }
  if(countStep == 23 && millis() >= countTimeMillis + delayMode1){
    countStep = 24;
    countTimeMillis = millis();
    digitalWrite(L3, HIGH);
  }
  if(countStep == 24 && millis() >= countTimeMillis + delayMode1){
    countStep = 25;
    countTimeMillis = millis();
    digitalWrite(L2, HIGH);
  }
  if(countStep == 25 && millis() >= countTimeMillis + delayMode1){
    countStep = 26;
    countTimeMillis = millis();
    digitalWrite(L1, HIGH);
  }
  if(countStep == 26 && millis() >= countTimeMillis + delayMode1 + 0){
    countStep = 27;
    countTimeMillis = millis();
  }

  //------------------------------------------------------------------------------------------------//
  //nhap nhay toan bo - 2500
  if(countStep == 27 && millis() >= countTimeMillis + 3*delayMode1){
    countStep = 28;
    countTimeMillis = millis();
    turnOffAll();
  }
  if(countStep == 28 && millis() >= countTimeMillis + 3*delayMode1){
    countStep = 29;
    countTimeMillis = millis();
    turnOnAll();
  }
  if(countStep == 29 && millis() >= countTimeMillis + 3*delayMode1){
    countStep = 30;
    countTimeMillis = millis();
    turnOffAll();
  }
  if(countStep == 30 && millis() >= countTimeMillis + 3*delayMode1){
    countStep = 31;
    countTimeMillis = millis();
    turnOnAll();
  }
  if(countStep == 31 && millis() >= countTimeMillis + 3*delayMode1){
    countStep = 32;
    countTimeMillis = millis();
    turnOffAll();
  }
  if(countStep == 32 && millis() >= countTimeMillis + 3*delayMode1){
    countStep = 0;
    statusModeRunning = 2;
    countTimeMillis = millis();
  }  
}

void mode2(){
  if(countStep == 0 && millis() >= countTimeMillis){
    countStep = 1;
    countTimeMillis = millis();
    digitalWrite(L1, HIGH);
  }
  if(countStep == 1 && millis() >= countTimeMillis + delayMode2){
    countStep = 2;
    countTimeMillis = millis();
    digitalWrite(L2, HIGH);
  }
  if(countStep == 2 && millis() >= countTimeMillis + delayMode2){
    countStep = 3;
    countTimeMillis = millis();
    digitalWrite(L3, HIGH);
  }
  if(countStep == 3 && millis() >= countTimeMillis + delayMode2){
    countStep = 4;
    countTimeMillis = millis();
    digitalWrite(L4, HIGH);
  }
  if(countStep == 4 && millis() >= countTimeMillis + delayMode2){
    countStep = 5;
    countTimeMillis = millis();
    digitalWrite(L5, HIGH);
  }
  if(countStep == 5 && millis() >= countTimeMillis + delayMode2){
    countStep = 6;
    countTimeMillis = millis();
    digitalWrite(L6, HIGH);
  }
  if(countStep == 6 && millis() >= countTimeMillis + delayMode2){
    countStep = 7;
    countTimeMillis = millis();
    digitalWrite(L7, HIGH);
  }
  if(countStep == 7 && millis() >= countTimeMillis + delayMode2){
    countStep = 8;
    countTimeMillis = millis();
    digitalWrite(L8, HIGH);
  }
  if(countStep == 8 && millis() >= countTimeMillis + delayMode2){
    countStep = 9;
    countTimeMillis = millis();
    digitalWrite(L9, HIGH);
  }
  if(countStep == 9 && millis() >= countTimeMillis + delayMode2){
    countStep = 10;
    countTimeMillis = millis();
  }
  //------------------------------------------------------------------------------------------------//
  if(countStep == 10 && millis() >= countTimeMillis + 3*delayMode2){
    turnOffAll();
    countStep = 11;
    countTimeMillis = millis();
  }
  //------------------------------------------------------------------------------------------------//
  if(countStep == 11 && millis() >= countTimeMillis + 3*delayMode2){
    countStep = 12;
    countTimeMillis = millis();
    digitalWrite(L9, HIGH);
  }
  if(countStep == 12 && millis() >= countTimeMillis + delayMode2){
    countStep = 13;
    countTimeMillis = millis();
    digitalWrite(L8, HIGH);
  }
  if(countStep == 13 && millis() >= countTimeMillis + delayMode2){
    countStep = 14;
    countTimeMillis = millis();
    digitalWrite(L7, HIGH);
  }
  if(countStep == 14 && millis() >= countTimeMillis + delayMode2){
    countStep = 15;
    countTimeMillis = millis();
    digitalWrite(L6, HIGH);
  }
  if(countStep == 15 && millis() >= countTimeMillis + delayMode2){
    countStep = 16;
    countTimeMillis = millis();
    digitalWrite(L5, HIGH);
  }
  if(countStep == 16 && millis() >= countTimeMillis + delayMode2){
    countStep = 17;
    countTimeMillis = millis();
    digitalWrite(L4, HIGH);
  }
  if(countStep == 17 && millis() >= countTimeMillis + delayMode2){
    countStep = 18;
    countTimeMillis = millis();
    digitalWrite(L3, HIGH);
  }
  if(countStep == 18 && millis() >= countTimeMillis + delayMode2){
    countStep = 19;
    countTimeMillis = millis();
    digitalWrite(L2, HIGH);
  }
  if(countStep == 19 && millis() >= countTimeMillis + delayMode2){
    countStep = 20;
    countTimeMillis = millis();
    digitalWrite(L1, HIGH);
  }
  //------------------------------------------------------------------------------------------------//
  if(countStep == 20 && millis() >= countTimeMillis + 3*delayMode2){
    turnOffAll();
    countStep = 21;
    countTimeMillis = millis();
  }
  //------------------------------------------------------------------------------------------------//
  if(countStep == 21 && millis() >= countTimeMillis + 3*delayMode2){
    countStep = 22;
    countTimeMillis = millis();
    ledWrite(1, 0, 0, 0, 0, 0, 0, 0, 0);
  }
  if(countStep == 22 && millis() >= countTimeMillis + delayMode2){
    countStep = 23;
    countTimeMillis = millis();
    ledWrite(0, 1, 0, 0, 0, 0, 0, 0, 0);
  }
  if(countStep == 23 && millis() >= countTimeMillis + delayMode2){
    countStep = 24;
    countTimeMillis = millis();
    ledWrite(0, 0, 1, 0, 0, 0, 0, 0, 0);
  }
  if(countStep == 24 && millis() >= countTimeMillis + delayMode2){
    countStep = 25;
    countTimeMillis = millis();
    ledWrite(0, 0, 0, 1, 0, 0, 0, 0, 0);
  }
  if(countStep == 25 && millis() >= countTimeMillis + delayMode2){
    countStep = 26;
    countTimeMillis = millis();
    ledWrite(0, 0, 0, 0, 1, 0, 0, 0, 0);
  }
  if(countStep == 26 && millis() >= countTimeMillis + delayMode2){
    countStep = 27;
    countTimeMillis = millis();
    ledWrite(0, 0, 0, 0, 0, 1, 0, 0, 0);
  }
  if(countStep == 27 && millis() >= countTimeMillis + delayMode2){
    countStep = 28;
    countTimeMillis = millis();
    ledWrite(0, 0, 0, 0, 0, 0, 1, 0, 0);
  }
  if(countStep == 28 && millis() >= countTimeMillis + delayMode2){
    countStep = 29;
    countTimeMillis = millis();
    ledWrite(0, 0, 0, 0, 0, 0, 0, 1, 0);
  }
  if(countStep == 29 && millis() >= countTimeMillis + delayMode2){
    countStep = 30;
    countTimeMillis = millis();
    ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 1);
  }
  //---------------------//  
  if(countStep == 30 && millis() >= countTimeMillis + delayMode2){
    countStep = 31;
    countTimeMillis = millis();
    ledWrite(1, 0, 0, 0, 0, 0, 0, 0, 1);
  }
  if(countStep == 31 && millis() >= countTimeMillis + delayMode2){
    countStep = 32;
    countTimeMillis = millis();
    ledWrite(0, 1, 0, 0, 0, 0, 0, 0, 1);
  }
  if(countStep == 32 && millis() >= countTimeMillis + delayMode2){
    countStep = 33;
    countTimeMillis = millis();
    ledWrite(0, 0, 1, 0, 0, 0, 0, 0, 1);
  }
  if(countStep == 33 && millis() >= countTimeMillis + delayMode2){
    countStep = 34;
    countTimeMillis = millis();
    ledWrite(0, 0, 0, 1, 0, 0, 0, 0, 1);
  }
  if(countStep == 34 && millis() >= countTimeMillis + delayMode2){
    countStep = 35;
    countTimeMillis = millis();
    ledWrite(0, 0, 0, 0, 1, 0, 0, 0, 1);
  }
  if(countStep == 35 && millis() >= countTimeMillis + delayMode2){
    countStep = 36;
    countTimeMillis = millis();
    ledWrite(0, 0, 0, 0, 0, 1, 0, 0, 1);
  }
  if(countStep == 36 && millis() >= countTimeMillis + delayMode2){
    countStep = 37;
    countTimeMillis = millis();
    ledWrite(0, 0, 0, 0, 0, 0, 1, 0, 1);
  }
  if(countStep == 37 && millis() >= countTimeMillis + delayMode2){
    countStep = 38;
    countTimeMillis = millis();
    ledWrite(0, 0, 0, 0, 0, 0, 0, 1, 1);
  }
  //---------------------//  
  if(countStep == 38 && millis() >= countTimeMillis + delayMode2){
    countStep = 39;
    countTimeMillis = millis();
    ledWrite(1, 0, 0, 0, 0, 0, 0, 1, 1);
  }
  if(countStep == 32 + 7 && millis() >= countTimeMillis + delayMode2){
    countStep = 33 + 7;
    countTimeMillis = millis();
    ledWrite(0, 1, 0, 0, 0, 0, 0, 1, 1);
  }
  if(countStep == 33 + 7 && millis() >= countTimeMillis + delayMode2){
    countStep = 34 + 7;
    countTimeMillis = millis();
    ledWrite(0, 0, 1, 0, 0, 0, 0, 1, 1);
  }
  if(countStep == 34 + 7 && millis() >= countTimeMillis + delayMode2){
    countStep = 35 + 7;
    countTimeMillis = millis();
    ledWrite(0, 0, 0, 1, 0, 0, 0, 1, 1);
  }
  if(countStep == 35 + 7 && millis() >= countTimeMillis + delayMode2){
    countStep = 36 + 7;
    countTimeMillis = millis();
    ledWrite(0, 0, 0, 0, 1, 0, 0, 1, 1);
  }
  if(countStep == 36 + 7 && millis() >= countTimeMillis + delayMode2){
    countStep = 37 + 7;
    countTimeMillis = millis();
    ledWrite(0, 0, 0, 0, 0, 1, 0, 1, 1);
  }
  if(countStep == 37 + 7 && millis() >= countTimeMillis + delayMode2){
    countStep = 38 + 7;
    countTimeMillis = millis();
    ledWrite(0, 0, 0, 0, 0, 0, 1, 1, 1);
  }
  //---------------------//  
  if(countStep == 31 + 14 && millis() >= countTimeMillis + delayMode2){
    countStep = 32 + 14;
    countTimeMillis = millis();
    ledWrite(1, 0, 0, 0, 0, 0, 1, 1, 1);
  }
  if(countStep == 32 + 14 && millis() >= countTimeMillis + delayMode2){
    countStep = 33 + 14;
    countTimeMillis = millis();
    ledWrite(0, 1, 0, 0, 0, 0, 1, 1, 1);
  }
  if(countStep == 33 + 14 && millis() >= countTimeMillis + delayMode2){
    countStep = 34 + 14;
    countTimeMillis = millis();
    ledWrite(0, 0, 1, 0, 0, 0, 1, 1, 1);
  }
  if(countStep == 34 + 14 && millis() >= countTimeMillis + delayMode2){
    countStep = 35 + 14;
    countTimeMillis = millis();
    ledWrite(0, 0, 0, 1, 0, 0, 1, 1, 1);
  }
  if(countStep == 35 + 14 && millis() >= countTimeMillis + delayMode2){
    countStep = 36 + 14;
    countTimeMillis = millis();
    ledWrite(0, 0, 0, 0, 1, 0, 1, 1, 1);
  }
  if(countStep == 36 + 14 && millis() >= countTimeMillis + delayMode2){
    countStep = 37 + 14;
    countTimeMillis = millis();
    ledWrite(0, 0, 0, 0, 0, 1, 1, 1, 1);
  }
 
  
    //---------------------//  
  if(countStep == 43 + 7 && millis() >= countTimeMillis + delayMode2){
    countStep = 44 + 7;
    countTimeMillis = millis();
    ledWrite(1, 0, 0, 0, 0, 1, 1, 1, 1);
  }
  if(countStep == 44 + 7 && millis() >= countTimeMillis + delayMode2){
    countStep = 45 + 7;
    countTimeMillis = millis();
    ledWrite(0, 1, 0, 0, 0, 1, 1, 1, 1);
  }
  if(countStep == 45 + 7 && millis() >= countTimeMillis + delayMode2){
    countStep = 46 + 7;
    countTimeMillis = millis();
    ledWrite(0, 0, 1, 0, 0, 1, 1, 1, 1);
  }
  if(countStep == 46 + 7 && millis() >= countTimeMillis + delayMode2){
    countStep = 47 + 7;
    countTimeMillis = millis();
    ledWrite(0, 0, 0, 1, 0, 1, 1, 1, 1);
  }
  if(countStep == 47 + 7 && millis() >= countTimeMillis + delayMode2){
    countStep = 48 + 7;
    countTimeMillis = millis();
    ledWrite(0, 0, 0, 0, 1, 1, 1, 1, 1);
  }
  //---------------------//  
  if(countStep == 48 + 7 && millis() >= countTimeMillis + delayMode2){
    countStep = 49 + 7;
    countTimeMillis = millis();
    ledWrite(1, 0, 0, 0, 1, 1, 1, 1, 1);
  }
  if(countStep == 49 + 7 && millis() >= countTimeMillis + delayMode2){
    countStep = 50 + 7;
    countTimeMillis = millis();
    ledWrite(0, 1, 0, 0, 1, 1, 1, 1, 1);
  }
  if(countStep == 50 + 7 && millis() >= countTimeMillis + delayMode2){
    countStep = 51 + 7;
    countTimeMillis = millis();
    ledWrite(0, 0, 1, 0, 1, 1, 1, 1, 1);
  }
  if(countStep == 51 + 7 && millis() >= countTimeMillis + delayMode2){
    countStep = 52 + 7;
    countTimeMillis = millis();
    ledWrite(0, 0, 0, 1, 1, 1, 1, 1, 1);
  }
  //---------------------//  
  if(countStep == 52 + 7 && millis() >= countTimeMillis + delayMode2){
    countStep = 53 + 7;
    countTimeMillis = millis();
    ledWrite(1, 0, 0, 1, 1, 1, 1, 1, 1);
  }
  if(countStep == 53 + 7 && millis() >= countTimeMillis + delayMode2){
    countStep = 54 + 7;
    countTimeMillis = millis();
    ledWrite(0, 1, 0, 1, 1, 1, 1, 1, 1);
  }
  if(countStep == 54 + 7 && millis() >= countTimeMillis + delayMode2){
    countStep = 55 + 7;
    countTimeMillis = millis();
    ledWrite(0, 0, 1, 1, 1, 1, 1, 1, 1);
  }
  //---------------------//  
  if(countStep == 55 + 7 && millis() >= countTimeMillis + delayMode2){
    countStep = 56 + 7;
    countTimeMillis = millis();
    ledWrite(1, 0, 1, 1, 1, 1, 1, 1, 1);
  }
  if(countStep == 56 + 7 && millis() >= countTimeMillis + delayMode2){
    countStep = 57 + 7;
    countTimeMillis = millis();
    ledWrite(0, 1, 1, 1, 1, 1, 1, 1, 1);
  }
  //---------------------//  
  if(countStep == 57 + 7 && millis() >= countTimeMillis + delayMode2){
    countStep = 58 + 7;
    countTimeMillis = millis();
    ledWrite(1, 1, 1, 1, 1, 1, 1, 1, 1);
  }

  //------------------------------------------------------------------------------------------------//
  if(countStep == 71 - 6 && millis() >= countTimeMillis + 3*delayMode2){
    turnOffAll();
    countStep = 72 - 6;
    countTimeMillis = millis();
  }
  //------------------------------------------------------------------------------------------------//
  if(countStep == 72 - 6 && millis() >= countTimeMillis + 3*delayMode2){
    countStep = 73 - 6;
    countTimeMillis = millis();
    ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 1);
  }
  if(countStep == 73 - 6 && millis() >= countTimeMillis + delayMode2){
    countStep = 74 - 6;
    countTimeMillis = millis();
    ledWrite(0, 0, 0, 0, 0, 0, 0, 1, 0);
  }
  if(countStep == 74 - 6 && millis() >= countTimeMillis + delayMode2){
    countStep = 75 - 6;
    countTimeMillis = millis();
    ledWrite(0, 0, 0, 0, 0, 0, 1, 0, 0);
  }
  if(countStep == 75 - 6 && millis() >= countTimeMillis + delayMode2){
    countStep = 76 - 6;
    countTimeMillis = millis();
    ledWrite(0, 0, 0, 0, 0, 1, 0, 0, 0);
  }
  if(countStep == 76 - 6 && millis() >= countTimeMillis + delayMode2){
    countStep = 77 - 6;
    countTimeMillis = millis();
    ledWrite(0, 0, 0, 0, 1, 0, 0, 0, 0);
  }
  if(countStep == 77 - 6 && millis() >= countTimeMillis + delayMode2){
    countStep = 78 - 6;
    countTimeMillis = millis();
    ledWrite(0, 0, 0, 1, 0, 0, 0, 0, 0);
  }
  if(countStep == 78 - 6 && millis() >= countTimeMillis + delayMode2){
    countStep = 79 - 6;
    countTimeMillis = millis();
    ledWrite(0, 0, 1, 0, 0, 0, 0, 0, 0);
  }
  if(countStep == 79 - 6 && millis() >= countTimeMillis + delayMode2){
    countStep = 80 - 6;
    countTimeMillis = millis();
    ledWrite(0, 1, 0, 0, 0, 0, 0, 0, 0);
  }
  if(countStep == 80 - 6 && millis() >= countTimeMillis + delayMode2){
    countStep = 81 - 6;
    countTimeMillis = millis();
    ledWrite(1, 0, 0, 0, 0, 0, 0, 0, 0);
  }

  //---------------------//  
  if(countStep == 81 - 6 && millis() >= countTimeMillis + delayMode2){
    countStep = 82 - 6;
    countTimeMillis = millis();
    ledWrite(1, 0, 0, 0, 0, 0, 0, 0, 1);
  }
  if(countStep == 82 - 6 && millis() >= countTimeMillis + delayMode2){
    countStep = 83 - 6;
    countTimeMillis = millis();
    ledWrite(1, 0, 0, 0, 0, 0, 0, 1, 0);
  }
  if(countStep == 83 - 6 && millis() >= countTimeMillis + delayMode2){
    countStep = 84 - 6;
    countTimeMillis = millis();
    ledWrite(1, 0, 0, 0, 0, 0, 1, 0, 0);
  }
  if(countStep == 85 - 7 && millis() >= countTimeMillis + delayMode2){
    countStep = 86 - 7;
    countTimeMillis = millis();
    ledWrite(1, 0, 0, 0, 0, 1, 0, 0, 0);
  }
  if(countStep == 86 - 7 && millis() >= countTimeMillis + delayMode2){
    countStep = 87 - 7;
    countTimeMillis = millis();
    ledWrite(1, 0, 0, 0, 1, 0, 0, 0, 0);
  }
  if(countStep == 87 - 7 && millis() >= countTimeMillis + delayMode2){
    countStep = 88 - 7;
    countTimeMillis = millis();
    ledWrite(1, 0, 0, 1, 0, 0, 0, 0, 0);
  }
  if(countStep == 88 - 7 && millis() >= countTimeMillis + delayMode2){
    countStep = 89 - 7;
    countTimeMillis = millis();
    ledWrite(1, 0, 1, 0, 0, 0, 0, 0, 0);
  }
  if(countStep == 90 - 8 && millis() >= countTimeMillis + delayMode2){
    countStep = 91 - 8;
    countTimeMillis = millis();
    ledWrite(1, 1, 0, 0, 0, 0, 0, 0, 0);
  }

  //---------------------//  
  if(countStep == 91 - 8 && millis() >= countTimeMillis + delayMode2){
    countStep = 92 - 8;
    countTimeMillis = millis();
    ledWrite(1, 1, 0, 0, 0, 0, 0, 0, 1);
  }
  if(countStep == 92 - 8 && millis() >= countTimeMillis + delayMode2){
    countStep = 93 - 8;
    countTimeMillis = millis();
    ledWrite(1, 1, 0, 0, 0, 0, 0, 1, 0);
  }
  if(countStep == 93 - 8 && millis() >= countTimeMillis + delayMode2){
    countStep = 94 - 8;
    countTimeMillis = millis();
    ledWrite(1, 1, 0, 0, 0, 0, 1, 0, 0);
  }
  if(countStep == 95 - 9 && millis() >= countTimeMillis + delayMode2){
    countStep = 96 - 9;
    countTimeMillis = millis();
    ledWrite(1, 1, 0, 0, 0, 1, 0, 0, 0);
  }
  if(countStep == 96 - 9 && millis() >= countTimeMillis + delayMode2){
    countStep = 97 - 9;
    countTimeMillis = millis();
    ledWrite(1, 1, 0, 0, 1, 0, 0, 0, 0);
  }
  if(countStep == 97 - 9 && millis() >= countTimeMillis + delayMode2){
    countStep = 98 - 9;
    countTimeMillis = millis();
    ledWrite(1, 1, 0, 1, 0, 0, 0, 0, 0);
  }
  if(countStep == 98 - 9 && millis() >= countTimeMillis + delayMode2){
    countStep = 99 - 9;
    countTimeMillis = millis();
    ledWrite(1, 1, 1, 0, 0, 0, 0, 0, 0);
  }
  //---------------------//  
  if(countStep == 99 - 9 && millis() >= countTimeMillis + delayMode2){
    countStep = 100 - 9;
    countTimeMillis = millis();
    ledWrite(1, 1, 1, 0, 0, 0, 0, 0, 1);
  }
  if(countStep == 100 - 9 && millis() >= countTimeMillis + delayMode2){
    countStep = 101 - 9;
    countTimeMillis = millis();
    ledWrite(1, 1, 1, 0, 0, 0, 0, 1, 0);
  }
  if(countStep == 101 - 9 && millis() >= countTimeMillis + delayMode2){
    countStep = 102 - 9;
    countTimeMillis = millis();
    ledWrite(1, 1, 1, 0, 0, 0, 1, 0, 0);
  }
  if(countStep == 102 - 9 && millis() >= countTimeMillis + delayMode2){
    countStep = 103 - 9;
    countTimeMillis = millis();
    ledWrite(1, 1, 1, 0, 0, 1, 0, 0, 0);
  }
  if(countStep == 103 - 9 && millis() >= countTimeMillis + delayMode2){
    countStep = 104 - 9;
    countTimeMillis = millis();
    ledWrite(1, 1, 1, 0, 1, 0, 0, 0, 0);
  }
  if(countStep == 104 - 9 && millis() >= countTimeMillis + delayMode2){
    countStep = 105 - 9;
    countTimeMillis = millis();
    ledWrite(1, 1, 1, 1, 0, 0, 0, 0, 0);
  }
  //---------------------//  
  if(countStep == 105 - 9 && millis() >= countTimeMillis + delayMode2){
    countStep = 106 - 9;
    countTimeMillis = millis();
    ledWrite(1, 1, 1, 1, 0, 0, 0, 0, 1);
  }
  if(countStep == 106 - 9 && millis() >= countTimeMillis + delayMode2){
    countStep = 107 - 9;
    countTimeMillis = millis();
    ledWrite(1, 1, 1, 1, 0, 0, 0, 1, 0);
  }
  if(countStep == 107 - 9 && millis() >= countTimeMillis + delayMode2){
    countStep = 108 - 9;
    countTimeMillis = millis();
    ledWrite(1, 1, 1, 1, 0, 0, 1, 0, 0);
  }
  if(countStep == 108 - 9 && millis() >= countTimeMillis + delayMode2){
    countStep = 109 - 9;
    countTimeMillis = millis();
    ledWrite(1, 1, 1, 1, 0, 1, 0, 0, 0);
  }
  if(countStep == 109 - 9 && millis() >= countTimeMillis + delayMode2){
    countStep = 110 - 9;
    countTimeMillis = millis();
    ledWrite(1, 1, 1, 1, 1, 0, 0, 0, 0);
  }
  //---------------------//  
  if(countStep == 110 - 9 && millis() >= countTimeMillis + delayMode2){
    countStep = 111 - 9;
    countTimeMillis = millis();
    ledWrite(1, 1, 1, 1, 1, 0, 0, 0, 1);
  }
  if(countStep == 111 - 9 && millis() >= countTimeMillis + delayMode2){
    countStep = 112 - 9;
    countTimeMillis = millis();
    ledWrite(1, 1, 1, 1, 1, 0, 0, 1, 0);
  }
  if(countStep == 112 - 9 && millis() >= countTimeMillis + delayMode2){
    countStep = 113 - 9;
    countTimeMillis = millis();
    ledWrite(1, 1, 1, 1, 1, 0, 1, 0, 0);
  }
  if(countStep == 113 - 9 && millis() >= countTimeMillis + delayMode2){
    countStep = 114 - 9;
    countTimeMillis = millis();
    ledWrite(1, 1, 1, 1, 1, 1, 0, 0, 0);
  }
  //---------------------//  
  if(countStep == 114 - 9 && millis() >= countTimeMillis + delayMode2){
    countStep = 115 - 9;
    countTimeMillis = millis();
    ledWrite(1, 1, 1, 1, 1, 1, 0, 0, 1);
  }
  if(countStep == 115 - 9 && millis() >= countTimeMillis + delayMode2){
    countStep = 116 - 9;
    countTimeMillis = millis();
    ledWrite(1, 1, 1, 1, 1, 1, 0, 1, 0);
  }
  if(countStep == 116 - 9 && millis() >= countTimeMillis + delayMode2){
    countStep = 117 - 9;
    countTimeMillis = millis();
    ledWrite(1, 1, 1, 1, 1, 1, 1, 0, 0);
  }
  //---------------------//  
  if(countStep == 117 - 9 && millis() >= countTimeMillis + delayMode2){
    countStep = 118 - 9;
    countTimeMillis = millis();
    ledWrite(1, 1, 1, 1, 1, 1, 1, 0, 1);
  }
  if(countStep == 118 - 9 && millis() >= countTimeMillis + delayMode2){
    countStep = 119 - 9;
    countTimeMillis = millis();
    ledWrite(1, 1, 1, 1, 1, 1, 1, 1, 0);
  }
  //---------------------//  
  if(countStep == 119 - 9 && millis() >= countTimeMillis + 3*delayMode2){
    countStep = 120 - 9;
    countTimeMillis = millis();
    ledWrite(1, 1, 1, 1, 1, 1, 1, 1, 1);
  }
  //------------------------------------------------------------------------------------------------//
  if(countStep == 120 - 9 && millis() >= countTimeMillis + 3*delayMode2){
    turnOffAll();
    countStep = 121 - 9;
    countTimeMillis = millis();
  }
  //------------------------------------------------------------------------------------------------//
  if(countStep == 121 - 9 && millis() >= countTimeMillis + delayMode2){
    countStep = 0;
    statusModeRunning = 3;
    countTimeMillis = millis();
  }

}

void mode3(){
  if(countStepMode3 < 5){
    if(countStep == 0 && millis() >= countTimeMillis + delayMode3){
      countStep = 1;
      countTimeMillis = millis();
      ledWrite(1, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 1 && millis() >= countTimeMillis + delayMode3){
      countStep = 2;
      countTimeMillis = millis();
      ledWrite(0, 1, 0, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 2 && millis() >= countTimeMillis + delayMode3){
      countStep = 3;
      countTimeMillis = millis();
      ledWrite(0, 0, 1, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 3 && millis() >= countTimeMillis + delayMode3){
      countStep = 4;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 1, 0, 0, 0, 0, 0);
    }
    if(countStep == 4 && millis() >= countTimeMillis + delayMode3){
      countStep = 5;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 1, 0, 0, 0, 0);
    }
    if(countStep == 5 && millis() >= countTimeMillis + delayMode3){
      countStep = 6;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 1, 0, 0, 0);
    }
    if(countStep == 6 && millis() >= countTimeMillis + delayMode3){
      countStep = 7;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 1, 0, 0);
    }
    if(countStep == 7 && millis() >= countTimeMillis + delayMode3){
      countStep = 8;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 1, 0);
    }
    if(countStep == 8 && millis() >= countTimeMillis + delayMode3){
      countStepMode3++;
      countStep = 0;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 1);
    }
  }
  
  //---------------------// 
  if(countStepMode3 >=5 && countStepMode3 < 10){
    if(countStep == 0 && millis() >= countTimeMillis + delayMode3){
      countStep = 1;
      countTimeMillis = millis();
      ledWrite(1, 1, 0, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 1 && millis() >= countTimeMillis + delayMode3){
      countStep = 2;
      countTimeMillis = millis();
      ledWrite(0, 1, 1, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 2 && millis() >= countTimeMillis + delayMode3){
      countStep = 3;
      countTimeMillis = millis();
      ledWrite(0, 0, 1, 1, 0, 0, 0, 0, 0);
    }
    if(countStep == 3 && millis() >= countTimeMillis + delayMode3){
      countStep = 4;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 1, 1, 0, 0, 0, 0);
    }
    if(countStep == 4 && millis() >= countTimeMillis + delayMode3){
      countStep = 5;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 1, 1, 0, 0, 0);
    }
    if(countStep == 5 && millis() >= countTimeMillis + delayMode3){
      countStep = 6;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 1, 1, 0, 0);
    }
    if(countStep == 6 && millis() >= countTimeMillis + delayMode3){
      countStep = 7;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 1, 1, 0);
    }
    if(countStep == 7 && millis() >= countTimeMillis + delayMode3){
      countStep = 8;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 1, 1);
    }
    if(countStep == 8 && millis() >= countTimeMillis + delayMode3){
      countStepMode3++;
      countStep = 0;
      countTimeMillis = millis();
      ledWrite(1, 0, 0, 0, 0, 0, 0, 0, 1);
    }
  }

  //---------------------// 
  if(countStepMode3 >=10 && countStepMode3 < 15){
    if(countStep == 0 && millis() >= countTimeMillis + delayMode3){
      countStep = 1;
      countTimeMillis = millis();
      ledWrite(1, 1, 1, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 1 && millis() >= countTimeMillis + delayMode3){
      countStep = 2;
      countTimeMillis = millis();
      ledWrite(0, 1, 1, 1, 0, 0, 0, 0, 0);
    }
    if(countStep == 2 && millis() >= countTimeMillis + delayMode3){
      countStep = 3;
      countTimeMillis = millis();
      ledWrite(0, 0, 1, 1, 1, 0, 0, 0, 0);
    }
    if(countStep == 3 && millis() >= countTimeMillis + delayMode3){
      countStep = 4;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 1, 1, 1, 0, 0, 0);
    }
    if(countStep == 4 && millis() >= countTimeMillis + delayMode3){
      countStep = 5;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 1, 1, 1, 0, 0);
    }
    if(countStep == 5 && millis() >= countTimeMillis + delayMode3){
      countStep = 6;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 1, 1, 1, 0);
    }
    if(countStep == 6 && millis() >= countTimeMillis + delayMode3){
      countStep = 7;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 1, 1, 1);
    }
    if(countStep == 7 && millis() >= countTimeMillis + delayMode3){
      countStep = 8;
      countTimeMillis = millis();
      ledWrite(1, 0, 0, 0, 0, 0, 0, 1, 1);
    }
    if(countStep == 8 && millis() >= countTimeMillis + delayMode3){
      
      countStepMode3++;
      countStep = 0;
      countTimeMillis = millis();
      ledWrite(1, 1, 0, 0, 0, 0, 0, 0, 1);
      if(countStepMode3 == 14){
        countStepMode3 = 0;
        statusModeRunning = 4;
        turnOffAll();
      }
    }
  }

}

void mode4(){
  if(countStepMode4 == 1){
    if(countStep == 0 && millis() >= countTimeMillis + delayMode4){
      countStep = 1;
      countTimeMillis = millis();
      ledWrite(1, 1, 1, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 1 && millis() >= countTimeMillis + delayMode4){
      countStep = 2;
      countTimeMillis = millis();
      ledWrite(0, 1, 1, 1, 0, 0, 0, 0, 0);
    }
    if(countStep == 2 && millis() >= countTimeMillis + delayMode4){
      countStep = 3;
      countTimeMillis = millis();
      ledWrite(0, 0, 1, 1,1, 0, 0, 0, 0);
    }
    if(countStep == 3 && millis() >= countTimeMillis + delayMode4){
      countStep = 4;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 1, 1, 1, 0, 0, 0);
    }
    if(countStep == 4 && millis() >= countTimeMillis + delayMode4){
      countStep = 5;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 1, 1, 1, 0, 0);
    }
    if(countStep == 5 && millis() >= countTimeMillis + delayMode4){
      countStep = 6;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 1, 1, 1, 0);
    }
    if(countStep == 6 && millis() >= countTimeMillis + delayMode4){
      countStep = 7;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 1, 1, 1);
    }
    if(countStep == 7 && millis() >= countTimeMillis + delayMode4){
      countStep = 8;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 1, 1);
    }
    if(countStep == 8 && millis() >= countTimeMillis + delayMode4){
      countStep = 9;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 1);
    }
    if(countStep == 9 && millis() >= countTimeMillis + delayMode4){
      countStep = 10;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    //---------------------// 
    if(countStep == 10 && millis() >= countTimeMillis + (35-10)*delayMode4){
      countStep = 11;
      countTimeMillis = millis();
      ledWrite(1, 1, 1, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 11 && millis() >= countTimeMillis + delayMode4){
      countStep = 12;
      countTimeMillis = millis();
      ledWrite(0, 1, 1, 1, 0, 0, 0, 0, 0);
    }
    if(countStep == 12 && millis() >= countTimeMillis + delayMode4){
      countStep = 13;
      countTimeMillis = millis();
      ledWrite(0, 0, 1, 1,1, 0, 0, 0, 0);
    }
    if(countStep == 13 && millis() >= countTimeMillis + delayMode4){
      countStep = 14;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 1, 1, 1, 0, 0, 0);
    }
    if(countStep == 14 && millis() >= countTimeMillis + delayMode4){
      countStep = 15;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 1, 1, 1, 0, 0);
    }
    if(countStep == 15 && millis() >= countTimeMillis + delayMode4){
      countStep = 16;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 1, 1, 1, 0);
    }
    if(countStep == 16 && millis() >= countTimeMillis + delayMode4){
      countStep = 17;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 1, 1, 1);
    }
    if(countStep == 17 && millis() >= countTimeMillis + delayMode4){
      countStep = 18;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 1, 1);
    }
    if(countStep == 18 && millis() >= countTimeMillis + delayMode4){
      countStep = 19;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 1);
    }
    if(countStep == 19 && millis() >= countTimeMillis + delayMode4){
      countStep = 20;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    //---------------------// 
    if(countStep == 20 && millis() >= countTimeMillis + (32-10)*delayMode4){
      countStep = 21;
      countTimeMillis = millis();
      ledWrite(1, 1, 1, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 21 && millis() >= countTimeMillis + delayMode4){
      countStep = 22;
      countTimeMillis = millis();
      ledWrite(0, 1, 1, 1, 0, 0, 0, 0, 0);
    }
    if(countStep == 22 && millis() >= countTimeMillis + delayMode4){
      countStep = 23;
      countTimeMillis = millis();
      ledWrite(0, 0, 1, 1,1, 0, 0, 0, 0);
    }
    if(countStep == 23 && millis() >= countTimeMillis + delayMode4){
      countStep = 24;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 1, 1, 1, 0, 0, 0);
    }
    if(countStep == 24 && millis() >= countTimeMillis + delayMode4){
      countStep = 25;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 1, 1, 1, 0, 0);
    }
    if(countStep == 25 && millis() >= countTimeMillis + delayMode4){
      countStep = 26;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 1, 1, 1, 0);
    }
    if(countStep == 26 && millis() >= countTimeMillis + delayMode4){
      countStep = 27;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 1, 1, 1);
    }
    if(countStep == 27 && millis() >= countTimeMillis + delayMode4){
      countStep = 28;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 1, 1);
    }
    if(countStep == 28 && millis() >= countTimeMillis + delayMode4){
      countStep = 29;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 1);
    }
    if(countStep == 29 && millis() >= countTimeMillis + delayMode4){
      countStep = 30;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
  //---------------------//     //het hang 4
    if(countStep == 30 && millis() >= countTimeMillis + (29-10)*delayMode4){
      countStep = 31;
      countTimeMillis = millis();
      ledWrite(1, 1, 1, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 31 && millis() >= countTimeMillis + delayMode4){
      countStep = 32;
      countTimeMillis = millis();
      ledWrite(0, 1, 1, 1, 0, 0, 0, 0, 0);
    }
    if(countStep == 32 && millis() >= countTimeMillis + delayMode4){
      countStep = 33;
      countTimeMillis = millis();
      ledWrite(0, 0, 1, 1,1, 0, 0, 0, 0);
    }
    if(countStep == 33 && millis() >= countTimeMillis + delayMode4){
      countStep = 34;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 1, 1, 1, 0, 0, 0);
    }
    if(countStep == 34 && millis() >= countTimeMillis + delayMode4){
      countStep = 35;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 1, 1, 1, 0, 0);
    }
    if(countStep == 35 && millis() >= countTimeMillis + delayMode4){
      countStep = 36;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 1, 1, 1, 0);
    }
    if(countStep == 36 && millis() >= countTimeMillis + delayMode4){
      countStep = 37;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 1, 1, 1);
    }
    if(countStep == 37 && millis() >= countTimeMillis + delayMode4){
      countStep = 38;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 1, 1);
    }
    if(countStep == 38 && millis() >= countTimeMillis + delayMode4){
      countStep = 39;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 1);
    }
    if(countStep == 39 && millis() >= countTimeMillis + delayMode4){
      countStep = 40;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    //---------------------// 
    if(countStep == 40 && millis() >= countTimeMillis + (26-10)*delayMode4){
      countStep = 41;
      countTimeMillis = millis();
      ledWrite(1, 1, 1, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 41 && millis() >= countTimeMillis + delayMode4){
      countStep = 42;
      countTimeMillis = millis();
      ledWrite(0, 1, 1, 1, 0, 0, 0, 0, 0);
    }
    if(countStep == 42 && millis() >= countTimeMillis + delayMode4){
      countStep = 43;
      countTimeMillis = millis();
      ledWrite(0, 0, 1, 1,1, 0, 0, 0, 0);
    }
    if(countStep == 43 && millis() >= countTimeMillis + delayMode4){
      countStep = 44;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 1, 1, 1, 0, 0, 0);
    }
    if(countStep == 44 && millis() >= countTimeMillis + delayMode4){
      countStep = 45;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 1, 1, 1, 0, 0);
    }
    if(countStep == 45 && millis() >= countTimeMillis + delayMode4){
      countStep = 46;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 1, 1, 1, 0);
    }
    if(countStep == 46 && millis() >= countTimeMillis + delayMode4){
      countStep = 47;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 1, 1, 1);
    }
    if(countStep == 47 && millis() >= countTimeMillis + delayMode4){
      countStep = 48;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 1, 1);
    }
    if(countStep == 48 && millis() >= countTimeMillis + delayMode4){
      countStep = 49;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 1);
    }
    if(countStep == 49 && millis() >= countTimeMillis + delayMode4){
      countStep = 50;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
      //---------------------// 
    if(countStep == 50 && millis() >= countTimeMillis + (23-10)*delayMode4){
      countStep = 51;
      countTimeMillis = millis();
      ledWrite(1, 1, 1, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 51 && millis() >= countTimeMillis + delayMode4){
      countStep = 52;
      countTimeMillis = millis();
      ledWrite(0, 1, 1, 1, 0, 0, 0, 0, 0);
    }
    if(countStep == 52 && millis() >= countTimeMillis + delayMode4){
      countStep = 53;
      countTimeMillis = millis();
      ledWrite(0, 0, 1, 1,1, 0, 0, 0, 0);
    }
    if(countStep == 53 && millis() >= countTimeMillis + delayMode4){
      countStep = 54;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 1, 1, 1, 0, 0, 0);
    }
    if(countStep == 54 && millis() >= countTimeMillis + delayMode4){
      countStep = 55;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 1, 1, 1, 0, 0);
    }
    if(countStep == 55 && millis() >= countTimeMillis + delayMode4){
      countStep = 56;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 1, 1, 1, 0);
    }
    if(countStep == 56 && millis() >= countTimeMillis + delayMode4){
      countStep = 57;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 1, 1, 1);
    }
    if(countStep == 57 && millis() >= countTimeMillis + delayMode4){
      countStep = 58;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 1, 1);
    }
    if(countStep == 58 && millis() >= countTimeMillis + delayMode4){
      countStep = 59;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 1);
    }
    if(countStep == 59 && millis() >= countTimeMillis + delayMode4){
      countStep = 60;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
  
      //---------------------// 
    if(countStep == 60 && millis() >= countTimeMillis + (20-10)*delayMode4){
      countStep = 61;
      countTimeMillis = millis();
      ledWrite(1, 1, 1, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 61 && millis() >= countTimeMillis + delayMode4){
      countStep = 62;
      countTimeMillis = millis();
      ledWrite(0, 1, 1, 1, 0, 0, 0, 0, 0);
    }
    if(countStep == 62 && millis() >= countTimeMillis + delayMode4){
      countStep = 63;
      countTimeMillis = millis();
      ledWrite(0, 0, 1, 1,1, 0, 0, 0, 0);
    }
    if(countStep == 63 && millis() >= countTimeMillis + delayMode4){
      countStep = 64;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 1, 1, 1, 0, 0, 0);
    }
    if(countStep == 64 && millis() >= countTimeMillis + delayMode4){
      countStep = 65;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 1, 1, 1, 0, 0);
    }
    if(countStep == 65 && millis() >= countTimeMillis + delayMode4){
      countStep = 66;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 1, 1, 1, 0);
    }
    if(countStep == 66 && millis() >= countTimeMillis + delayMode4){
      countStep = 67;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 1, 1, 1);
    }
    if(countStep == 67 && millis() >= countTimeMillis + delayMode4){
      countStep = 68;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 1, 1);
    }
    if(countStep == 68 && millis() >= countTimeMillis + delayMode4){
      countStep = 69;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 1);
    }
    if(countStep == 69 && millis() >= countTimeMillis + delayMode4){
      countStep = 70;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    //---------------------// 
    if(countStep == 70 && millis() >= countTimeMillis + (17-10)*delayMode4){
      countStep = 71;
      countTimeMillis = millis();
      ledWrite(1, 1, 1, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 71 && millis() >= countTimeMillis + delayMode4){
      countStep = 72;
      countTimeMillis = millis();
      ledWrite(0, 1, 1, 1, 0, 0, 0, 0, 0);
    }
    if(countStep == 72 && millis() >= countTimeMillis + delayMode4){
      countStep = 73;
      countTimeMillis = millis();
      ledWrite(0, 0, 1, 1,1, 0, 0, 0, 0);
    }
    if(countStep == 73 && millis() >= countTimeMillis + delayMode4){
      countStep = 74;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 1, 1, 1, 0, 0, 0);
    }
    if(countStep == 74 && millis() >= countTimeMillis + delayMode4){
      countStep = 75;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 1, 1, 1, 0, 0);
    }
    if(countStep == 75 && millis() >= countTimeMillis + delayMode4){
      countStep = 76;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 1, 1, 1, 0);
    }
    if(countStep == 76 && millis() >= countTimeMillis + delayMode4){
      countStep = 77;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 1, 1, 1);
    }
    if(countStep == 77 && millis() >= countTimeMillis + delayMode4){
      countStep = 78;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 1, 1);
    }
    if(countStep == 78 && millis() >= countTimeMillis + delayMode4){
      countStep = 79;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 1);
    }
    if(countStep == 79 && millis() >= countTimeMillis + delayMode4){
      countStep = 80;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
  
    //---------------------// 
    if(countStep == 80 && millis() >= countTimeMillis + (14-10)*delayMode4){
      countStep = 81;
      countTimeMillis = millis();
      ledWrite(1, 1, 1, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 81 && millis() >= countTimeMillis + delayMode4){
      countStep = 82;
      countTimeMillis = millis();
      ledWrite(0, 1, 1, 1, 0, 0, 0, 0, 0);
    }
    if(countStep == 82 && millis() >= countTimeMillis + delayMode4){
      countStep = 83;
      countTimeMillis = millis();
      ledWrite(0, 0, 1, 1,1, 0, 0, 0, 0);
    }
    if(countStep == 83 && millis() >= countTimeMillis + delayMode4){
      countStep = 84;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 1, 1, 1, 0, 0, 0);
    }
    if(countStep == 84 && millis() >= countTimeMillis + delayMode4){
      countStep = 85;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 1, 1, 1, 0, 0);
    }
    if(countStep == 85 && millis() >= countTimeMillis + delayMode4){
      countStep = 86;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 1, 1, 1, 0);
    }
    if(countStep == 86 && millis() >= countTimeMillis + delayMode4){
      countStep = 87;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 1, 1, 1);
    }
    if(countStep == 87 && millis() >= countTimeMillis + delayMode4){
      countStep = 88;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 1, 1);
    }
    if(countStep == 88 && millis() >= countTimeMillis + delayMode4){
      countStep = 89;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 1);
    }
    if(countStep == 89 && millis() >= countTimeMillis + delayMode4){
      countStep = 90;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    //---------------------//     //het luot 2
    if(countStep == 90 && millis() >= countTimeMillis + (11-10)*delayMode4){
      countStep = 91;
      countTimeMillis = millis();
      ledWrite(1, 1, 1, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 91 && millis() >= countTimeMillis + delayMode4){
      countStep = 92;
      countTimeMillis = millis();
      ledWrite(0, 1, 1, 1, 0, 0, 0, 0, 0);
    }
    if(countStep == 92 && millis() >= countTimeMillis + delayMode4){
      countStep = 93;
      countTimeMillis = millis();
      ledWrite(0, 0, 1, 1,1, 0, 0, 0, 0);
    }
    if(countStep == 93 && millis() >= countTimeMillis + delayMode4){
      countStep = 94;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 1, 1, 1, 0, 0, 0);
    }
    if(countStep == 94 && millis() >= countTimeMillis + delayMode4){
      countStep = 95;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 1, 1, 1, 0, 0);
    }
    if(countStep == 95 && millis() >= countTimeMillis + delayMode4){
      countStep = 96;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 1, 1, 1, 0);
    }
    if(countStep == 96 && millis() >= countTimeMillis + delayMode4){
      countStep = 97;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 1, 1, 1);
    }
    //---------------------// 
    if(countStep == 97 && millis() >= countTimeMillis + (8-7)*delayMode4){
      countStep = 98;
      countTimeMillis = millis();
      ledWrite(1, 1, 1, 0, 0, 0, 1, 1, 1);
    }
    if(countStep == 98 && millis() >= countTimeMillis + delayMode4){
      countStep = 99;
      countTimeMillis = millis();
      ledWrite(0, 1, 1, 1, 0, 0, 1, 1, 1);
    }
    if(countStep == 99 && millis() >= countTimeMillis + delayMode4){
      countStep = 100;
      countTimeMillis = millis();
      ledWrite(0, 0, 1, 1,1, 0, 1, 1, 1);
    }
    if(countStep == 100 && millis() >= countTimeMillis + delayMode4){
      countStep = 101;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 1, 1, 1, 1, 1, 1);
    }
    //---------------------// 
    if(countStep == 101 && millis() >= countTimeMillis + (8-7)*delayMode4){
      countStep = 102;
      countTimeMillis = millis();
      ledWrite(1, 1, 1, 1, 1, 1, 1, 1, 1);
    }
  
    //---------------------// 
    if(countStep == 102 && millis() >= countTimeMillis + 5*delayMode4){
      countStep = 103;
      countTimeMillis = millis();
      turnOffAll();
    }
    if(countStep == 103 && millis() >= countTimeMillis + 5*delayMode4){
      countStep = 104;
      countTimeMillis = millis();
      turnOnAll();
    }
    if(countStep == 104 && millis() >= countTimeMillis + 5*delayMode4){
      countStep = 105;
      countTimeMillis = millis();
      turnOffAll();
    }
    if(countStep == 105 && millis() >= countTimeMillis + 5*delayMode4){
      countStep = 106;
      countTimeMillis = millis();
      turnOnAll();
    }
    if(countStep == 106 && millis() >= countTimeMillis + 5*delayMode4){
      countStep = 0;
      countStepMode4 = 2;
      countTimeMillis = millis();
      turnOffAll();
    }
  }
    

  //------------------------------------------------------------------------------------------------// //het luot 1

  if(countStepMode4 == 2){
    if(countStep == 0 && millis() >= countTimeMillis + 26*delayMode4){
      countStep = 1;
      countTimeMillis = millis();
      ledWriteInverse(1, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 1 && millis() >= countTimeMillis + delayMode4){
      countStep = 2;
      countTimeMillis = millis();
      ledWriteInverse(1, 1, 0, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 2 && millis() >= countTimeMillis + delayMode4){
      countStep = 3;
      countTimeMillis = millis();
      ledWriteInverse(1, 1, 1, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 3 && millis() >= countTimeMillis + delayMode4){
      countStep = 4;
      countTimeMillis = millis();
      ledWriteInverse(0, 1, 1, 1, 0, 0, 0, 0, 0);
    }
    if(countStep == 4 && millis() >= countTimeMillis + delayMode4){
      countStep = 5;
      countTimeMillis = millis();
      ledWriteInverse(0, 0, 1, 1, 1, 0, 0, 0, 0);
    }
    if(countStep == 5 && millis() >= countTimeMillis + delayMode4){
      countStep = 6;
      countTimeMillis = millis();
      ledWriteInverse(0, 0, 0, 1, 1, 1, 0, 0, 0);
    }
    if(countStep == 6 && millis() >= countTimeMillis + delayMode4){
      countStep = 7;
      countTimeMillis = millis();
      ledWriteInverse(0, 0, 0, 0, 1, 1, 1, 0, 0);
    }
    if(countStep == 7 && millis() >= countTimeMillis + delayMode4){
      countStep = 8;
      countTimeMillis = millis();
      ledWriteInverse(0, 0, 0, 0, 0, 1, 1, 1, 0);
    }
    if(countStep == 8 && millis() >= countTimeMillis + delayMode4){
      countStep = 9;
      countTimeMillis = millis();
      ledWriteInverse(0, 0, 0, 0, 0, 0, 1, 1, 1);
    }   //34
    //---------------------// 
    if(countStep == 9 && millis() >= countTimeMillis + (32-9+3)*delayMode4){
      countStep = 10;
      countTimeMillis = millis();
      ledWriteInverse(1, 0, 0, 0, 0, 0, 1, 1, 1);
    }
    if(countStep == 10 && millis() >= countTimeMillis + delayMode4){
      countStep = 11;
      countTimeMillis = millis();
      ledWriteInverse(1, 1, 0, 0, 0, 0, 1, 1, 1);
    }
    if(countStep == 11 && millis() >= countTimeMillis + delayMode4){
      countStep = 12;
      countTimeMillis = millis();
      ledWriteInverse(1, 1, 1, 0, 0, 0, 1, 1, 1);
    }
    if(countStep == 12 && millis() >= countTimeMillis + delayMode4){
      countStep = 13;
      countTimeMillis = millis();
      ledWriteInverse(0, 1, 1, 1, 0, 0, 1, 1, 1);
    }
    if(countStep == 13 && millis() >= countTimeMillis + delayMode4){
      countStep = 14;
      countTimeMillis = millis();
      ledWriteInverse(0, 0, 1, 1, 1, 0, 1, 1, 1);
    }
    if(countStep == 14 && millis() >= countTimeMillis + delayMode4){
      countStep = 15;
      countTimeMillis = millis();
      ledWriteInverse(0, 0, 0, 1, 1, 1, 1, 1, 1);
    }
    //---------------------// 
    if(countStep == 15 && millis() >= countTimeMillis + (32-9+3)*delayMode4){
      countStep = 16;
      countTimeMillis = millis();
      ledWriteInverse(1, 0, 0, 1, 1, 1, 1, 1, 1);
    }
    if(countStep == 16 && millis() >= countTimeMillis + delayMode4){
      countStep = 17;
      countTimeMillis = millis();
      ledWriteInverse(1, 1, 0, 1, 1, 1, 1, 1, 1);
    }
    if(countStep == 17 && millis() >= countTimeMillis + delayMode4){
      countStep = 18;
      countTimeMillis = millis();
      ledWriteInverse(1, 1, 1, 1, 1, 1, 1, 1, 1);
    }
    if(countStep == 18 && millis() >= countTimeMillis + (116+1)*delayMode4){
      countStep = 19;
      countTimeMillis = millis();
    }
  
    //---------------------// 
    if(countStep == 19 && millis() >= countTimeMillis + 5*delayMode4){
      countStep = 20;
      countTimeMillis = millis();
      turnOffAll();
    }
    if(countStep == 20 && millis() >= countTimeMillis + 5*delayMode4){
      countStep = 21;
      countTimeMillis = millis();
      turnOnAll();
    }
    if(countStep == 21 && millis() >= countTimeMillis + 5*delayMode4){
      countStep = 22;
      countTimeMillis = millis();
      turnOffAll();
    }
    if(countStep == 22 && millis() >= countTimeMillis + 5*delayMode4){
      countStep = 23;
      countTimeMillis = millis();
      turnOnAll();
    }
    if(countStep == 23 && millis() >= countTimeMillis + 5*delayMode4){
      countStep = 24;
      countTimeMillis = millis();
      turnOffAll();
    }
    if(countStep == 24 && millis() >= countTimeMillis + 5*delayMode4){
      countStep = 0;
      statusModeRunning = 5;
      countStepMode4 = 1;
      countTimeMillis = millis();
    }
  }
}

void mode5(){
  if(countStep == 0 && millis() >= countTimeMillis + delayMode5){
    countStep = 1;
    countTimeMillis = millis();
    digitalWrite(L1, HIGH);
  }
  if(countStep == 1 && millis() >= countTimeMillis + delayMode5){
    countStep = 2;
    countTimeMillis = millis();
    digitalWrite(L2, HIGH);
  }
  if(countStep == 2 && millis() >= countTimeMillis + delayMode5){
    countStep = 3;
    countTimeMillis = millis();
    digitalWrite(L3, HIGH);
  }
  if(countStep == 3 && millis() >= countTimeMillis + delayMode5){
    countStep = 4;
    countTimeMillis = millis();
    digitalWrite(L4, HIGH);
  }
  if(countStep == 4 && millis() >= countTimeMillis + delayMode5){
    countStep = 5;
    countTimeMillis = millis();
    digitalWrite(L5, HIGH);
  }
  if(countStep == 5 && millis() >= countTimeMillis + delayMode5){
    countStep = 6;
    countTimeMillis = millis();
    digitalWrite(L6, HIGH);
  }
  if(countStep == 6 && millis() >= countTimeMillis + delayMode5){
    countStep = 7;
    countTimeMillis = millis();
    digitalWrite(L7, HIGH);
  }
  if(countStep == 7 && millis() >= countTimeMillis + delayMode5){
    countStep = 8;
    countTimeMillis = millis();
    digitalWrite(L8, HIGH);
  }
  if(countStep == 8 && millis() >= countTimeMillis + delayMode5){
    countStep = 9;
    countTimeMillis = millis();
    digitalWrite(L9, HIGH);
  }
  if(countStep == 9 && millis() >= countTimeMillis + 9*delayMode5){
    countStep = 10;
    countTimeMillis = millis();
  }
  //---------------------// 
  if(countStep == 10 && millis() >= countTimeMillis + 5*delayMode5){
    countStep = 11;
    countTimeMillis = millis();
  }
  if(countStep == 11 && millis() >= countTimeMillis + 5*delayMode5){
    countStep = 12;
    countTimeMillis = millis();
    turnOffAll();
  }
  if(countStep == 12 && millis() >= countTimeMillis + 5*delayMode5){
    countStep = 13;
    countTimeMillis = millis();
  }
  //---------------------// 
  if(countStep == 13 && millis() >= countTimeMillis + 9*delayMode5){
    countStep = 14;
    countTimeMillis = millis();
  }
  if(countStep == 14 && millis() >= countTimeMillis + delayMode5){
    countStep = 15;
    countTimeMillis = millis();
    digitalWrite(L9, HIGH);
  }
  if(countStep == 15 && millis() >= countTimeMillis + delayMode5){
    countStep = 16;
    countTimeMillis = millis();
    digitalWrite(L8, HIGH);
  }
  if(countStep == 16 && millis() >= countTimeMillis + delayMode5){
    countStep = 17;
    countTimeMillis = millis();
    digitalWrite(L7, HIGH);
  }
  if(countStep == 17 && millis() >= countTimeMillis + delayMode5){
    countStep = 18;
    countTimeMillis = millis();
    digitalWrite(L6, HIGH);
  }
  if(countStep == 18 && millis() >= countTimeMillis + delayMode5){
    countStep = 19;
    countTimeMillis = millis();
    digitalWrite(L5, HIGH);
  }
  if(countStep == 19 && millis() >= countTimeMillis + delayMode5){
    countStep = 20;
    countTimeMillis = millis();
    digitalWrite(L4, HIGH);
  }
  if(countStep == 20 && millis() >= countTimeMillis + delayMode5){
    countStep = 21;
    countTimeMillis = millis();
    digitalWrite(L3, HIGH);
  }
  if(countStep == 21 && millis() >= countTimeMillis + delayMode5){
    countStep = 22;
    countTimeMillis = millis();
    digitalWrite(L2, HIGH);
  }
  if(countStep == 22 && millis() >= countTimeMillis + delayMode5){
    countStep = 23;
    countTimeMillis = millis();
    digitalWrite(L1, HIGH);
  }
  //---------------------// 
  if(countStep == 23 && millis() >= countTimeMillis + 5*delayMode5){
    countStep = 24;
    countTimeMillis = millis();
  }
  if(countStep == 24 && millis() >= countTimeMillis + 5*delayMode5){
    countStep = 25;
    countTimeMillis = millis();
    turnOffAll();
  }
  if(countStep == 25 && millis() >= countTimeMillis + 5*delayMode5){
    countStep = 0;
    statusModeRunning = 6;
    countTimeMillis = millis();
  }

}

void mode6(){
  if(countStepMode6 == 1){
    if(countStep == 0 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(1, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 1 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 1, 0, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 2 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 1, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 3 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 1, 0, 0, 0, 0, 0);
    }
    if(countStep == 4 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 1, 0, 0, 0, 0);
    }
    if(countStep == 5 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 1, 0, 0, 0);
    }
    if(countStep == 6 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 1, 0, 0);
    }
    if(countStep == 7 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 1, 0);
    }
    if(countStep == 8 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 1);
    }
    if(countStep == 9 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    //---------------------// 
    if(countStep == 10 && millis() >= countTimeMillis + (8+1)*delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(1, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 11 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 1, 0, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 12 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 1, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 13 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 1, 0, 0, 0, 0, 0);
    }
    if(countStep == 14 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 1, 0, 0, 0, 0);
    }
    if(countStep == 15 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 1, 0, 0, 0);
    }
    if(countStep == 16 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 1, 0, 0);
    }
    if(countStep == 17 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 1, 0);
    }
    if(countStep == 18 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 1);
    }
    if(countStep == 19 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    //---------------------// 
    if(countStep == 20 && millis() >= countTimeMillis + (7+1)*delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(1, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 21 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 1, 0, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 22 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 1, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 23 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 1, 0, 0, 0, 0, 0);
    }
    if(countStep == 24 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 1, 0, 0, 0, 0);
    }
    if(countStep == 25 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 1, 0, 0, 0);
    }
    if(countStep == 26 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 1, 0, 0);
    }
    if(countStep == 27 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 1, 0);
    }
    if(countStep == 28 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 1);
    }
    if(countStep == 29 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    //---------------------// 
    if(countStep == 30 && millis() >= countTimeMillis + (6+1)*delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(1, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 31 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 1, 0, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 32 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 1, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 33 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 1, 0, 0, 0, 0, 0);
    }
    if(countStep == 34 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 1, 0, 0, 0, 0);
    }
    if(countStep == 35 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 1, 0, 0, 0);
    }
    if(countStep == 36 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 1, 0, 0);
    }
    if(countStep == 37 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 1, 0);
    }
    if(countStep == 38 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 1);
    }
    if(countStep == 39 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    //---------------------// 
    if(countStep == 40 && millis() >= countTimeMillis + (5+1)*delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(1, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 41 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 1, 0, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 42 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 1, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 43 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 1, 0, 0, 0, 0, 0);
    }
    if(countStep == 44 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 1, 0, 0, 0, 0);
    }
    if(countStep == 45 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 1, 0, 0, 0);
    }
    if(countStep == 46 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 1, 0, 0);
    }
    if(countStep == 47 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 1, 0);
    }
    if(countStep == 48 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 1);
    }
    if(countStep == 49 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    //---------------------// 
    if(countStep == 50 && millis() >= countTimeMillis + (4+1)*delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(1, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 51 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 1, 0, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 52 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 1, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 53 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 1, 0, 0, 0, 0, 0);
    }
    if(countStep == 54 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 1, 0, 0, 0, 0);
    }
    if(countStep == 55 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 1, 0, 0, 0);
    }
    if(countStep == 56 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 1, 0, 0);
    }
    if(countStep == 57 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 1, 0);
    }
    if(countStep == 58 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 1);
    }
    if(countStep == 59 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    //---------------------// 
    if(countStep == 60 && millis() >= countTimeMillis + (3+1)*delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(1, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 61 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 1, 0, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 62 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 1, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 63 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 1, 0, 0, 0, 0, 0);
    }
    if(countStep == 64 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 1, 0, 0, 0, 0);
    }
    if(countStep == 65 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 1, 0, 0, 0);
    }
    if(countStep == 66 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 1, 0, 0);
    }
    if(countStep == 67 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 1, 0);
    }
    if(countStep == 68 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 1);
    }
    if(countStep == 69 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    //---------------------// 
    if(countStep == 70 && millis() >= countTimeMillis + (2+1)*delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(1, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 71 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 1, 0, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 72 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 1, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 73 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 1, 0, 0, 0, 0, 0);
    }
    if(countStep == 74 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 1, 0, 0, 0, 0);
    }
    if(countStep == 75 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 1, 0, 0, 0);
    }
    if(countStep == 76 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 1, 0, 0);
    }
    if(countStep == 77 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 1, 0);
    }
    if(countStep == 78 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 1);
    }
    if(countStep == 79 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    //---------------------// 
    if(countStep == 80 && millis() >= countTimeMillis + (1+1)*delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(1, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 81 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 1, 0, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 82 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 1, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 83 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 1, 0, 0, 0, 0, 0);
    }
    if(countStep == 84 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 1, 0, 0, 0, 0);
    }
    if(countStep == 85 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 1, 0, 0, 0);
    }
    if(countStep == 86 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 1, 0, 0);
    }
    if(countStep == 87 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 1, 0);
    }
    if(countStep == 88 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 1);
    }
    if(countStep == 89 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    //---------------------// 
    if(countStep == 90 && millis() >= countTimeMillis + 1*delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(1, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 91 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 1, 0, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 92 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 1, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 93 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 1, 0, 0, 0, 0, 0);
    }
    if(countStep == 94 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 1, 0, 0, 0, 0);
    }
    if(countStep == 95 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 1, 0, 0, 0);
    }
    if(countStep == 96 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 1, 0, 0);
    }
    if(countStep == 97 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 1, 0);
    }
    if(countStep == 98 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 0, 1);
    }
    //---------------------// 
    if(countStep == 99 && millis() >= countTimeMillis + 1*delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(1, 0, 0, 0, 0, 0, 0, 0, 1);
    }
    if(countStep == 100 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 1, 0, 0, 0, 0, 0, 0, 1);
    }
    if(countStep == 101 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 1, 0, 0, 0, 0, 0, 1);
    }
    if(countStep == 102 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 1, 0, 0, 0, 0, 1);
    }
    if(countStep == 103 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 1, 0, 0, 0, 1);
    }
    if(countStep == 104 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 1, 0, 0, 1);
    }
    if(countStep == 105 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 1, 0, 1);
    }
    if(countStep == 106 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 0, 1, 1);
    }
    //---------------------// 
    if(countStep == 107 && millis() >= countTimeMillis + 1*delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(1, 0, 0, 0, 0, 0, 0, 1, 1);
    }
    if(countStep == 108 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 1, 0, 0, 0, 0, 0, 1, 1);
    }
    if(countStep == 109 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 1, 0, 0, 0, 0, 1, 1);
    }
    if(countStep == 110 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 1, 0, 0, 0, 1, 1);
    }
    if(countStep == 111 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 1, 0, 0, 1, 1);
    }
    if(countStep == 112 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 1, 0, 1, 1);
    }
    if(countStep == 113 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 0, 1, 1, 1);
    }
    //---------------------// 
    if(countStep == 114 && millis() >= countTimeMillis + 1*delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(1, 0, 0, 0, 0, 0, 1, 1, 1);
    }
    if(countStep == 115 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 1, 0, 0, 0, 0, 1, 1, 1);
    }
    if(countStep == 116 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 1, 0, 0, 0, 1, 1, 1);
    }
    if(countStep == 117 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 1, 0, 0, 1, 1, 1);
    }
    if(countStep == 118 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 1, 0, 1, 1, 1);
    }
    if(countStep == 119 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 0, 1, 1, 1, 1);
    }
    //---------------------// 
    if(countStep == 120 && millis() >= countTimeMillis + 1*delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(1, 0, 0, 0, 0, 1, 1, 1, 1);
    }
    if(countStep == 121 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 1, 0, 0, 0, 1, 1, 1, 1);
    }
    if(countStep == 122 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 1, 0, 0, 1, 1, 1, 1);
    }
    if(countStep == 123 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 1, 0, 1, 1, 1, 1);
    }
    if(countStep == 124 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 0, 1, 1, 1, 1, 1);
    }
    //---------------------// 
    if(countStep == 125 && millis() >= countTimeMillis + 1*delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(1, 0, 0, 0, 1, 1, 1, 1, 1);
    }
    if(countStep == 126 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 1, 0, 0, 1, 1, 1, 1, 1);
    }
    if(countStep == 127 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 1, 0, 1, 1, 1, 1, 1);
    }
    if(countStep == 128 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 0, 1, 1, 1, 1, 1, 1);
    }
    //---------------------// 
    if(countStep == 129 && millis() >= countTimeMillis + 1*delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(1, 0, 0, 1, 1, 1, 1, 1, 1);
    }
    if(countStep == 130 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 1, 0, 1, 1, 1, 1, 1, 1);
    }
    if(countStep == 131 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 0, 1, 1, 1, 1, 1, 1, 1);
    }
    //---------------------// 
    if(countStep == 132 && millis() >= countTimeMillis + 1*delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(1, 0, 1, 1, 1, 1, 1, 1, 1);
    }
    if(countStep == 133 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(0, 1, 1, 1, 1, 1, 1, 1, 1);
    }
    //---------------------// 
    if(countStep == 134 && millis() >= countTimeMillis + 1*delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWrite(1, 1, 1, 1, 1, 1, 1, 1, 1);
    }
    //------------------------------------------------------------------------------------------------//
    if(countStep == 135 && millis() >= countTimeMillis + 5*delayMode6){
      countStep++;
      countTimeMillis = millis();
      turnOffAll();
    }
    if(countStep == 136 && millis() >= countTimeMillis + 5*delayMode6){
      countStep ++;
      countTimeMillis = millis();
      turnOnAll();
    }
    if(countStep == 137 && millis() >= countTimeMillis + 5*delayMode6){
      countStep++;
      countTimeMillis = millis();
      turnOffAll();
    }
    if(countStep == 138 && millis() >= countTimeMillis + 5*delayMode6){
      countStep ++;
      countTimeMillis = millis();
      turnOnAll();
    }
    if(countStep == 139 && millis() >= countTimeMillis + 5*delayMode6){
      countStep++;
      countTimeMillis = millis();
      turnOffAll();
    }
    if(countStep == 140 && millis() >= countTimeMillis + 5*delayMode6){
      countStep = 0;
      countStepMode6 = 2;
      countTimeMillis = millis();
    }
  }
  //------------------------------------------------------------------------------------------------//
  if(countStepMode6 == 2){
    if(countStep == 0 && millis() >= countTimeMillis + (9+1)*delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(1, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 1 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(0, 1, 0, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 2 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(0, 0, 1, 0, 0, 0, 0, 0, 0);
    }
    if(countStep == 3 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(0, 0, 0, 1, 0, 0, 0, 0, 0);
    }
    if(countStep == 4 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(0, 0, 0, 0, 1, 0, 0, 0, 0);
    }
    if(countStep == 5 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(0, 0, 0, 0, 0, 1, 0, 0, 0);
    }
    if(countStep == 6 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(0, 0, 0, 0, 0, 0, 1, 0, 0);
    }
    if(countStep == 7 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(0, 0, 0, 0, 0, 0, 0, 1, 0);
    }
    if(countStep == 8 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(0, 0, 0, 0, 0, 0, 0, 0, 1);
    }
    //---------------------// 
    if(countStep == 9 && millis() >= countTimeMillis + (9+1)*delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(1, 0, 0, 0, 0, 0, 0, 0, 1);
    }
    if(countStep == 10 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(0, 1, 0, 0, 0, 0, 0, 0, 1);
    }
    if(countStep == 11 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(0, 0, 1, 0, 0, 0, 0, 0, 1);
    }
    if(countStep == 12 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(0, 0, 0, 1, 0, 0, 0, 0, 1);
    }
    if(countStep == 13 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(0, 0, 0, 0, 1, 0, 0, 0, 1);
    }
    if(countStep == 14 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(0, 0, 0, 0, 0, 1, 0, 0, 1);
    }
    if(countStep == 15 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(0, 0, 0, 0, 0, 0, 1, 0, 1);
    }
    if(countStep == 16 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(0, 0, 0, 0, 0, 0, 0, 1, 1);
    }
    //---------------------// 
    if(countStep == 17 && millis() >= countTimeMillis + (9+1)*delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(1, 0, 0, 0, 0, 0, 0, 1, 1);
    }
    if(countStep == 18 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(0, 1, 0, 0, 0, 0, 0, 1, 1);
    }
    if(countStep == 19 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(0, 0, 1, 0, 0, 0, 0, 1, 1);
    }
    if(countStep == 20 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(0, 0, 0, 1, 0, 0, 0, 1, 1);
    }
    if(countStep == 21 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(0, 0, 0, 0, 1, 0, 0, 1, 1);
    }
    if(countStep == 22 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(0, 0, 0, 0, 0, 1, 0, 1, 1);
    }
    if(countStep == 23 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(0, 0, 0, 0, 0, 0, 1, 1, 1);
    }
    //---------------------// 
    if(countStep == 24 && millis() >= countTimeMillis + (9+1)*delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(1, 0, 0, 0, 0, 0, 1, 1, 1);
    }
    if(countStep == 25 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(0, 1, 0, 0, 0, 0, 1, 1, 1);
    }
    if(countStep == 26 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(0, 0, 1, 0, 0, 0, 1, 1, 1);
    }
    if(countStep == 27 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(0, 0, 0, 1, 0, 0, 1, 1, 1);
    }
    if(countStep == 28 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(0, 0, 0, 0, 1, 0, 1, 1, 1);
    }
    if(countStep == 29 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(0, 0, 0, 0, 0, 1, 1, 1, 1);
    }
    //---------------------// 
    if(countStep == 30 && millis() >= countTimeMillis + (9+1)*delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(1, 0, 0, 0, 0, 1, 1, 1, 1);
    }
    if(countStep == 31 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(0, 1, 0, 0, 0, 1, 1, 1, 1);
    }
    if(countStep == 32 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(0, 0, 1, 0, 0, 1, 1, 1, 1);
    }
    if(countStep == 33 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(0, 0, 0, 1, 0, 1, 1, 1, 1);
    }
    if(countStep == 34 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(0, 0, 0, 0, 1, 1, 1, 1, 1);
    }
    //---------------------// 
    if(countStep == 35 && millis() >= countTimeMillis + (9+1)*delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(1, 0, 0, 0, 1, 1, 1, 1, 1);
    }
    if(countStep == 36 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(0, 1, 0, 0, 1, 1, 1, 1, 1);
    }
    if(countStep == 37 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(0, 0, 1, 0, 1, 1, 1, 1, 1);
    }
    if(countStep == 38 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(0, 0, 0, 1, 1, 1, 1, 1, 1);
    }
    //---------------------// 
    if(countStep == 39 && millis() >= countTimeMillis + (9+1)*delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(1, 0, 0, 1, 1, 1, 1, 1, 1);
    }
    if(countStep == 40 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(0, 1, 0, 1, 1, 1, 1, 1, 1);
    }
    if(countStep == 41 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(0, 0, 1, 1, 1, 1, 1, 1, 1);
    }
    //---------------------// 
    if(countStep == 42 && millis() >= countTimeMillis + (9+1)*delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(1, 0, 1, 1, 1, 1, 1, 1, 1);
    }
    if(countStep == 43 && millis() >= countTimeMillis + delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(0, 1, 1, 1, 1, 1, 1, 1, 1);
    }
    //---------------------// 
    if(countStep == 44 && millis() >= countTimeMillis + (9+1)*delayMode6){
      countStep++;
      countTimeMillis = millis();
      ledWriteInverse(1, 1, 1, 1, 1, 1, 1, 1, 1);
    }
    
    if(countStep == 45 && millis() >= countTimeMillis + 45*delayMode6){
      countStep++;
      countTimeMillis = millis();
    }
    //------------------------------------------------------------------------------------------------//
    if(countStep == 46 && millis() >= countTimeMillis + 5*delayMode6){
      countStep++;
      countTimeMillis = millis();
      turnOffAll();
    }
    if(countStep == 47 && millis() >= countTimeMillis + 5*delayMode6){
      countStep ++;
      countTimeMillis = millis();
      turnOnAll();
    }
    if(countStep == 48 && millis() >= countTimeMillis + 5*delayMode6){
      countStep++;
      countTimeMillis = millis();
      turnOffAll();
    }
    if(countStep == 49 && millis() >= countTimeMillis + 5*delayMode6){
      countStep ++;
      countTimeMillis = millis();
      turnOnAll();
    }
    if(countStep == 50 && millis() >= countTimeMillis + 5*delayMode6){
      countStep++;
      countTimeMillis = millis();
      turnOffAll();
    }
    if(countStep == 51 && millis() >= countTimeMillis + 5*delayMode6){
      countStep = 0;
      countStepMode6 = 1;
      statusModeRunning = 1;
      countTimeMillis = millis();
    }
  }
}

void runAll(){
  if(statusModeRunning == 1){
    mode1();
  }
  if(statusModeRunning == 2){
    mode2();
  }
  if(statusModeRunning == 3){
    mode3();
  }
  if(statusModeRunning == 4){
    mode4();
  }
  if(statusModeRunning == 5){
    mode5();
  }
  if(statusModeRunning == 6){
    mode6();
  }
}
