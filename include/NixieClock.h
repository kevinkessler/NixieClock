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

bool displayNums(uint8_t idx, uint8_t dig);
void displayError(uint8_t err);
void initNixies(void);
bool displayNums(uint8_t idx, uint8_t dig);
void allOff(uint8_t idx);
void otaSetup(void);
void wifiSetup(void);

#endif /*INCLUDE_NIXIECLOCK_H_*/