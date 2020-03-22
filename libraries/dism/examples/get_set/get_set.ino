#include <dism.h>
#define led D1

dism object;
const char* ssid = "Dism1";
const char* password = "00000000";
String ledvalue;
int n;

void setup() { 
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Antosan...");
    }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    ledvalue = object.dism_get("1"); //1 id di database (getDB)
    Serial.println(ledvalue);
    object.dism_set("2", String(n)); //id database, value (setDB)
    
    if(ledvalue != "ERROR") {
      digitalWrite(led, ledvalue.toInt());
    }
  }
  n++;
  delay(10);
}
