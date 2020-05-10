#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "nodemcuiot-7334a.firebaseio.com"
#define FIREBASE_AUTH "i8LdJWNW7xPVzOMJjp1lZPYXITRHCd1saVpg9tqD"
#define WIFI_SSID "Dism1"
#define WIFI_PASSWORD "00000000"

#define Pled1 D7
#define Pled2 D2

String command;
String dataIn;
String dt[10];
int i;
boolean parsing = false;

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
  pinMode(Pled1, OUTPUT);
  pinMode(Pled2, OUTPUT);

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  if (Firebase.failed()) {
    Serial.print("pushing /logs failed:");
    Serial.println(Firebase.error());
    return;
  }
  onLed();
  getdata();
  delay(1000);
}

void onLed () {
  int led1 = Firebase.getInt("led1");
  int led2 = Firebase.getInt("led2");

  Serial.println(String ("*")  + ("xx") + (",") + led1 + (",") + led2 + ("#"));

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
