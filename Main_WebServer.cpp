#include "Main_WebServer.h"

String HTML_StartPage = "";
String HTML_AlcoholicList = "";
String HTML_NonAlcoholicList = "";
String HTML_OrderOK = "";

String HTML_InformationMixHit = "";
String HTML_InformationUser = "";

String HTML_MixHitConfigChange = "";
String HTML_WiFiConfigChange = "";
String HTML_MixHitConfig = "";
String HTML_WiFiConfig = "";

WiFiServer server(80, 200);
extern Que Order = { "/Bestellung_Alkoholisch", 0, 0, false };

void setup_WebServer() {
  const char* ssid = "CocktailOMatic";
  const char* password = "0123456789";
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  delay(200);
  IPAddress Ip(192, 168, 1, 1);
  IPAddress NMask(255, 255, 255, 0);
  WiFi.softAPConfig(Ip, Ip, NMask);
  server.begin();
}

void loop_WebServer() {
  WiFiClient client = server.available();  // Warten auf Anfragen von Clients
  if (client) {
    String lURL;
    int lUserIPIndex;
    String c;
    if (client.connected()) {
      int client_Index = client.available();
      if (client_Index) {
#ifndef REGION_ReadClientInfo
        c = client.readString();  // Gesamte Information welche vom client empfangen wird
        lURL = getURL(c);
        //lUserIP = client.remoteIP().toString();

#endif
        if (lURL == "/") {
          Print_StartPage(client);
        } else if (lURL == "/Cocktailkarte_Alkoholfrei") {
          Print_NonAlcoholicCocktailsPage(client);
        } else if (lURL == "/Cocktailkarte_Alkoholisch") {
          Print_AlcoholicCocktailsPage(client);
        } else if (lURL == "/Cocktailkarte_______Shots") {
          Print_Shots(client);
        } else if (lURL == "/Cocktailkarte_Prime_Setti") {
          Print_Settings(client);
        } else {
          String cocktailId = String(lURL);
          String cocktailVolume = String(lURL);
          String cocktailList = String(lURL);

          cocktailList.remove(CHARACTERS_TO_VOL);
          cocktailVolume.remove(0, CHARACTERS_TO_VOL + 1);
          cocktailVolume.remove(11);
          int volume = 0;
          if (cocktailVolume == "ORDER_SMALL") {
            volume = ORDER_SMALL;
          } else if (cocktailVolume == "ORDER_LARGE") {
            volume = ORDER_LARGE;
          } else if (cocktailVolume == "ORDER__SHOT") {
            volume = ORDER__SHOT;
          } else if (cocktailVolume == "ORDER_PRIME") {
            volume = ORDER_PRIME;
          } else if (cocktailVolume == "ORDER___5cl") {
            volume = ORDER_5CL;
          }

          cocktailId.remove(0, CHARACTERS_TO_ID);
          int id = cocktailId.toInt();

          Order.mix = true;
          Order.OrderId = id;
          Order.OrderVolume = volume;
          Order.OrderCocktaillist = cocktailList;

          int IngredientRatio[11];
          for (int i = 0; i < 11; i++) {
            if (Order.OrderCocktaillist == "/Bestellung_Alkoholisch") {
              IngredientRatio[i] = Alcoholic_List[Order.OrderId].IngredientVolume[i];
            } else if (Order.OrderCocktaillist == "/Bestellung_Alkoholfrei") {
              IngredientRatio[i] = NonAlcoholic_List[Order.OrderId].IngredientVolume[i];
            } else if (Order.OrderCocktaillist == "/Bestellung_Prime_Shots") {
              IngredientRatio[i] = Prime_List[Order.OrderId].IngredientVolume[i];
              if (Order.OrderId == PRIME_ID) {
                Order.OrderVolume = ORDER_PRIME * 11;
              }
              if (Order.OrderId == FLUSH_ID) {
                Order.OrderVolume = ORDER_FLUSH * 11;
              }
            }
          }

          String CocktailName = "";
          if (cocktailList == "/Bestellung_Alkoholisch") {
            CocktailName += Alcoholic_List[id].RecipeName;
          } else if (cocktailList == "/Bestellung_Alkoholfrei") {
            CocktailName += NonAlcoholic_List[id].RecipeName;
          } else if (cocktailList == "/Bestellung_Prime_Shots") {
            CocktailName += Prime_List[id].RecipeName;
          }
          Show_Message(client, CocktailName);
          int ingredientSum = 0;
          for (int i = 0; i < nb_ingredients; i++) {
            ingredientSum += IngredientRatio[i];
          }

          for (int i = 0; i < nb_ingredients; i++) {
            volume = int(double(IngredientRatio[i]) * double(Order.OrderVolume) / double(ingredientSum));
            if (IngredientRatio[i] > 0) {
              String Message = String(volume) + " ml " + Ingredients_List[i].IngredientName;
              Show_Message(client, Message);
            }
          }
          Back_StartPage(client);
        }
      }
    }
  }
  //vTaskDelay(10);
}

