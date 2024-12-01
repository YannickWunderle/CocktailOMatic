#include "Main_Mixer.h"
#include <ESP32Servo.h>

#define NUM_LEDS 71
#define LED_PIN 13
#define BAT_PIN 34
#define SERVO_PIN 26
#define HALL_PIN_1 5
#define HALL_PIN_2 19
#define HALL_PIN_3 21
#define INI_PIN 12

#define AIR_VOLUME 50  //ml
#define CUTOFF 2600    // 2.9V/cell, 17.4V
#define LOWBAT 2850    // 3.2V/cell, 19.2V
#define FULLBAT 4000   // 4.2V/cell, 25.2V
#define LED_IDLE_Brightness 50
#define STEPS_PER_ML 135  //162  //135

Servo myservo;
CRGB leds[NUM_LEDS];
unsigned long previousMillisStripe = 0;
bool shutdown = false;

int b_red = 0;
int b_green = 0;
int b_blue = LED_IDLE_Brightness;
unsigned long lastBat = LOWBAT;

unsigned long lastStop = 0;
long int pos = 0;
long int goalpos = 0;
long int lastStopPos = 0;
int lastState = 0;

extern int cocktailCounter = 0;

void init_LEDs() {
  pinMode(LED_PIN, OUTPUT);
  FastLED.addLeds<WS2812, LED_PIN, RGB>(leds, NUM_LEDS);
}

void init_Motor() {
  pinMode(HALL_PIN_1, INPUT);
  pinMode(HALL_PIN_2, INPUT);
  pinMode(HALL_PIN_3, INPUT);
  pinMode(RELAIS_PIN, OUTPUT);
  pinMode(INI_PIN, INPUT);
  digitalWrite(RELAIS_PIN, HIGH);
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);             // standard 50 hz servo
  myservo.attach(SERVO_PIN, 1000, 2000);  // attaches the servo on pin 18 to the servo object
  myservo.writeMicroseconds(1500);
  delay(3200);
}

void showLEDsIngredient(int i) {
  for (int ID = 0; ID < NUM_LEDS; ID++) {
    leds[ID].b = Ingredients_List[i].b;
    leds[ID].r = Ingredients_List[i].g;
    leds[ID].g = Ingredients_List[i].r;
  }
  FastLED.show();
}

void showLEDs() {
  for (int ID = 0; ID < NUM_LEDS; ID++) {
    leds[ID].r = b_green;
    leds[ID].b = b_blue;
    leds[ID].g = b_red;
  }
  FastLED.show();
}

void LEDsIdle() {
  if (millis() - previousMillisStripe > 85) {
    previousMillisStripe = millis();
    if (b_green <= 0) {
      if (b_red < LED_IDLE_Brightness) {
        b_red = b_red + 1;
        b_blue = b_blue - 1;
      } else {
        b_green = 1;
        b_blue = 0;
      }
      showLEDs();
      return;
    }

    if (b_blue <= 0) {
      if (b_green < LED_IDLE_Brightness) {
        b_green++;
        b_red--;
      } else {
        b_blue = 1;
        b_red = 0;
      }
      showLEDs();
      return;
    }

    if (b_red <= 0) {
      if (b_blue < LED_IDLE_Brightness) {
        b_blue++;
        b_green--;
      } else {
        b_red = 1;
        b_green = 0;
      }
      showLEDs();
      return;
    } else {
      b_red = 0;
      b_green = 0;
      b_blue = LED_IDLE_Brightness;
    }
  }
}

void init_Valves(bool forceIni) {
  if (!digitalRead(INI_PIN) or forceIni) {  //init air valve
    myservo.writeMicroseconds(1400);
    while (!digitalRead(INI_PIN)) {
    }
    pos = 0;
    goalpos = -50;
    myservo.writeMicroseconds(1450);
    while (pos > goalpos) {
      getPos();
    }
    myservo.writeMicroseconds(1500);
    delay(100);
    pos = 0;
    goalpos = 70;
    myservo.writeMicroseconds(1600);
    while (pos < goalpos) {
      getPos();
    }
    myservo.writeMicroseconds(1500);
    delay(100);
  }
}

void mixCocktail() {
  Order.mix = false;
  cocktailCounter++;
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
      }if (Order.OrderId == FLUSH_ID) {
        Order.OrderVolume = ORDER_FLUSH * 11;
      }
    }
  }

  int lastValve = 0;
  for (int i = 0; i < nb_ingredients; i++) {
    if (IngredientRatio[i] > 0) {
      lastValve = i + 1;
    }
  }

  int ingredientSum = 0;
  int volume = 0;
  for (int i = 0; i < nb_ingredients; i++) {
    ingredientSum += IngredientRatio[i];
  }

  for (int i = 0; i < lastValve; i++) {
    nextValve(i);
    volume = int(double(IngredientRatio[i]) * double(Order.OrderVolume) / double(ingredientSum));
    if (volume > 0) {
      showLEDsIngredient(i);
      pump(volume);
      Ingredients_List[i].volume += (double(volume)/1000);
    }
  }
  init_Valves(true);
  pump(AIR_VOLUME);
}

