#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);
String uid;

String baseUrl = "http://192.168.0.111:5000";

const char* ssid = "Dism";
const char* password = "RoUmAhKITA";

#define led 12

#define relay1 27
#define relay2 26
#define relay3 14
#define relay4 25
#define relay5 13
#define relay6 33

#define input1 34
#define input2 19
#define input3 35
#define input4 18
#define input5 32
#define input6 5

int loker1, loker2, loker3, loker4, loker5, loker6;

int aktif = 0;
int pasif = 1;

DynamicJsonDocument doc(2048);
String command;
void setup() {
  Serial.begin(115200);
  nfc.begin();
  lcd.init();
  lcd.backlight();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  digitalWrite(relay1, pasif);
  digitalWrite(relay2, pasif);
  digitalWrite(relay3, pasif);
  digitalWrite(relay4, pasif);
  digitalWrite(relay5, pasif);
  digitalWrite(relay6, pasif);

  pinMode(led, OUTPUT);

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(relay5, OUTPUT);
  pinMode(relay6, OUTPUT);

  pinMode(input1, INPUT);
  pinMode(input2, INPUT);
  pinMode(input3, INPUT);
  pinMode(input4, INPUT);
  pinMode(input5, INPUT);
  pinMode(input6, INPUT);
  lcdDefault();
  delay(6000);
}

void loop() {
  //  while (Serial.available()) {
  //    char c = Serial.read();
  //    if (c == '\n') {
  //      Serial.println(command);
  //      getUser(command);
  //      command = "";
  //    }
  //    else {
  //      command += c;
  //    }
  //  }
  if (nfc.tagPresent(100)) {
    NfcTag tag = nfc.read();
    tag.print();
    uid = tag.getUidString();
    uid.replace(" ", "");
    Serial.println(uid);
    delay(300);
    getUser(uid);
  }
  delay(100);
}

void sensor () {
  for (int i = 0; i <= 5; i++) {
    loker1 = digitalRead(input1);
    loker2 = digitalRead(input2);
    loker3 = digitalRead(input3);
    loker4 = digitalRead(input4);
    loker5 = digitalRead(input5);
    loker6 = digitalRead(input6);

    Serial.print("Sensor 1 => " );
    Serial.println(loker1);
    Serial.print("Sensor 2 => " );
    Serial.println(loker2);
    Serial.print("Sensor 3 => " );
    Serial.println(loker3);
    Serial.print("Sensor 4 => " );
    Serial.println(loker4);
    Serial.print("Sensor 5 => " );
    Serial.println(loker5);
    Serial.print("Sensor 6 => " );
    Serial.println(loker6);
    Serial.println("");
    delay(100);
  }
}

void getUser (String uid) {
  Serial.println(uid);
  HTTPClient http;
  http.begin(baseUrl + "/api/history/user?uid=" + uid);
  int httpCode = http.GET();
  if (httpCode > 0)
  {
    String response = http.getString();
    Serial.println(response);
    deserializeJson(doc, response);
    String status = doc["status"];
    Serial.println(status);
    if (status == "success") {
      JsonObject data = doc["data"];
      int data_id = data["id"];
      int data_loker = data["loker"];
      int data_loker_id = data["loker_id"];
      Serial.println("Loker Ke - " + String(data_loker));
      Serial.println("Loker ID " + String(data_loker_id));
      if (data_loker == 0) {
        Serial.print("loker");
        Serial.println(data_loker);
        logikaPost(data_id);
      }
      else {
        logikaPut(data_loker, data_loker_id);
      }
    } else {
      String message = doc["message"];
      if (message == "User Expired") {
        Serial.println(message);
      } else {
        Serial.println("User Tidak Terdaftar");
        Serial.println(message);
      }
    }
  }
  Serial.println("");
  relayOff();
  http.end();
}