String getURL(String pString) {
  // Position von 'GET /' suchen
  int addr_start = pString.indexOf("GET /");
  int lOffset = 4;
  if (addr_start < 4) {
    addr_start = pString.indexOf("POST /");
    lOffset = 5;
  }
  // Position von ' HTTP' suchen
  int addr_end = pString.indexOf(" HTTP", addr_start + 1);

  // Text dazwischen zur�ckgeben
  if (addr_start > -1 || addr_end != -1) {
    return pString.substring(addr_start + lOffset, addr_end);
  }
  return "";
}

void Print_StartPage(WiFiClient pWifiClient) {
  pWifiClient.println("<!DOCTYPE html><html>");
  pWifiClient.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  //pWifiClient.println("<body style="background-color:black;">);

  pWifiClient.println("<link rel=\"icon\" href=\"data:,\">");
  pWifiClient.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  pWifiClient.println(".button {height:200px;width:400px; background-color: #333333; border: none; color: white; padding: 16px 40px;");
  pWifiClient.println("text-decoration: none; font-size: 40px; margin: 2px; cursor: pointer;}");
  pWifiClient.println(".button2 {background-color: #555555;}</style></head>");
  // Web Page Heading
  pWifiClient.println("<body><h1>Cocktail-O-Matic</h1>");
  pWifiClient.println("<body><h2>Batterie: " + getBatteryState() + " Prozent</h2>");

  pWifiClient.println("<p><a href=\"/Cocktailkarte_Alkoholisch\"><button class=\"button\">Cocktails</button></a></p>");
  pWifiClient.println("<p><a href=\"/Cocktailkarte_Alkoholfrei\"><button class=\"button\">Cocktails alkoholfrei</button></a></p>");
  pWifiClient.println("<p><a href=\"/Cocktailkarte_______Shots\"><button class=\"button\">Shots</button></a></p>");
  pWifiClient.println("<p><a href=\"/Cocktailkarte_Prime_Setti\"><button class=\"button\">Einstellungen / Statistik</button></a></p>");
  pWifiClient.println("</body></html>");
  pWifiClient.println();
}

void Back_StartPage(WiFiClient pWifiClient) {
  pWifiClient.println("<!DOCTYPE html><html>");
  pWifiClient.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  pWifiClient.println("<link rel=\"icon\" href=\"data:,\">");
  pWifiClient.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  pWifiClient.println(".button {height:120px;width:400px; background-color: #333333; border: none; color: white; padding: 16px 40px;");
  pWifiClient.println("text-decoration: none; font-size: 40px; margin: 2px; cursor: pointer;}");
  pWifiClient.println(".button2 {background-color: #555555;}</style></head>");
  pWifiClient.println("<p><a href=\"/\"><button class=\"button\">Startseite</button></a></p>");
  pWifiClient.println("</body></html>");
  pWifiClient.println();
}

