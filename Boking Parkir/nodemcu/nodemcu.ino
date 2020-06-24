#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "WString.h"
#include <Arduino.h>

#define led D1

const char* ssid = "Dism";
const char* password = "RoUmAhKITA";
String command;
String dataIn;
String dt[10];
int i;
boolean parsing = false;

unsigned long previousMillis = 0;
const long interval = 6000;

void setup () {
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  dataIn = "";
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(led, HIGH);
    delay(500);
    digitalWrite(led, LOW);
    delay(500);
    Serial.println("Connecting..");
  }
  Serial.println("Masuk Pak");
  //    putt();
  digitalWrite(led, HIGH);
}

void loop() {
  getdata();

  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
      // save the last time you blinked the LED
      previousMillis = currentMillis;

      HTTPClient http;  //Declare an object of class HTTPClient
      http.begin("http://skripsiparkir.pythonanywhere.com/api/parkirs/?format=json");
      int httpCode = http.GET();
      if (httpCode > 0) { //Check the returning code
        String payload = http.getString();   //Get the request response payload
        Serial.println(payload);
        delay(100);
      }
      http.end();   //Close connection
    }
  }
  delay(10);
}

void putt() {
  while (1) {
    const size_t bufferSize = JSON_OBJECT_SIZE(3);
    DynamicJsonBuffer jsonBuffer(bufferSize);

    JsonObject& root = jsonBuffer.createObject();
    root["booking_place"] = "Book A2";
    root["user"] = "others";
    root["book_status"] = "false";
    int len = root.measureLength() + 1;
    char rootstr[len];
    root.printTo(rootstr, len);
    HTTPClient http;
    http.begin("http://192.168.0.103:8000/api/parkirs/Book%20A2/");
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.PUT(rootstr);
    //    Serial.println(httpCode);
    http.end();
    delay(5000);
  }

}


void getdata() {
  if (Serial.available() > 0)
  {
    char inChar = (char)Serial.read();
    dataIn += inChar;
    if (inChar == '\n') {
      parsing = true;
    }
  }
  if (parsing)
  {
    parsingData();
    parsing = false;
    dataIn = "";
  }
}

void parsingData() {
  int j = 0;
  int z = 0;
  //inisialisasi variabel, (reset isi variabel)
  dt[j] = "";
  //proses parsing data
  for (i = 1; i < dataIn.length(); i++)
  {
    //pengecekan tiap karakter dengan karakter (#) dan (,)
    if ((dataIn[i] == '#') || (dataIn[i] == ','))
    {
      j++;
      dt[j] = "";
      z++;
    }
    else
    {
      dt[j] = dt[j] + dataIn[i];
    }
  }
  if (z >= 2) {
    const size_t bufferSize = JSON_OBJECT_SIZE(3);
    DynamicJsonBuffer jsonBuffer(bufferSize);

    JsonObject& root = jsonBuffer.createObject();
    root["booking_place"] = dt[0];
    root["user"] = dt[1];
    root["book_status"] = dt[2];
    int len = root.measureLength() + 1;
    char rootstr[len];
    root.printTo(rootstr, len);
    HTTPClient http;
    String a = dt[0];
    if (a == "Book A1") {
      a = "Book%20A1";
    }
    else if (a == "Book A2") {
      a = "Book%20A2";
    }
    else if (a == "Book A3") {
      a = "Book%20A3";
    }

    http.begin("http://skripsiparkir.pythonanywhere.com/api/parkirs/" + a + "/");
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.PUT(rootstr);
    Serial.println(httpCode);
    http.end();
    delay(1000);
  }
}
