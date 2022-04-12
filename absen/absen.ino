#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

#define led1 18
#define led2 5

SoftwareSerial TAG1(22, 23); // RX and TX
SoftwareSerial TAG2(19, 21);

int rate = 9600;

const char* ssid = "Fakhri";
const char* password =  "katasandi13";

const String baseUrl = "http://107.175.247.242:2000";
//const String baseUrl = "http://192.168.0.133:2000";
String uidCard;
String paramAbsen;

char c;
DynamicJsonDocument doc(2048);
void setup()
{
  Serial.begin(rate);
  TAG1.begin(rate);
  TAG2.begin(rate);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(led1, HIGH);
    delay(500);
    digitalWrite(led1, LOW);
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  delay(3000);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  Serial.println("Connected to the WiFi network");

}

void loop()
{
  while (TAG1.available() > 0 || TAG2.available() > 0) {
    if (TAG1.available() > 0) {
      c = TAG1.read();
      paramAbsen = "absen1";
    }
    if (TAG2.available() > 0) {
      c = TAG2.read();
      paramAbsen = "absen2";
    }
    uidCard += c;
  }

  if (uidCard.length() > 20) absen();
}

void absen()
{
  Serial.println(uidCard.length());
  uidCard = uidCard.substring(1, 11);
  led();
  int user = getUser(uidCard);
  Serial.println(" user id -> " + user);
  if (user != 0) {
    String absen = userAbsen(user, paramAbsen);
    Serial.println("status Absen -> " + absen);
    if (absen == "success") {
      led();
    }
  }
  delay(2000);
  Serial.println(" ");
  uidCard = "";
  clearSerial();
  Serial.println("clear");
}

void clearSerial() {
  Serial.println("Masuk Clear");
  while (TAG1.read() >= 0 || TAG2.read() >= 0) {
    ;
  }
}

void led() {
  if (paramAbsen == "absen1") digitalWrite(led1, HIGH);
  if (paramAbsen == "absen2") digitalWrite(led2, HIGH);
  delay(5000);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
}

int getUser (String uid) {
  Serial.println(uid);
  HTTPClient http;
  http.begin(baseUrl + "/api/absen/user?uid=" + uid);
  int httpCode = http.GET();
  if (httpCode > 0)
  {
    String input = http.getString();
    Serial.println(input);
    deserializeJson(doc, input);

    const char* status = doc["status"]; // "success"
    JsonObject data = doc["data"];
    int data_id = data["id"]; 
    return data_id;
  }
  http.end();
}

String userAbsen (int id, String absen) {
  HTTPClient http;
  http.begin(baseUrl + "/api/absen/alat/" + absen + "?user_id=" + id);
  int httpCode = http.GET();
  if (httpCode > 0)
  {

    String input = http.getString();
    Serial.println(input);
    deserializeJson(doc, input);
    const char* status = doc["status"]; // "success"

    JsonObject data = doc["data"];
    const char* data_user_id = data["user_id"]; // "1"
    bool data_absen1 = data["absen1"]; // true
    bool data_absen2 = data["absen2"]; // true
    return status;
  }
  http.end();
}