void postHistory (int user, int loker) {
  doc["user_id"] = user;
  doc["loker"] = loker;
  String _body;
  serializeJson(doc, _body);
  Serial.println("OK");
  Serial.println(_body);
  HTTPClient http;
  http.begin(baseUrl + "/api/history");
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.POST(_body);
  if (httpCode > 0) {
    String response = http.getString();
    Serial.println(response);
  }
  http.end();
}

void putHistory (int loker_id) {
  doc["id"] = loker_id;;
  String _body;
  serializeJson(doc, _body);
  Serial.println(_body);
  HTTPClient http;
  http.begin(baseUrl + "/api/history");
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.PUT(_body);
  if (httpCode > 0) {
    String response = http.getString();
    Serial.println(response);
  }
  http.end();
}

void relayOff() {
  digitalWrite(relay1, pasif);
  digitalWrite(relay2, pasif);
  digitalWrite(relay3, pasif);
  digitalWrite(relay4, pasif);
  digitalWrite(relay5, pasif);
  digitalWrite(relay6, pasif);
  Serial.println("Relay Off");
  lcdDefault();
}

void relayOn(int relay) {
  digitalWrite(relay, aktif);
  Serial.println("Relay On");
  delay(6000);
}

void logikaPost(int user_id) {
  sensor();
  delay(200);
  if (loker1 == 1) {
    lcdLoker(1);
    relayOn(relay1);
    sensor();
    if (loker1 == 0) {
      postHistory(user_id, 1);
    }
    return;
  }
  else if (loker2 == 1) {
    lcdLoker(2);
    relayOn(relay2);
    sensor();
    if (loker2 == 0) {
      postHistory(user_id, 2);
    }
    return;
  }
  else if (loker3 == 1) {
    lcdLoker(3);
    relayOn(relay3);
    sensor();
    if (loker3 == 0) {
      postHistory(user_id, 3);
    }
  }
  else if (loker4 == 1) {
    lcdLoker(4);
    relayOn(relay4);
    sensor();
    if (loker4 == 0) {
      postHistory(user_id, 4);
    }
    return;
  }
  else if (loker5 == 1) {
    lcdLoker(5);
    relayOn(relay5);
    sensor();
    if (loker5 == 0) {
      postHistory(user_id, 5);
    }
  }
  else if (loker6 == 1) {
    lcdLoker(6);
    relayOn(relay6);
    sensor();
    if (loker6 == 0) {
      postHistory(user_id, 6);
    }
    return;
  }
}


void logikaPut(int loker, int loket_id) {
  lcdLoker(loker);
  if (loker == 1) {
    relayOn(relay1);
    sensor();
    if (loker1 == 1) {
      putHistory(loket_id);
    }
    return;
  }
  else if (loker == 2) {
    relayOn(relay2);
    sensor();
    if (loker2 == 1) {
      putHistory(loket_id);
    }
    return;
  }
  else if (loker == 3) {
    relayOn(relay3);
    sensor();
    if (loker3 == 1) {
      putHistory(loket_id);
    }
    return;
  }
  else if (loker == 4) {
    relayOn(relay4);
    sensor();
    if (loker4 == 1) {
      putHistory(loket_id);
    }
    return;
  }
  else if (loker == 5) {
    relayOn(relay5);
    sensor();
    if (loker5 == 1) {
      putHistory(loket_id);
    }
    return;
  }
  else if (loker == 6) {
    relayOn(relay6);
    sensor();
    if (loker6 == 1) {
      putHistory(loket_id);
    }
    return;
  }
}

void lcdLoker(int loker) {
  Serial.print("LCD LOKER ke-");
  Serial.println(loker);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Silahkan Buka");
  lcd.setCursor(3, 1);
  lcd.print("Loker ke-");
  lcd.print(loker);
}

void lcdDefault () {
  lcd.setCursor(0, 0);
  lcd.print("Afrizal Gusnedy");
  lcd.setCursor(3, 1);
  lcd.print("065114162");
}
