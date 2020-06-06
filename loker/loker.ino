#define BLYNK_PRINT Serial

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);

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
int send1 = 0;
int send2 = 0;
int send3 = 0;
int send4 = 0;
int send5 = 0;
int send6 = 0;

int aktif = 0;
int pasif = 1;

char auth[] = "mk-fD9isJvxYWT_Sm2JJWHB9rbU8CNhC";
char ssid[] = "Izey";
char pass[] = "izeybaik123";
char server[] = "119.18.158.238";
int port = 3579;

bool state = false;
int start = 1;
int duration = 10;

const unsigned char qrCode [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xf8, 0xff, 0xff, 0x1f, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xf8, 0xff, 0xff, 0x1f, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xf8, 0xff, 0xff, 0x1f, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x38, 0x1e, 0x78, 0x1c, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x38, 0x1c, 0x38, 0x1c, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x38, 0x1e, 0x38, 0x1c, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xe3, 0xfe, 0x38, 0x1f, 0xc0, 0x1c, 0x7f, 0xc7, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xe3, 0xfe, 0x38, 0x1f, 0xc0, 0x1c, 0x7f, 0xc7, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xe3, 0xfe, 0x38, 0x1f, 0xc0, 0x1c, 0x7f, 0xc7, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xe3, 0xfe, 0x38, 0x1f, 0xf0, 0x1c, 0x7f, 0xc7, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xe3, 0xfe, 0x38, 0x1f, 0xf8, 0x1c, 0x7f, 0xc7, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xe3, 0xfe, 0x38, 0x1f, 0xf8, 0x1c, 0x7f, 0xc7, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xe3, 0xfe, 0x38, 0x03, 0xc6, 0x1c, 0x7f, 0xc7, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xe3, 0xfe, 0x38, 0x03, 0xc7, 0x1c, 0x7f, 0xc7, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xe3, 0xfe, 0x38, 0x03, 0xc7, 0x1c, 0x7f, 0xc7, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x38, 0x00, 0x3f, 0x1c, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x38, 0x00, 0x3f, 0x1c, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x38, 0x00, 0x3f, 0x1c, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xf8, 0x61, 0xcf, 0x1f, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xf8, 0xe3, 0xc7, 0x1f, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xf8, 0xe3, 0xc7, 0x1f, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfc, 0x3e, 0x18, 0x1f, 0xcf, 0x18, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfc, 0x7e, 0x38, 0x1f, 0xc7, 0x1c, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfc, 0x7e, 0x38, 0x1f, 0xc7, 0x1c, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x07, 0x1c, 0x7f, 0xf8, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x07, 0x1c, 0x7f, 0xf8, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x07, 0x1c, 0x7f, 0xf8, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0xf0, 0xf8, 0x73, 0x80, 0xfc, 0x7b, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x03, 0xf1, 0xf8, 0xe3, 0xc0, 0xfc, 0x71, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x03, 0xf1, 0xf8, 0xe3, 0xc0, 0xfc, 0x71, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x07, 0xf0, 0xf8, 0xe3, 0xc0, 0xf8, 0x61, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xf0, 0x07, 0xe3, 0xf8, 0x00, 0x01, 0xf8, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xf0, 0x07, 0xe3, 0xf8, 0x00, 0x01, 0xf8, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xf0, 0x07, 0xe1, 0xf0, 0x00, 0x01, 0xf8, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xe0, 0x0f, 0xff, 0x00, 0x00, 0x1c, 0x0e, 0x38, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xe0, 0x0f, 0xff, 0x00, 0x00, 0x1c, 0x0e, 0x38, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xe0, 0x07, 0xff, 0x00, 0x00, 0x1c, 0x0c, 0x38, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xf8, 0x03, 0xc7, 0xff, 0xf1, 0xc0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xf8, 0x03, 0xc7, 0xff, 0xf1, 0xc0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xf8, 0x01, 0xcf, 0xff, 0xf1, 0xc0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x38, 0x00, 0x3f, 0xe3, 0xff, 0xc7, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x38, 0x00, 0x3f, 0xe3, 0xff, 0xc7, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x38, 0x00, 0x7f, 0xe7, 0xff, 0xc7, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xe3, 0xfe, 0x38, 0xe3, 0xff, 0x1f, 0x80, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xe3, 0xfe, 0x38, 0xe3, 0xff, 0x1f, 0x80, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xe3, 0xfe, 0x38, 0xe1, 0xff, 0x1f, 0x80, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xe3, 0xfe, 0x38, 0xe0, 0x7f, 0x00, 0x71, 0xc0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xe3, 0xfe, 0x38, 0xe0, 0x3f, 0x00, 0x71, 0xc0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xe3, 0xfe, 0x38, 0x60, 0x3e, 0x00, 0x71, 0xc0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xe3, 0xfe, 0x38, 0x1f, 0xc0, 0x1c, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xe3, 0xfe, 0x38, 0x1f, 0xc0, 0x1c, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xe3, 0xfe, 0x38, 0x1f, 0x80, 0x1c, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0xfc, 0x71, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x38, 0xe0, 0x00, 0xfc, 0x71, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x38, 0xf0, 0x00, 0xfc, 0x71, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xf8, 0xff, 0xf8, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xf8, 0xff, 0xf8, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xf8, 0xff, 0xf8, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00
};

