#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_SH1106.h>
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);
Adafruit_SH1106 display(21, 22);

const int buttonPin1 = 19;
const int buttonPin2 = 18;
const int buttonPin3 = 5;
const int buttonPin4 = 17;
const int buttonPin5 = 16;

int btn1 = 0;
int btn2 = 0;
int btn3 = 0;
int btn4 = 0;
int btn5 = 0;

String baseUrl = "http://107.175.247.242:8002";
const char* ssid = "Dism";
const char* password = "RoUmAhKITA";

DynamicJsonDocument doc(2048);

String poli = "";
String uid = "";

int count = 0;
int delayCount = 50; //50 25detik, 100 50 detik

void setup() {
  Serial.begin(115200);
  nfc.begin();

  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  pinMode(buttonPin4, INPUT);
  pinMode(buttonPin5, INPUT);

  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0, 28);
  display.println("Connecting");
  display.display();
  delay(3000);
  display.clearDisplay();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  btn();
  menu();
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  poli = "";
}

void loop() {
  lcdDefault();
  btn();
  if (poli != "") {
    lcdTag();
    while (count <= delayCount) {
      readEktp();
      count++;
      delay(100);
    }
    poli = "";
  }
  count = 0;
  delay(100);
}

void btn () {
  btn1 = digitalRead(buttonPin1);
  btn2 = digitalRead(buttonPin2);
  btn3 = digitalRead(buttonPin3);
  btn4 = digitalRead(buttonPin4);
  btn5 = digitalRead(buttonPin5);
  Serial.println(btn1);
  Serial.println(btn2);
  Serial.println(btn3);
  Serial.println(btn4);
  Serial.println(btn5);
  Serial.println();

  if (btn1 == 1) poli = "Poli Umum";
  if (btn2 == 1) poli = "Poli Gigi";
  if (btn3 == 1) poli = "Poli Penyakit Dalam";
  if (btn4 == 1) poli = "Poli Anak";
  if (btn5 == 1) poli = "Poli Kandungan";

  if (poli != "")  Serial.println(poli);
}

void readEktp () {
  if (nfc.tagPresent(100)) {
    NfcTag tag = nfc.read();
    //        tag.print();
    uid = tag.getUidString();
    uid.replace(" ", "");
    Serial.println(uid);
    lcdLoading();
    postAntrian(uid, poli);
  }
}

void postAntrian (String uid, String poli) {
  doc["poli"] = poli;
  String _body;
  serializeJson(doc, _body);
  HTTPClient http;
  http.begin(baseUrl + "/api/v1/antrian/" + uid);
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.POST(_body);
  Serial.println(httpCode);
  if (httpCode > 0) {
    String response = http.getString();
    Serial.println(response);
    deserializeJson(doc, response);
    int no_antrian = doc["no_antrian"];
    String detail = doc["detail"];
    bool aktif = doc["aktif"]; // true

    if (no_antrian != 0) lcdAntri(no_antrian);
    if (detail == "User sudah mengantri" && no_antrian == 0) lcdExist();
    if (aktif == true) lcdNotFound();

    Serial.println(no_antrian);
    Serial.println(detail);
    Serial.println(aktif);
  }
  count = delayCount;
  http.end();
}

void lcdDefault () {
  //size 1 21 Huruf, size 2 10 huruf
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("   Nisa   ");
  display.setTextSize(1);
  display.setCursor(0, 28); // 120,56
  display.println("    Eskaibnariyati  ");
  display.setTextSize(2);
  display.setCursor(0, 50); // `120,56
  display.println(" 065118709");
  display.display();
  display.clearDisplay();
}

void lcdTag() {
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(poli);
  display.setTextSize(2);
  display.setCursor(0, 28);
  display.println(" Tempelkan");
  display.setCursor(0, 45);
  display.println("   EKTP");
  display.display();
  display.clearDisplay();
}

void lcdNotFound() {
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("Kartu Tidk");
  display.setCursor(0, 28);
  display.println(" Terdaftar");
  display.display();
  delay(5000);
  display.clearDisplay();
}

void lcdExist () {
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println(" Lihat No");
  display.setCursor(0, 25);
  display.println("Antrian Di");
  display.setCursor(0, 45);
  display.println("   Layar");
  display.display();
  delay(5000);
  display.clearDisplay();
}

void lcdAntri (int antri) {
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("Antrian Ke");
  display.setTextSize(4);
  display.setCursor(45, 28); // `120,56
  display.println(antri);
  display.display();
  delay(5000);
  display.clearDisplay();
}

void lcdLoading() {
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0, 28);
  display.println("Loading...");
  display.display();
  delay(1000);
  display.clearDisplay();
}

void menu () {
  poli = "Poli Umum";
  while (1) {
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(0, 28);
    display.println("SET EKTP");
    display.display();
    display.clearDisplay();
    btn();
    if (btn2 == 1)break;
    readEktp();
    delay(100);
  }
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("MASK SISTM");
  display.setCursor(0, 28);
  display.println(" TUNGGU");
  display.display();
  display.clearDisplay();
  delay(4000);
}