void Print_Settings(WiFiClient pWifiClient) {
  pWifiClient.println("<!DOCTYPE html><html>");
  pWifiClient.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  //pWifiClient.println("<body style="background-color:black;">);

  pWifiClient.println("<link rel=\"icon\" href=\"data:,\">");
  pWifiClient.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  pWifiClient.println(".button {height:100px;width:400px; background-color: #333333; border: none; color: white; padding: 16px 40px;");
  pWifiClient.println("text-decoration: none; font-size: 40px; margin: 2px; cursor: pointer;}");
  pWifiClient.println(".button2 {background-color: #555555;}</style></head>");
  // Web Page Heading
  pWifiClient.println("<body><h1>Cocktail-O-Matic</h1>");
  pWifiClient.println("<body><h2>Batterie: " + getBatteryState() + " Prozent</h2>");
  pWifiClient.println("<body><h2>Anzahl Getr&aumlnke: " + String(cocktailCounter) + " </h2>");
  uint32_t seconds = millis() / 1000;
  uint32_t minutesTotal;
  uint32_t hours;
  minutesTotal = seconds / 60;
  seconds %= 60;
  hours = minutesTotal / 60;
  uint32_t minutes = minutesTotal % 60;
  pWifiClient.println("<body><h2>Betriebszeit: " + String(hours) + ":" + String(minutes) + ":" + String(seconds) + " </h2>");
  //pWifiClient.println("<body><h2>Getr&aumlnke pro Stunde: " + String(cocktailCounter * 60 / minutesTotal) + " </h2>");
  float TotalVolume = 0;
  for (int i = 0; i < nb_ingredients; i++) {
    pWifiClient.println("<body><h2> " + Ingredients_List[i].IngredientName + ": " + String(Ingredients_List[i].volume) + " L</h2>");
    TotalVolume += Ingredients_List[i].volume;
  }
  pWifiClient.println("<body><h2> Gesamtverbrauch: " + String(TotalVolume) + " L</h2>");
  pWifiClient.println("<p><a href=\"/Bestellung_Prime_Shots/ORDER_LARGE/" + String(PRIME_ID) + "\"><button class=\"button\">Prime Alle</button></a></p>");
  pWifiClient.println("<p><a href=\"/Bestellung_Prime_Shots/ORDER_LARGE/" + String(FLUSH_ID) + "\"><button class=\"button\">Sp&uumllprogramm</button></a></p>");
  pWifiClient.println("</body></html>");
  pWifiClient.println();
}


void Print_NonAlcoholicCocktailsPage(WiFiClient pWifiClient) {
  HTML_AlcoholicList = "<!DOCTYPE html><html>";
  HTML_AlcoholicList += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  HTML_AlcoholicList += "<link rel=\"icon\" href=\"data:,\">";
  HTML_AlcoholicList += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";
  HTML_AlcoholicList += ".button {height:40px;width:80px; background-color: #333333; border: none; color: white; padding: 5px 5px;";
  HTML_AlcoholicList += "text-decoration: none; font-size: 20px; margin: 2px; cursor: pointer;}";
  HTML_AlcoholicList += ".button2 {background-color: #555555;}</style></head>";
  HTML_AlcoholicList += " </div>";
  HTML_AlcoholicList += "</div>";
  HTML_AlcoholicList += "<div class=row>";
  HTML_AlcoholicList += "<table class=\"table\">";
  HTML_AlcoholicList += "<thead>";
  HTML_AlcoholicList += "<tr>";
  HTML_AlcoholicList += "<th scope=\"colgroup\" colspan=\"2\" style=\"text-align:center;\">Cocktail</th>";
  HTML_AlcoholicList += "<th scope=\"colgroup\" colspan=\"2\" style=\"text-align:center;\">Gr&ouml&szlige</th>";
  HTML_AlcoholicList += "</tr>";
  HTML_AlcoholicList += "</thead>";
  HTML_AlcoholicList += "<tbody>";
  HTML_AlcoholicList += "<body><h2>Cocktails Alkoholfrei</h2>";

  for (int i = 0; i < nb_noncocktails; i++) {
    HTML_AlcoholicList += "<tr>";
    HTML_AlcoholicList += "<th scope=\"row\" style=\"vertical-align:left\">" + String(i + 1) + "</th>";
    HTML_AlcoholicList += "<td><strong>" + NonAlcoholic_List[i].RecipeName + "</strong></br>";
    HTML_AlcoholicList += "<em>" + getRecipeIngredients(NonAlcoholic_List[i]) + "</em></td>";

    HTML_AlcoholicList += "<td><a href=\"/Bestellung_Alkoholfrei/ORDER_SMALL/" + String(i) + "\" ><button class=\"button\">Klein</button></a></td>";
    HTML_AlcoholicList += "<td><a href=\"/Bestellung_Alkoholfrei/ORDER_LARGE/" + String(i) + "\" ><button class =\"button\">Gro&szlig</button></a></td>";
  }
  HTML_AlcoholicList += "</tbody>";
  HTML_AlcoholicList += "</table>";
  HTML_AlcoholicList += "</div>";
  HTML_AlcoholicList += "</div></body></html>";
  pWifiClient.println(HTML_AlcoholicList);
}

