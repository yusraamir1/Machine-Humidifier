#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27,16,2); 
//Constants
#define DHTPIN 7     
#define DHTTYPE DHT22   
DHT dht(DHTPIN, DHTTYPE); 

float h = 0.0;  
float t = 0.0; 
const int PB_UP = 2;
const int PB_DOWN = 3;
int humidity_setpoint = 85;
unsigned long previousMillis = 0;  
const long interval = 1000; 
void setup()
{
    Serial.begin(9600);
    delay(500);
    Serial.println("Temperature and Humidity Sensor Test");
    dht.begin();
    delay(1000);
    lcd.init(); 
    lcd.print("Initializing");
    pinMode(8,OUTPUT); //FAN
    pinMode(9,OUTPUT); 
    pinMode(PB_UP,INPUT_PULLUP);
    pinMode(PB_DOWN,INPUT_PULLUP);
}

void loop()
{
    unsigned long currentMillis = millis();
    int var_1 = digitalRead(PB_UP);
    int var_2 = digitalRead(PB_DOWN);

    if (var_1 == LOW){
      humidity_setpoint += 1;
      if (humidity_setpoint > 100){
        humidity_setpoint = 100;
      
      Serial.print("Set Point: ");
      Serial.print(humidity_setpoint);
      Serial.println("%");
      while (digitalRead(PB_UP) == LOW)
        delay(10);
      }
    }
    if (var_2 == LOW){
      humidity_setpoint -= 1;
      if (humidity_setpoint < 0){
        humidity_setpoint = 0;
      Serial.print("Set Point: ");
      Serial.print(humidity_setpoint);
      Serial.println("%");
      while (digitalRead(PB_DOWN) == LOW)
        delay(10);
    }
    }
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
    h = dht.readHumidity();
    t = dht.readTemperature();
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %, Setpoint: ");
    Serial.println(humidity_setpoint);

    lcd.setCursor(0, 0);
    lcd.print("T: ");
    lcd.print(t);
    lcd.print("C");

    lcd.setCursor(11, 1);
    lcd.print("H:");
    lcd.print(h);
    lcd.print("%");
    
  delay(1000); 
  if (h < humidity_setpoint) {
    // to turn off humidifiers)
    digitalWrite(9, HIGH);
    digitalWrite(8, HIGH);//FAN
    } else {
    // Turn on the humidifiers
    digitalWrite(9, LOW);
    digitalWrite(8, LOW);//FAN
    }
  }
}
