#ifndef _MAIN_WEBSERVER_H_
#define _MAIN_WEBSERVER_H_

#include <WiFi.h>
#include "esp_wifi.h"

#include "Ingredients.h"
#include "Main_Mixer.h"


#define AP_SSID "Cocktail-O-Matic"

#define ORDER_SMALL 125 //ml
#define ORDER_LARGE 200 //ml
#define ORDER_PRIME 20 //ml
#define ORDER__SHOT 20 //ml
#define ORDER_FLUSH 100//ml
#define ORDER_5CL 50 // ml

#define CHARACTERS_TO_ID 36
#define CHARACTERS_TO_VOL 23

struct Que
{
  String OrderCocktaillist;
  int OrderId;
  int OrderVolume;
  bool mix = false;
};

extern Que Order;

void setup_WebServer();
void loop_WebServer();
void WriteBootstrap();

String getURL(String pString);

void Print_StartPage(WiFiClient pWifiClient);
void Back_StartPage(WiFiClient pWifiClient);

void Print_NonAlcoholicCocktailsPage(WiFiClient pWifiClient);
void Print_AlcoholicCocktailsPage(WiFiClient pWifiClient);
void Print_Shots(WiFiClient pWifiClient);
void Print_Settings(WiFiClient pWifiClient);
void Print_Order(WiFiClient pWiFiClient, String pURL, int pListSelect, int pSize);
void Show_Message(WiFiClient pWifiClient, String Message);

#endif