void nextValve(int i) {
  pos = 0;
  goalpos = Ingredients_List[i].step;
  myservo.writeMicroseconds(1420);

  while (pos > goalpos) {
    getPos();
  }
  motorStop();
  pump(1);
}

void motorStop() {
  //delay(10);
  myservo.writeMicroseconds(1500);
  delay(10);

  bool running = true;
  lastStop = millis();
  while (running) {
    getPos();
    if (millis() - lastStop > 60) {
      lastStop = millis();
      Serial.println(pos);
      Serial.println(lastStopPos);
      Serial.println();
      if (pos == lastStopPos) {
        running = false;
      } else {
        lastStopPos = pos;
      }
    }
  }
  Serial.println("End Stop function");
}

void pump(int volume) {
  Serial.println("Pump function running");
  if (volume < 0) {
    volume = 0;
  }
  if (volume > ORDER_LARGE) {
    volume = ORDER_LARGE;
  }
  pos = 0;
  goalpos = STEPS_PER_ML * volume;
  //long int diff = goalpos - pos;
  myservo.writeMicroseconds(1800);

  while (pos < goalpos) {
    getPos();
    //diff = constrain(goalpos - pos, 0, 200);
    //diff = map(diff, 0, 200, 1650, 1800);
    //myservo.writeMicroseconds(diff);
  }
  motorStop();
  //myservo.writeMicroseconds(1500);
  //delay(1000);
}


void checkBattery() {
  unsigned long int VoltageSum = 0;
  for (int i = 0; i < 100; i++) {
    VoltageSum += analogRead(BAT_PIN);
    delayMicroseconds(1);
  }
  VoltageSum = VoltageSum / 100;

  if (VoltageSum < CUTOFF) {
    digitalWrite(RELAIS_PIN, LOW);
  }
  if (VoltageSum < LOWBAT) {
    shutdown = true;
  }
}

void checkShutdown() {
  if (shutdown) {
    for (int i = 0; i < 10; i++) {
      for (int ID = 0; ID < NUM_LEDS; ID++) {
        leds[ID].r = 0;
        leds[ID].b = 0;
        leds[ID].g = 0;
      }
      FastLED.show();
      delay(500);
      for (int ID = 0; ID < NUM_LEDS; ID++) {
        leds[ID].r = 0;
        leds[ID].b = 0;
        leds[ID].g = 150;
      }
      FastLED.show();
      delay(500);
    }
    digitalWrite(RELAIS_PIN, LOW);
  }
}

String getBatteryState() {
  unsigned long int VoltageSum = 0;
  for (int i = 0; i < 100; i++) {
    VoltageSum += analogRead(BAT_PIN);
    delayMicroseconds(1);
  }
  VoltageSum = VoltageSum / 100;
  if (VoltageSum < LOWBAT) {
    VoltageSum = LOWBAT;
  }
  if (VoltageSum > FULLBAT) {
    VoltageSum = FULLBAT;
  }
  int percent = map(VoltageSum, LOWBAT, FULLBAT, 0, 100);
  return String(percent);
}

void getPos() {
  bool H1 = digitalRead(HALL_PIN_1);
  bool H2 = digitalRead(HALL_PIN_2);
  bool H3 = digitalRead(HALL_PIN_3);

  if (H1 && !H2 && !H3) {
    if (lastState == 5) {
      pos++;
    }
    if (lastState == 1) {
      pos--;
    }
    lastState = 0;
  } else if (H1 && H2 && !H3) {
    if (lastState == 0) {
      pos++;
    }
    if (lastState == 2) {
      pos--;
    }
    lastState = 1;
  } else if (!H1 && H2 && !H3) {
    if (lastState == 1) {
      pos++;
    }
    if (lastState == 3) {
      pos--;
    }
    lastState = 2;
  } else if (!H1 && H2 && H3) {
    if (lastState == 2) {
      pos++;
    }
    if (lastState == 4) {
      pos--;
    }
    lastState = 3;
  } else if (!H1 && !H2 && H3) {
    if (lastState == 3) {
      pos++;
    }
    if (lastState == 5) {
      pos--;
    }
    lastState = 4;
  } else if (H1 && !H2 && H3) {
    if (lastState == 4) {
      pos++;
    }
    if (lastState == 6) {
      pos--;
    }
    lastState = 5;
  }
}