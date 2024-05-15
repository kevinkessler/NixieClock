/**
 *  @filename   :   main.cpp
 *  @brief      :   WiFi Nixie Clock
 *  @author     :   Kevin Kessler
 *
 * Copyright (C) 2024 Kevin Kessler
 */

#include <Arduino.h>
#include <EEPROM.h>
#include <ElegantOTA.h>
#include "NixieClock.h"

extern int8_t errorFlag;

ntpConf nixieConf;

const char* NTP_SERVER = "pool.ntp.org";
// Posix Time Zone String - https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
const char* TZ_INFO    = "EST5EDT,M3.2.0,M11.1.0"; 


extern WebServer webServer;



bool writeEEPROM(char *server, char *zone) {
  Serial.printf("Writing %s, %s to EEPROM\n",server,zone);

  nixieConf.valid = 0xDEADBEEF;
  strcpy(nixieConf.ntpServer,server);
  strcpy(nixieConf.tzInfo, zone);

  EEPROM.put(0, nixieConf);
  return EEPROM.commit();
}

void loadConf(){
  bool retval=EEPROM.begin(sizeof(struct ntpConf));
  Serial.printf("begin %i\n", retval);
  struct ntpConf loc=EEPROM.get(0,nixieConf);

  if(nixieConf.valid!=0xDEADBEEF)
  {
    writeEEPROM((char *)NTP_SERVER,(char *)TZ_INFO);
  }
}

void setup() {
  sleep(3);
  Serial.begin(115200);
  Serial.println("Begin");

  loadConf();
  Serial.printf("ntpServer %s, tz %s\n",nixieConf.ntpServer,nixieConf.tzInfo);
  initNixies();


  wifiSetup();

  otaSetup();

  ntpSetup();

}

uint8_t toggle=0;
unsigned long curTime=0;
uint8_t prevHour=99;
uint8_t prevMin=99;
uint8_t prevSec=99;
void loop() {

  if(curTime==0)
    curTime=millis();

  if(errorFlag != NIXIE_NO_ERROR) { // Display Error Code 
    if(toggle) {
      uint8_t err = errorFlag * 10 + errorFlag;
      displayNums(0,err);
      displayNums(1,err);
      displayNums(2,err);
    } else {
      //Serial.println("Toogle");
      allOff(0);
      allOff(1);
      allOff(2);
    }
  } else { // Display Time
    if(getNTPtime(1)) {

      if(getHour() != prevHour) {
        displayNums(0,getHour());
        prevHour=getHour();
      }
      if(getMin() != prevMin) {
        displayNums(1,getMin());
        prevMin=getMin();
      }
      if(getSec() != prevSec) {
        displayNums(2,getSec());
        prevSec=getSec();
      }
    }
  }

  if(curTime + 1000 < millis()) {
    if(toggle) {
      toggle = 0; 
    } else {
      toggle = 1;
    }
    curTime = millis();
    Serial.println("Toggle");
  }

  webServer.handleClient();
  ElegantOTA.loop();

}

