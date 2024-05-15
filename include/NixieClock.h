/**
 *  @filename   :   NixieClock.h
 *  @brief      :   WiFi Nixie Clock
 *  @author     :   Kevin Kessler
 *
 * Copyright (C) 2024 Kevin Kessler
 */

#ifndef INCLUDE_NIXIECLOCK_H_
#define INCLUDE_NIXIECLOCK_H_

#define PIN_E 12
#define PIN_A0 13
#define PIN_A1 14
#define PIN_A2 15

#define WIFI_CONFIG 9
#define NTP_FAIL 8
#define I2C_ERROR 7
#define NIXIE_NO_ERROR 0


struct ntpConf {
  uint32_t valid;
  char ntpServer[254];
  char tzInfo[50];
};


bool displayNums(uint8_t idx, uint8_t dig);
void displayError(uint8_t err);
void initNixies(void);
bool displayNums(uint8_t idx, uint8_t dig);
void allOff(uint8_t idx);
void otaSetup(void);
void wifiSetup(void);
bool writeEEPROM(char *server, char *zone);
void ntpSetup(void);
bool getNTPtime(uint16_t sec);
uint8_t getHour(void);
uint8_t getMin(void);
uint8_t getSec(void); 

#endif /*INCLUDE_NIXIECLOCK_H_*/