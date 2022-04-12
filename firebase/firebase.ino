#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "my-room-iot.firebaseio.com"
#define FIREBASE_AUTH "D9zz030ZzObNz5JOxLjIJQgFYYXVZQohMOgPIPoT"
#define WIFI_SSID "Dism"
#define WIFI_PASSWORD "RoUmAhKITA"

String command;
String dataIn;
String dt[10];
int i;
boolean parsing = false;

unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 1000; 

void setup() {
  Serial.begin(9600);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (Firebase.failed()) {
      Serial.print("pushing /logs failed:");
      Serial.println(Firebase.error());
      return;
    }
    onLed();
  }
}

void onLed () {
  int lock1 = Firebase.getInt("control/lock1");
  int lock2 = Firebase.getInt("control/lock2");
  int lock3 = Firebase.getInt("control/lock3");

  Serial.println(String ("*") + ("xx") + (",") + lock1 + (",") + lock2 + (",") + lock3 + ("#"));

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
  dt[j] = "";
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
}