BLYNK_CONNECTED() {
}

BLYNK_WRITE(V7)
{
  String textIn = param.asStr();
  Serial.println(textIn);
  Selonoid(textIn);
}

void setup() {
  Serial.begin(9600);
  digitalWrite(relay1, pasif);
  digitalWrite(relay2, pasif);
  digitalWrite(relay3, pasif);
  digitalWrite(relay4, pasif);
  digitalWrite(relay5, pasif);
  digitalWrite(relay6, pasif);

  duration *= 10;
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

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //or 0x3C
  display.clearDisplay(); //for Clearing the display
  display.drawBitmap(0, 0, qrCode, 128, 64, WHITE); // display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  display.display();

  //  Blynk.begin(auth, ssid, pass);
  Blynk.begin(auth, ssid, pass, server, port);

  sendParam();
  digitalWrite(led, HIGH);

}

void loop() {
  Blynk.run();
  sensor();
  if (state) {
    if (start == duration) {
      digitalWrite(relay1, pasif);
      digitalWrite(relay2, pasif);
      digitalWrite(relay3, pasif);
      digitalWrite(relay4, pasif);
      digitalWrite(relay5, pasif);
      digitalWrite(relay6, pasif);
      Serial.println("Relay Off");
      state = false;
    }
    start ++;
    Serial.println(start);
  } else {
    start = 0;
  }
  Serial.println();
  delay(100);
}

void Selonoid(String param) {
  Serial.println(param + " On");
  if (param == "Loker 1") {
    digitalWrite(relay1, aktif);
  }
  else if (param == "Loker 2") {
    digitalWrite(relay2, aktif);
  }
  else if (param == "Loker 3") {
    digitalWrite(relay3, aktif);
  }
  else if (param == "Loker 4") {
    digitalWrite(relay4, aktif);
  }
  else if (param == "Loker 5") {
    digitalWrite(relay5, aktif);
  }
  else if (param == "Loker 6") {
    digitalWrite(relay6, aktif);
  }
  state = true;
}

void sensor () {
  display.drawBitmap(0, 0, qrCode, 128, 64, WHITE); // display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  display.display();

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

  if (loker1 == aktif) {
    if (send1 == 0) {
      Blynk.virtualWrite(V1, 1);
      send1 = 1;
    }
  } else {
    if (send1 == 1) {
      Blynk.virtualWrite(V1, 0);
      send1 = 0;
    }
  }

  if (loker2 == aktif) {
    if (send2 == 0) {
      Blynk.virtualWrite(V2, 1);
      send2 = 1;
    }
  } else {
    if (send2 == 1) {
      Blynk.virtualWrite(V2, 0);
      send2 = 0;
    }
  }

  if (loker3 == aktif) {
    if (send3 == 0) {
      Blynk.virtualWrite(V3, 1);
      send3 = 1;
    }
  } else {
    if (send3 == 1) {
      Blynk.virtualWrite(V3, 0);
      send3 = 0;
    }
  }

  if (loker4 == aktif) {
    if (send4 == 0) {
      Blynk.virtualWrite(V4, 1);
      send4 = 1;
    }
  } else {
    if (send4 == 1) {
      Blynk.virtualWrite(V4, 0);
      send4 = 0;
    }
  }

  if (loker5 == aktif) {
    if (send5 == 0) {
      Blynk.virtualWrite(V5, 1);
      send5 = 1;
    }
  } else {
    if (send5 == 1) {
      Blynk.virtualWrite(V5, 0);
      send5 = 0;
    }
  }

  if (loker6 == aktif) {
    if (send6 == 0 ) {
      Blynk.virtualWrite(V6, 1);
      send6 = 1;
    }
  } else {
    if (send6 == 1) {
      Blynk.virtualWrite(V6, 0);
      send6 = 0;
    }
  }
}

void sendParam() {
  loker1 = digitalRead(input1);
  loker2 = digitalRead(input2);
  loker3 = digitalRead(input3);
  loker4 = digitalRead(input4);
  loker5 = digitalRead(input5);
  loker6 = digitalRead(input6);

  if (loker1 == aktif) {
    Blynk.virtualWrite(V1, 1);
    send1 = 1;
  } else {
    Blynk.virtualWrite(V1, 0);
    send1 = 0;
  }

  if (loker2 == aktif) {
    Blynk.virtualWrite(V2, 1);
    send2 = 1;
  } else {
    Blynk.virtualWrite(V2, 0);
    send2 = 0;
  }

  if (loker3 == aktif) {
    Blynk.virtualWrite(V3, 1);
    send3 = 1;
  } else {
    Blynk.virtualWrite(V3, 0);
    send3 = 0;
  }

  if (loker4 == aktif) {
    Blynk.virtualWrite(V4, 1);
    send4 = 1;
  } else {
    Blynk.virtualWrite(V4, 0);
    send4 = 0;
  }

  if (loker5 == aktif) {
    Blynk.virtualWrite(V5, 1);
    send5 = 1;

  } else {
    Blynk.virtualWrite(V5, 0);
    send5 = 0;
  }

  if (loker6 == aktif) {
    Blynk.virtualWrite(V6, 1);
    send6 = 1;
  } else {
    Blynk.virtualWrite(V6, 0);
    send6 = 0;
  }
}
