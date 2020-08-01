#include <SPI.h>
#include <ArduinoJson.h>
#include <RFID.h>

#define echoPin1 2
#define trigPin1 3
#define echoPin2 4
#define trigPin2 5
#define SS_PIN 10
#define RST_PIN 9
#include <Servo.h>
Servo servoParkir1, servoParkir2, servoParkir3;

RFID rfid(SS_PIN, RST_PIN);
int serNum[5];          //Variable buffer Scan Card
int cards[][5] = {      //ID Kartu yang diperbolehkan masuk
  {235, 235, 232, 33, 201},
  {57, 177, 249, 162, 211},
  {136, 4, 80, 120, 164}
};
const char* a[2][3];
const char* cekbook[3][3];
int cekBook[][2] = { { 1, 1 }, { 1, 3 }, { 1, 2 } };
int cekUser;
String command;
//Servo servo[3] =  { servoParkir1, servoParkir2, servoParkir3 };
String user[] = {{"dism"}, {"tommy"}, {"admin"}};
bool  access = false;
bool statusparkirtomi = false;
bool statusparkirdism = false;
bool statusparkiradmin = false;
bool parkir1 = false;
bool parkir2 = false;
bool parkir3 = false;

long duration1, duration2;
int distance1, distance2;
void setup() {
  Serial.begin(9600);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  servoParkir1.attach(6);
  servoParkir2.attach(7);
  servoParkir3.attach(8);
  SPI.begin();
  rfid.init();

  servoParkir1.write(0);
  servoParkir2.write(0);
  servoParkir3.write(0);
}

void loop() {
  getdata();
  if (rfid.isCard()) {
    if (rfid.readCardSerial()) {
//            Serial.print(rfid.serNum[0]);
//            Serial.print(" ");
//            Serial.print(rfid.serNum[1]);
//            Serial.print(" ");
//            Serial.print(rfid.serNum[2]);
//            Serial.print(" ");
//            Serial.print(rfid.serNum[3]);
//            Serial.print(" ");
//            Serial.print(rfid.serNum[4]);
//            Serial.println("");

      for (int x = 0; x < sizeof(cards); x++) {
        for (int i = 0; i < sizeof(rfid.serNum); i++ ) {
          if (rfid.serNum[i] != cards[x][i]) {
            access = false;
            break;
          } else {
            cekUser = x;
            access = true;
          }
        }
        if (access) break;
      }
    }

    if (access) {
      berhasil();
    }

    else {
      Serial.println("Not allowed!");
    }

    Serial.println("");
    rfid.halt();
  }

}

void berhasil() {
  Serial.println(user[cekUser]);
  for (int i = 0 ; i < 3 ; i++) {
    //    Serial.println("ini");
    String cekplace = cekbook[i][0];
    String cekuserbook = cekbook[i][1];
    String cekstatus = cekbook[i][2];
    //    Serial.println(cekplace);
    //        Serial.println(cekuserbook);
    //    Serial.println(cekstatus);

    if (user[cekUser] == "tommy") {
      if (user[cekUser] == cekuserbook) {
        if (cekstatus == "true" && statusparkirtomi == false) {
          Serial.println("Tommy Sudah Boking");
          String a = "Buka Servo Ke -> ";
          a += cekplace;
          statusparkirtomi = true;
          Serial.println(a);
          konservo(cekplace);
        }
        else if (cekstatus == "true" && statusparkirtomi == true) {
          Serial.println("tommy Keluar");
          statusparkirtomi = false;
          cekbook[i][2] = "false";
          konservo(cekplace);
          Serial.println(String ("*") + cekplace + (",") + ("others,") + ("false#"));
          delay(100);
        }
      }
    }

    else if (user[cekUser] == "admin") {
      if (cekuserbook == "admin") {
        if (cekstatus == "true" && statusparkiradmin == false) {
          Serial.println("User Sudah Boking");
          String a = "Buka Servo Ke -> ";
          a += cekplace;
          statusparkiradmin = true;
          Serial.println(a);
          konservo(cekplace);
        }
        else if (cekstatus == "true" && statusparkiradmin == true) {
          Serial.println("Keluar");
          statusparkiradmin = false;
          cekbook[i][2] = "false";
          konservo(cekplace);
          Serial.println(String ("*") + cekplace + (",") + ("others,") + ("false#"));
          delay(100);
        }
      }
    }

    else if (user[cekUser] == "dism") {
      if (cekuserbook != "tommy" && cekuserbook != "others" && cekuserbook != "admin" && cekuserbook != "") {
        if (cekstatus == "true" && statusparkirdism == false) {
          Serial.println("User Sudah Boking");
          String a = "Buka Servo Ke -> ";
          a += cekplace;
          statusparkirdism = true;
          Serial.println(a);
          konservo(cekplace);
        }
        else if (cekstatus == "true" && statusparkirdism == true) {
          Serial.println("Keluar");
          statusparkirdism = false;
          cekbook[i][2] = "false";
          konservo(cekplace);
          Serial.println(String ("*") + cekplace + (",") + ("others,") + ("false#"));
          delay(100);
        }
      }
    }
  }
  delay(2000);
  access = false;
}

