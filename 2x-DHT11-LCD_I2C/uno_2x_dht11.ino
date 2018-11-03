#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN1 2
#define DHTPIN2 3
#define DHTTYPE DHT11

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

static char tempstr1[15];
static char humstr1[15];
static char tempstr2[15];
static char humstr2[15];


unsigned long postTimer;
unsigned int showSensor = 1;

DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
}

void loop() {
  
  if(millis()-postTimer >= 3000UL) {
    postTimer = millis();
    postData();
  }
  
}

void postData() {
    
    dtostrf(dht1.readTemperature(),5, 1, tempstr1);
    dtostrf(dht1.readHumidity(),5, 1, humstr1);
    dtostrf(dht2.readTemperature(),5, 1, tempstr2);
    dtostrf(dht2.readHumidity(),5, 1, humstr2);

    if(showSensor == 1) {
      lcd.setCursor(0,0);
      lcd.print(tempstr1);
      lcd.setCursor(6,0);
      lcd.print("grader C");
      lcd.setCursor(0,1);
      lcd.print(humstr1);
      lcd.setCursor(6,1);
      lcd.print("procent");
      Serial.println(tempstr1);
      Serial.println(humstr1);
      showSensor = 2;
    } else {
    lcd.setCursor(0,0);
      lcd.print(tempstr2);
      lcd.setCursor(6,0);
      lcd.print("grader C");
      lcd.setCursor(0,1);
      lcd.print(humstr2);
      lcd.setCursor(6,1);
      lcd.print("procent");
      Serial.println(tempstr2);
      Serial.println(humstr2);
      showSensor = 1;
    }


}
