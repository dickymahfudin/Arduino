#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
 
const char *ssid = "Dism"; 
const char *password = "RoUmAhKITA";
 
const char *host = "my-room-iot.firebaseio.com";
const int Port = 443;  //HTTPS= 443 and HTTP = 80
 
const char fingerprint[] PROGMEM = "03 D6 42 23 03 D1 0C 06 73 F7 E2 BD 29 47 13 C3 22 71 37 1B";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);     
 
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); 
}
 
void loop() {
  WiFiClientSecure httpClient;
  httpClient.setFingerprint(fingerprint);
  httpClient.setTimeout(15000);
    
  int counter=0; // counter
  while((!httpClient.connect(host, Port)) && (counter < 30)){
      delay(50);
      Serial.print(".");
      counter++;
  }
  if(counter==30) {
    Serial.println("Connection failed");
  }
    
  String Link = "/light.json";
  httpClient.print(String("GET ") + Link + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +               
               "Connection: close\r\n\r\n");
 
  while (httpClient.connected()) {
    String line = httpClient.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
  }
 
  String line;
  while(httpClient.available()){        
    line = httpClient.readStringUntil('\n');
    Serial.println(line);
  }
  Serial.println("");
    
  delay(1000);
}