void getdata() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      //      Serial.println(command);
      parsjson(command);
      command = "";
    }
    else {
      command += c;
    }
  }
}
void parsjson (String payload) {
  //  Serial.println(payload);
  if (payload.length() > 20) {
    const size_t capacity = JSON_ARRAY_SIZE(3) + 3 * JSON_OBJECT_SIZE(3) + 170;
    DynamicJsonBuffer jsonBuffer(capacity);

    //    const char* json = "[{\"booking_place\":\"Book A3\",\"user\":\"adam\",\"book_status\":true},{\"booking_place\":\"Book A1\",\"user\":\"DICKY\",\"book_status\":true},{\"booking_place\":\"Book A2\",\"user\":\"adam\",\"book_status\":false},{\"booking_place\":\"\",\"user\":\"adam\",\"book_status\":true}]";

    //      const char* json = payload;
    JsonArray& root = jsonBuffer.parseArray(payload);
    for (int i = 0; i < 3; i++) {
      JsonObject& root_0 = root[i];
      const char* root_0_booking_place = root_0["booking_place"];
      const char* root_0_user = root_0["user"];
      const char* root_0_book_status = root_0["book_status"];
      String a = root_0_user;
      cekbook[i][0] = root_0_booking_place;
      cekbook[i][1] = root_0_user;
      cekbook[i][2] = root_0_book_status;
      //      Serial.println(cekbook[i][0]);
      //      Serial.println(cekbook[i][1]);
      //      Serial.println(cekbook[i][2]);
      //      Serial.println("");
    }

  }
}

void konservo (String a) {
  //  Serial.print("ini a: ");
  //  Serial.println(a);
  if (a == "Book A1") {
    if (parkir1 == false) {
      //serial.println("Servo Buka");
      servoParkir1.write(90);
      while (1) {
        readSensor();
        if (distance1 <= 5) {
          break;
        }
        delay(100);
      }
      delay(1000);
      servoParkir1.write(0);
      parkir1 = true;
      delay(100);
    }
    else {
      servoParkir1.write(90);
      delay(5000);
      servoParkir1.write(0);
      //serial.println("Servo tuutp");
      parkir1 = false;
      delay(100);
    }
  }


  else if (a == "Book A2") {
    if (parkir2 == false) {
      servoParkir2.write(90);
      while (1) {
        readSensor();
        if (distance2 <= 5) {
          break;
        }
        delay(100);
      }
      delay(1000);
      servoParkir2.write(0);
      parkir2 = true;
      delay(100);
    }
    else {
      servoParkir2.write(90);
      delay(5000);
      servoParkir2.write(0);
      //serial.println("Servo tuutp");
      parkir2 = false;
      delay(100);
    }
  }


  else if (a == "Book A3") {
    if (parkir3 == false) {
      //serial.println("Servo Buka");
      servoParkir3.write(90);
      delay(5000);
      servoParkir3.write(0);
      parkir3 = true;
      delay(100);
    }
    else {
      servoParkir3.write(90);
      delay(5000);
      servoParkir3.write(0);
      //serial.println("Servo tuutp");
      parkir3 = false;
      delay(100);
    }
  }
}

void readSensor() {
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = duration1 * 0.034 / 2;
  
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = duration2 * 0.034 / 2;
}
