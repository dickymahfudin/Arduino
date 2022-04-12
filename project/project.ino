#include <ArduinoJson.h>
#include <Ethernet.h>
#include <SPI.h>

EthernetClient client;

#define pin0 3
#define pin1 4

// Name address for Open Weather Map API
const char* server = "bookingroom.spero.id";

// Replace with your unique URL resource
const char* resource = "/api/get_data/android";

const unsigned long HTTP_TIMEOUT = 10000;  // max respone time from server
const size_t MAX_CONTENT_SIZE = 512;       // max size of the HTTP response

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

// The type of data that we want to extract from the page
struct clientData {
  //  char temp[0];
  //  char humidity[0];
  //  char visibility;
};

const char* statuss;
const char* message;
const char* data;

// ARDUINO entry point #1: runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  pinMode(pin0, OUTPUT);
  pinMode(pin1, OUTPUT);
  
  while (!Serial) {
    ;  // wait for serial port to initialize
  }
  Serial.println("Serial ready");
  if (!Ethernet.begin(mac)) {
    Serial.println("Failed to configure Ethernet");
    return;
  }
  Serial.println("Ethernet ready");
  delay(1000);
}

// ARDUINO entry point #2: runs over and over again forever
void loop() {
  if (connect(server)) {
    if (sendRequest(server, resource) && skipResponseHeaders()) {
      clientData clientData;
      if (readReponseContent(&clientData)) {
        printclientData(&clientData);
      }
    }
  }
  disconnect();
  wait();
}

// Open connection to the HTTP server
bool connect(const char* hostName) {
  Serial.print("Connect to ");
  Serial.println(hostName);

  bool ok = client.connect(hostName, 80);

  Serial.println(ok ? "Connected" : "Connection Failed!");
  return ok;
}

// Send the HTTP GET request to the server
bool sendRequest(const char* host, const char* resource) {
  Serial.print("GET ");
  Serial.println(resource);

  client.print("GET ");
  client.print(resource);
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.println(host);
  client.println("Connection: close");
  client.println();

  return true;
}

// Skip HTTP headers so that we are at the beginning of the response's body
bool skipResponseHeaders() {
  // HTTP headers end with an empty line
  char endOfHeaders[] = "\r\n\r\n";

  client.setTimeout(HTTP_TIMEOUT);
  bool ok = client.find(endOfHeaders);

  if (!ok) {
    Serial.println("No response or invalid response!");
  }
  return ok;
}

bool readReponseContent(struct clientData* clientData) {
  const size_t bufferSize = JSON_OBJECT_SIZE(3) + 36;
  DynamicJsonBuffer jsonBuffer(bufferSize);

  JsonObject& root = jsonBuffer.parseObject(client);

  if (!root.success()) {
    Serial.println("JSON parsing failed!");
    return false;
  }

  statuss = root["status"];
  message = root["message"];
  data = root["data"];
  return true;
}

// Print the data extracted from the JSON
void printclientData(const struct clientData* clientData) {
  Serial.print("Status = ");
  Serial.println(statuss);
  Serial.print("message = ");
  Serial.println(message);
  Serial.print("data = ");
  Serial.println(data);
  Serial.println("");

  //data 01 = 10
  //data 00 = 01

  if (strcmp(data, "01") == 0) {
    digitalWrite(pin0, HIGH);
    digitalWrite(pin1, LOW);
    Serial.println("LED ON");
  }
  else {
    digitalWrite(pin0, LOW);
    digitalWrite(pin1, HIGH);
    Serial.println("led off");
  }
}

// Close the connection with the HTTP server
void disconnect() {
  Serial.println("Disconnect");
  client.stop();
}

// Pause for a 1 minute
void wait() {
  Serial.println("Wait 2 seconds");
  //  delay(60000);
  delay(1000);
}