void Print_Shots(WiFiClient pWifiClient) {
  HTML_AlcoholicList = "<!DOCTYPE html><html>";
  HTML_AlcoholicList += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  HTML_AlcoholicList += "<link rel=\"icon\" href=\"data:,\">";
  HTML_AlcoholicList += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";
  HTML_AlcoholicList += ".button {height:40px;width:80px; background-color: #333333; border: none; color: white; padding: 5px 5px;";
  HTML_AlcoholicList += "text-decoration: none; font-size: 20px; margin: 2px; cursor: pointer;}";
  HTML_AlcoholicList += ".buttonRed {height:40px;width:80px; background-color: #db1414; border: none; color: black; padding: 5px 5px;";
  HTML_AlcoholicList += "text-decoration: none; font-size: 20px; margin: 2px; cursor: pointer;}";
  HTML_AlcoholicList += ".button2 {background-color: #555555;}</style></head>";
  HTML_AlcoholicList += " </div>";
  HTML_AlcoholicList += "</div>";
  HTML_AlcoholicList += "<div class=row>";
  HTML_AlcoholicList += "<table class=\"table\">";
  HTML_AlcoholicList += "<thead>";
  HTML_AlcoholicList += "<tr>";
  HTML_AlcoholicList += "<th scope=\"colgroup\" colspan=\"2\" style=\"text-align:center;\">Shots</th>";
  HTML_AlcoholicList += "<th scope=\"colgroup\" colspan=\"2\" style=\"text-align:center;\">Gr&ouml&szlige</th>";
  HTML_AlcoholicList += "</tr>";
  HTML_AlcoholicList += "</thead>";
  HTML_AlcoholicList += "<tbody>";
  HTML_AlcoholicList += "<body><h2>Shots</h2>";

  for (int i = 0; i < nb_prime; i++) {
    HTML_AlcoholicList += "<tr>";
    HTML_AlcoholicList += "<th scope=\"row\" style=\"vertical-align:left\">" + String(i + 1) + "</th>";
    HTML_AlcoholicList += "<td><strong>" + Prime_List[i].RecipeName + "</strong></br>";
    HTML_AlcoholicList += "<em>" + getRecipeIngredients(Prime_List[i]) + "</em></td>";

    HTML_AlcoholicList += "<td><a href=\"/Bestellung_Prime_Shots/ORDER__SHOT/" + String(i) + "\" ><button class=\"button\">2 cL</button></a></td>";
    HTML_AlcoholicList += "<td><a href=\"/Bestellung_Prime_Shots/ORDER___5cl/" + String(i) + "\" ><button class =\"button\">5 cL</button></a></td>";
  }
  HTML_AlcoholicList += "</tbody>";
  HTML_AlcoholicList += "</table>";
  HTML_AlcoholicList += "</div>";
  HTML_AlcoholicList += "</div></body></html>";
  pWifiClient.println(HTML_AlcoholicList);
}

