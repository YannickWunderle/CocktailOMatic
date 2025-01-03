/*Author: Yannick Wunderle
Date: 17.11.2024
Function: Cocktail machine with 12 valves and one peristalic pump that use the same motor

TODO:
Mainpage Button - 
Settings Extra-
Array and String for Mix Function list choice -
Pump Limit - 
Idel LED sequence -
Battery monitor -
pump air -

*/

#include "Main_WebServer.h"
#include "Ingredients.h"

int valvePos = 0;

void setup() {
  Serial.begin(115200);
  pinMode(RELAIS_PIN, OUTPUT);
  digitalWrite(RELAIS_PIN, HIGH);
  init_Motor();
  init_LEDs();
  setup_WebServer();
  init_Valves(false);
}


void loop() {
  loop_WebServer();
  if (Order.mix) {
    mixCocktail();
    LEDsFinish();
  }
  LEDsIdle();
  checkBattery();
  checkShutdown();
  /*
  int incomingByte = 0;

  if (Serial.available() > 0) {
    incomingByte = Serial.parseInt();
    if (incomingByte > 0) {
      //testfun(incomingByte);
      
      nextValve(valvePos);
      valvePos++;
      pump(10);
      if (valvePos > 10) {
        valvePos = 0;
        init_Valves(true);
      }
    }
    incomingByte = 0;
  }*/
}
