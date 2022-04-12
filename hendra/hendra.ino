#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>
#include <LiquidCrystal_I2C.h>

const int buttonPin1 = D5;
const int buttonPin2 = D6;

PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);

LiquidCrystal_I2C lcd(0x26, 16, 2);
LiquidCrystal_I2C lcd1(0x27, 16, 2);
String baseUrl = "http://192.168.43.216:2000";
String user[2];
String candidate[2];
String uid;

String ssid = "Dism2";
String password = "00000000";
int dSort = 2000;
int dMid = 6000;
int dLong = 10000;

int button1 = 0, button2 = 0;
#include "api.h"

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  StaticJsonDocument<200> doc;
  lcd.init();
  lcd1.init();
  nfc.begin();
  lcd.backlight();
  lcd1.backlight();
  
  lcd1.setCursor(0, 0);
  lcd1.print("  Connecting ");
  lcd.setCursor(0, 0);
  lcd.print("    e-voting    ");
  lcd.setCursor(0, 1);
  lcd.print(" Hendra Wahyu S ");

  Serial.println("NDEF Reader");

  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  // lcd.setCursor(2, 0);
  // lcd.print("Connecting...");
  // lcd1.setCursor(2, 0);
  // lcd1.print("Hello, World!");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting...");
  }
  
  lcd1.setCursor(0, 0);
  lcd1.print("  Tekan Button ");
  lcd1.setCursor(0, 1);
  lcd1.print("Untuk Pengetsan");

  getCandidate();
  Serial.println(candidate[0]);
  Serial.println(candidate[1]);
  button();
  button();
  button();
  if (button1 == 1 || button2 == 1)
  {
    lcd1.clear();
    while (1)
    {
      button();
      lcd1.setCursor(0, 0);
      lcd1.print("Pengetesan Kartu");
      
        Serial.println("loop1");
      if (nfc.tagPresent()) {
          NfcTag tag = nfc.read();
          button();
          uid = tag.getUidString();
          uid.replace(" ", "");
          lcd1.setCursor(2, 1);
          lcd1.print(uid);
          lcd1.print("           ");
        Serial.println(uid);
          delay(100);
          if (button1 == 1 || button2 == 1) {break;}   

      }
          if (button1 == 1 || button2 == 1) {break;}   
          delay(100);   
    }
  }
  lcd.clear();
  lcd1.clear();
  Serial.println("Masuk Sistem");
}

void loop() {
  lcd1.setCursor(0, 0);
  lcd1.print(" Tempelkan KTP ");
  lcd1.setCursor(0, 1);
  lcd1.print("      Anda     ");
  lcd.setCursor(0, 0);
  lcd.print("    e-voting    ");
  lcd.setCursor(0, 1);
  lcd.print(" Hendra Wahyu S ");
  if (nfc.tagPresent()) {
    NfcTag tag = nfc.read();
    uid = tag.getUidString();
    lcd1.setCursor(2, 0);
    uid.replace(" ", "");

    lcd1.clear();
    lcd1.setCursor(0, 0);
    lcd1.print("Proses ...");
    getUser(uid);
    delay(dSort);
    if (user[0] == "belum")
    {
      lcd.clear();
      lcd1.clear();
      lcd1.setCursor(0, 0);
      lcd1.print("Silahkan Tentukn");
      lcd1.setCursor(0, 1);
      lcd1.print("  Pilihan Anda  ");
      
      lcd.setCursor(0, 0);
      lcd.print("  Didalam Bilik  ");
      lcd.setCursor(0, 1);
      lcd.print(user[1]);

      lcd1.clear();
      lcd1.setCursor(0, 0);
      lcd1.print("1. "+ candidate[0]);
      lcd1.setCursor(0, 1);
      lcd1.print("2. "+ candidate[1]);
      while (1)
      {
        button();
        if (button1 == 1)
        {
          createResult("satu", uid);
          lcd1.clear();
          lcd1.setCursor(0, 0);
          lcd1.print("Proses ...");
          delay(dSort);
          lcd1.clear();
          lcd1.setCursor(0, 0);
          lcd1.print("  Terimakasih  ");
          lcd1.setCursor(0, 1);
          lcd1.print(" Sudah Memilih ");
          delay(dMid);
          break;
        } else if(button2 == 1)
        {
          createResult("dua", uid);
          lcd1.clear();
          lcd1.setCursor(0, 0);
          lcd1.print("Proses ...");
          delay(dSort);
          lcd1.clear();
          lcd1.setCursor(0, 0);
          lcd1.print("  Terimakasih  ");
          lcd1.setCursor(0, 1);
          lcd1.print(" Sudah Memilih ");
          delay(dMid);
          break;
        }
        delay(100);
      }
      
    }
    else if (user[0] == "sudah")
    {      
      lcd.clear();
      lcd1.clear();
      lcd1.setCursor(0, 0);
      lcd1.print("   Anda Sudah   ");
      lcd1.setCursor(0, 1);
      lcd1.print("     Memilih    ");
      
      lcd.setCursor(0, 0);
      lcd.print(user[1]);
      lcd.setCursor(0, 1);
      lcd.print(" Sudah Memilih  ");
      delay(dLong);
    } 
    else {
      lcd1.setCursor(0, 0);
      lcd1.print("   KTP Tidak   ");
      lcd1.setCursor(0, 1);
      lcd1.print("   Terdaftar   ");

      lcd.setCursor(0, 0);
      lcd.print("   KTP Tidak   ");
      lcd.setCursor(0, 1);
      lcd.print("   Terdaftar   ");
      delay(dLong);
    }
    
  }
  delay(1000);
}

void button() {
  button1 = digitalRead(buttonPin1);
  button2 = digitalRead(buttonPin2);
  Serial.println(button1);
  Serial.println(button2);
}
