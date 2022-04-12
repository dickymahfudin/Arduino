#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h> //library OneWire
#include <DallasTemperature.h>
#define ONE_WIRE_BUS1 2
#define ONE_WIRE_BUS2 3
#define relay1 6
#define relay2 7
LiquidCrystal_I2C lcd(0x27, 16, 2);

OneWire oneWire1(ONE_WIRE_BUS1);
OneWire oneWire2(ONE_WIRE_BUS2);
DallasTemperature sensors1(&oneWire1);
DallasTemperature sensors2(&oneWire2);
float suhu1 , suhu2;
int panjang = 50;
int lebar = 40;
int trig_pin = 5;
int echo_pin = 4;
long echotime;
int distance, liter;
int on = 1;
int of = 0;
int sensorUltra;
long angka;
void setup()
{
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Essa Fawwaz Azha");
  lcd.setCursor(0, 1);
  lcd.print("   065113086");
  sensors1.begin();
  sensors2.begin();
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);

  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
  digitalWrite(trig_pin, LOW);
  digitalWrite(relay1, on);
  digitalWrite(relay2, on);
  delay(3000);
}


void loop()
{
  sensor();
  ultra();
  tampil();
  logika();
  delay(1000);
}

void sensor() {
  sensors1.requestTemperatures();
  sensors2.requestTemperatures();
  suhu1 = sensors1.getTempCByIndex(0);
  suhu2 = sensors2.getTempCByIndex(0);

  Serial.print("Suhu 1 -> ");
  Serial.println(suhu1);
  Serial.print("Suhu 2 -> ");
  Serial.println(suhu2);
}

void ultra() {
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);
  echotime = pulseIn(echo_pin, HIGH);
  distance = 0.0001 * ((float)echotime * 340.0) / 2.0;
  sensorUltra = distance-36;
  liter = (panjang * lebar * sensorUltra) / 1000;
  Serial.print(distance);
  Serial.println(" cm");
  Serial.print(liter);
  Serial.println(" liter");
}

void tampil() {
  lcd.setCursor(0, 0);
  lcd.print("Suhu1:");
  lcd.print(suhu1);
  lcd.print("C ");
  lcd.print(liter + 40);
  lcd.print("L ");

  lcd.setCursor(0, 1);
  lcd.print("Suhu2:");
  lcd.print(suhu2);
  lcd.print("C ");
}

void logika () {
  if (suhu1 < 33) {
    digitalWrite(relay1, on);
    digitalWrite(relay2, on);
  } else {
    digitalWrite(relay1, of);
    digitalWrite(relay2, of);
  }
}