void Print_AlcoholicCocktailsPage(WiFiClient pWifiClient) {
  HTML_AlcoholicList = "<!DOCTYPE html><html>";
  HTML_AlcoholicList += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  HTML_AlcoholicList += "<link rel=\"icon\" href=\"data:,\">";
  HTML_AlcoholicList += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";
  HTML_AlcoholicList += ".button {height:40px;width:80px; background-color: #333333; border: none; color: white; padding: 5px 5px;";
  HTML_AlcoholicList += "text-decoration: none; font-size: 20px; margin: 2px; cursor: pointer;}";
  HTML_AlcoholicList += ".button2 {background-color: #555555;}</style></head>";
  HTML_AlcoholicList += " </div>";
  HTML_AlcoholicList += "</div>";
  HTML_AlcoholicList += "<div class=row>";
  HTML_AlcoholicList += "<table class=\"table\">";
  HTML_AlcoholicList += "<thead>";
  HTML_AlcoholicList += "<tr>";
  HTML_AlcoholicList += "<th scope=\"colgroup\" colspan=\"2\" style=\"text-align:center;\">Cocktail</th>";
  HTML_AlcoholicList += "<th scope=\"colgroup\" colspan=\"2\" style=\"text-align:center;\">Gr&ouml&szlige</th>";
  HTML_AlcoholicList += "</tr>";
  HTML_AlcoholicList += "</thead>";
  HTML_AlcoholicList += "<tbody>";
  HTML_AlcoholicList += "<body><h2>Cocktails</h2>";

  for (int i = 0; i < nb_cocktails; i++) {
    HTML_AlcoholicList += "<tr>";
    HTML_AlcoholicList += "<th scope=\"row\" style=\"vertical-align:left\">" + String(i + 1) + "</th>";
    HTML_AlcoholicList += "<td><strong>" + Alcoholic_List[i].RecipeName + "</strong></br>";
    HTML_AlcoholicList += "<em>" + getRecipeIngredients(Alcoholic_List[i]) + "</em></td>";

    HTML_AlcoholicList += "<td><a href=\"/Bestellung_Alkoholisch/ORDER_SMALL/" + String(i) + "\" ><button class=\"button\">Klein</button></a></td>";
    HTML_AlcoholicList += "<td><a href=\"/Bestellung_Alkoholisch/ORDER_LARGE/" + String(i) + "\" ><button class =\"button\">Gro&szlig</button></a></td>";
  }
  HTML_AlcoholicList += "</tbody>";
  HTML_AlcoholicList += "</table>";
  HTML_AlcoholicList += "</div>";
  HTML_AlcoholicList += "</div></body></html>";
  pWifiClient.println(HTML_AlcoholicList);
}

void Show_Message(WiFiClient pWifiClient, String Message) {
  pWifiClient.println("<!DOCTYPE html><html>");
  pWifiClient.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  //pWifiClient.println("<body style="background-color:black;">);

  pWifiClient.println("<link rel=\"icon\" href=\"data:,\">");
  pWifiClient.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  pWifiClient.println(".button {height:200px;width:400px; background-color: #333333; border: none; color: white; padding: 16px 40px;");
  pWifiClient.println("text-decoration: none; font-size: 40px; margin: 2px; cursor: pointer;}");
  pWifiClient.println(".button2 {background-color: #555555;}</style></head>");
  // Web Page Heading
  //pWifiClient.println("<body><h1>Cocktail-O-Matic</h1>");
  pWifiClient.println("<body><h2>" + Message + "</h2>");

  pWifiClient.println("</body></html>");
  pWifiClient.println();
}
