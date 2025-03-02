#include "DHT.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define ledPinB 18
#define ledPinG 19
#define ledPinR 12

#define ledPinWl 23
#define ledPinWyl 13

#define dhtPin 4
#define DHTTYPE DHT11

#define buttonPinH 15
#define buttonPinP 2
bool wlaczonyH = false;
bool wlaczonyP = false;
bool lastButtonStateH = HIGH;
bool lastButtonStateP = HIGH;
bool buttonStateH;
bool buttonStateP;

bool ButtonSignal = true;

DHT dht11(dhtPin, DHT11);

void blue(){
  analogWrite(ledPinR, 0);
  analogWrite(ledPinG, 0);
  analogWrite(ledPinB, 255);
}
void orange(){
  analogWrite(ledPinR, 255);
  analogWrite(ledPinG, 20);
  analogWrite(ledPinB, 0);
}
void yellow(){
  analogWrite(ledPinR, 255);
  analogWrite(ledPinG, 255);
  analogWrite(ledPinB, 0);
}
void lblue(){
  analogWrite(ledPinR, 51);
  analogWrite(ledPinG, 255);
  analogWrite(ledPinB, 255);
}
void red(){
    analogWrite(ledPinR, 255);
    analogWrite(ledPinG, 0);
    analogWrite(ledPinB, 0);

}
void diodeH_off(){
  analogWrite(ledPinG, 0);
  analogWrite(ledPinB, 0);
  analogWrite(ledPinR,0);
}
void diodeP_off(){
  analogWrite(ledPinWl,0);
  analogWrite(ledPinWyl,120);
}
void diodeP_on(){
  analogWrite(ledPinWl,120);
  analogWrite(ledPinWyl,0);
}
void off_state(bool wlaczonyH, bool lastButtonStateH, bool buttonStateH){
  wlaczonyH = false;
  lastButtonStateH = HIGH;
  buttonStateH = LOW;
}
void print_readings(float temp, float humi){
  display.clearDisplay();
  display.setTextSize(1.7);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Temperatura: ");
  display.print(temp);
  display.println("*C");
  display.println("\n------------");
  display.println("\nWilgotnosc: ");
  display.print(humi);
  display.println("%");
  display.display();
}
void pin_setup(){
  pinMode(ledPinR, OUTPUT);
  pinMode(ledPinG, OUTPUT);
  pinMode(ledPinB, OUTPUT);
  pinMode(ledPinWl, OUTPUT);
  pinMode(ledPinWyl, OUTPUT);
  pinMode(buttonPinH, INPUT);
  pinMode(buttonPinP, INPUT);
}
void startDisplay(){
   if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
  display.setTextSize(1.5);
  display.setTextColor(WHITE);
  display.setCursor(0, 30);
  // Display static text
  display.println("POMIAR TEMPERATURY \nORAZ WILGOTNOSCI");
  display.display();
  delay(100);
}

void setup() {
  Serial.begin(115200);
  pin_setup();
  dht11.begin();
  delay(2000);
  startDisplay();
}

void loop() {
  if(ButtonSignal) analogWrite(ledPinWyl,120); //świeci sie swieczka 
  buttonStateP = digitalRead(buttonPinP);

  if (buttonStateP == HIGH && lastButtonStateP == HIGH) {
    wlaczonyP = !wlaczonyP; // Przełączanie stanu
    delay(200);
    if(wlaczonyP){
      diodeP_on();
      startDisplay();
    }
    else{
      display.clearDisplay();
      display.display();
      diodeH_off();
      diodeP_off();
      off_state(wlaczonyH, lastButtonStateH, buttonStateH);
    }
  }
  if (wlaczonyP){
    buttonStateH = digitalRead(buttonPinH);
    if (buttonStateH){
      float humi = dht11.readHumidity();
      float temp = dht11.readTemperature();
      delay(2000);
      if(temp>30){
        red();
      }
      else if(temp>20){
        orange();
      }
      else if(temp>10){
        yellow();
      }
      else if(temp>0){
        lblue();
      }
      else{
        blue();
      }
      print_readings(temp, humi);
    }
  }
  ButtonSignal = false;
  lastButtonStateP = buttonStateP;
}