#ifndef _MAIN_MIXER_H_
#define _MAIN_MIXER_H_

#include "Main_WebServer.h"
#include "Ingredients.h"
#include "Arduino.h"
#include <WiFi.h>
#include "esp_wifi.h"
#include <FastLED.h>

#define PRIME_ID 18
#define FLUSH_ID 19
#define RELAIS_PIN 14

void motorStop();
void init_Motor();
void getPos();
void showLEDs();
void showLEDsIngredient(int i);
void LEDsIdle();
void init_Valves(bool forceIni);
void mixCocktail();
void nextValve(int i);
void pump(int volume);
void init_LEDs();
void checkBattery();
void checkShutdown();
void LEDsFinish();

String getBatteryState();

extern int cocktailCounter;



#endif