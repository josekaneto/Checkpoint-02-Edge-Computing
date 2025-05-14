 //Library to use DHT sensor
#include <Adafruit_Sensor.h> 
#include <DHT.h>
#include <DHT_U.h>

//Library to use LCD
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTTYPE DHT22
#define DHTPIN 2
DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;

#define col 16
#define lin 2
#define ende 0x27

LiquidCrystal_I2C lcd(ende, col, lin);

byte pilar[8] = {
    B11111,
    B01110,
    B00100,
    B00100,
    B00100,
    B01110,
    B11111,
};

int ledRedPin = 11;
int ledYellowPin = 12;
int ledGreenPin = 13;
int ldrPin = A0;
int ldrValue = 0;
int buzzerPin = 10;

void setup() {
  //Serial monitor 9600 bps
  Serial.begin(9600);

  //Led's configuration
  pinMode(ledRedPin, OUTPUT);
  pinMode(ledYellowPin, OUTPUT);
  pinMode(ledGreenPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  
  //Starting sensor DHT
  dht.begin();
  Serial.println("Usando o Sensor DHT");
  sensor_t sensor; //Perguntar para o professor
  
  //Getting info about temp.
  dht.temperature().getSensor(&sensor);

  //Getting info about humidity.
  dht.humidity().getSensor(&sensor);

  //Settings of LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Grupo Pilar!");
  lcd.createChar(0, pilar);
  lcd.setCursor(15, 1);
  lcd.write(byte(0));
  delayMS = 5000;

}

void loop() {
  float tempSum = 0;
  float humidSum = 0;
  float lightSum = 0;
  int count = 0;
  int ldrValuePercentual = map(ldrValue, 1015, 8, 0, 100);

  delay(delayMS);

  for(int i = 0; i < 5; i++){
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    tempSum += event.temperature;
 
    dht.humidity().getEvent(&event);
    humidSum += event.relative_humidity;

    lightSum += ldrValuePercentual;

    count ++;
  }

  float averageTemp = tempSum / count;
  float averageHumid = humidSum / count;
  float averageLight = lightSum / count;

  lcd.clear();

  ldrValue = analogRead(ldrPin);
  Serial.println(averageLight);
  Serial.println(averageHumid);
  Serial.println(averageTemp);
  Serial.println(ldrValue);
  
  if(isnan(averageLight)){
    lcd.setCursor(0, 0);
    lcd.print("Erro Iluminação");
  }
  else {
    lcd.setCursor(0, 0);
    lcd.print("Ilum.: ");
    lcd.print(averageLight);
    lcd.print("%");
    lcd.setCursor(15,1);
    lcd.write(byte(0));

    if(averageLight == 100){
        digitalWrite(ledYellowPin, LOW);
        digitalWrite(ledGreenPin, LOW);
        digitalWrite(ledRedPin, HIGH);
        tone(buzzerPin, 262);
        lcd.setCursor(0, 1);
        lcd.print("Muito claro.");
    }
    else if (averageLight >= 50){
      digitalWrite(ledRedPin, LOW);
      digitalWrite(ledGreenPin, LOW);
      digitalWrite(ledYellowPin, HIGH);
      noTone(buzzerPin);
      lcd.setCursor(0, 1);
      lcd.print("Meia luz.");
    }
    else {
      digitalWrite(ledGreenPin, HIGH);
      digitalWrite(ledYellowPin, LOW);
      digitalWrite(ledRedPin, LOW);
      noTone(buzzerPin);
      lcd.print("Luminosidade OK!");
    }
  }

  delay(delayMS);
  lcd.clear();

  if(isnan(averageTemp)){
    lcd.print("Erro: Temp.");
  }
  else{
    lcd.setCursor(0, 0);
    lcd.print("Temp.: ");
    lcd.print(averageTemp);
    lcd.print("C");
    lcd.setCursor(15,1);
    lcd.write(byte(0));

    if(averageTemp >= 10 && averageTemp <= 15){
      lcd.setCursor(0, 1);
      lcd.print("Temperatura OK!");
    }
    else if(averageTemp > 15){
      digitalWrite(ledRedPin, LOW);
      digitalWrite(ledGreenPin, LOW);
      digitalWrite(ledYellowPin, HIGH);
      tone(buzzerPin, 262);
      lcd.setCursor(0, 1);
      lcd.print("Temp. Alta.");
    }
    else{
      digitalWrite(ledRedPin, LOW);
      digitalWrite(ledGreenPin, LOW);
      digitalWrite(ledYellowPin, HIGH);
      tone(buzzerPin, 262);
      lcd.setCursor(0, 1);
      lcd.print("Temp. Baixa.");
    }

  }

  delay(delayMS);

  if(isnan(averageHumid)){
    lcd.print("Erro: Umidade.");
  }
  else{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Umidade: ");
    lcd.print(averageHumid);
    lcd.println("%");
    lcd.setCursor(15, 1);
    lcd.write(byte(8));

    if(averageHumid >= 50 && averageHumid <= 70){
      lcd.setCursor(0, 1);
      lcd.print("Umidade OK!");
    }
    else if(averageHumid > 70){
      digitalWrite(ledRedPin, HIGH);
      digitalWrite(ledGreenPin, LOW);
      digitalWrite(ledYellowPin, LOW);
      tone(buzzerPin, 262);
      lcd.setCursor(0, 1);
      lcd.print("Umidade Alta.");
    }
    else{
      digitalWrite(ledRedPin, HIGH);
      digitalWrite(ledGreenPin, LOW);
      digitalWrite(ledYellowPin, LOW);
      tone(buzzerPin, 262);
      lcd.setCursor(0, 1);
      lcd.print("Umidade Baixa.");
    }
  }
}