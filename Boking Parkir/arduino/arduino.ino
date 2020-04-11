#include <SPI.h>
#include <ArduinoJson.h>
#include <RFID.h>
#define SS_PIN 10
#define RST_PIN 9
#include <Servo.h>
Servo servoParkir1, servoParkir2, servoParkir3;

RFID rfid(SS_PIN, RST_PIN);
int serNum[5];          //Variable buffer Scan Card
int cards[][5] = {      //ID Kartu yang diperbolehkan masuk
  {235, 235, 232, 33, 201},
  {57, 177, 249, 162, 211}
};
const char* a[2][3];
const char* cekbook[3][3];
int cekBook[][2] = { { 1, 1 }, { 1, 3 }, { 1, 2 } };
int cekUser;
String command;
//Servo servo[3] =  { servoParkir1, servoParkir2, servoParkir3 };
String user[] = {{"dism"}, {"tommy"}, {"hadyan"}};
bool  access = false;
bool statusparkirtomi = false;
bool statusparkirdism = false;
bool statusparkirhadyan = false;
bool parkir1 = false;
bool parkir2 = false;
bool parkir3 = false;

void setup() {
  Serial.begin(9600);
  servoParkir1.attach(5);
  servoParkir2.attach(6);
  servoParkir3.attach(7);
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
      //serial.print(rfid.serNum[0]);
      //serial.print(" ");
      //serial.print(rfid.serNum[1]);
      //serial.print(" ");
      //serial.print(rfid.serNum[2]);
      //serial.print(" ");
      //serial.print(rfid.serNum[3]);
      //serial.print(" ");
      //serial.print(rfid.serNum[4]);
      //serial.println("");

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
    //    Serial.println(cekuserbook);
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

    if (user[cekUser] != "tommy") {
      if (cekuserbook != "tommy" && cekuserbook != "others" || cekuserbook != "tommy" && cekuserbook != "others") {
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
    const size_t capacity = JSON_ARRAY_SIZE(3) + 3*JSON_OBJECT_SIZE(3) + 170;
    DynamicJsonBuffer jsonBuffer(capacity);

    //  const char* json = "[{\"booking_place\":\"Book A3\",\"user\":\"adam\",\"book_status\":true},{\"booking_place\":\"Book A1\",\"user\":\"DICKY\",\"book_status\":true},{\"booking_place\":\"Book A2\",\"user\":\"adam\",\"book_status\":false},{\"booking_place\":\"\",\"user\":\"adam\",\"book_status\":true}]";

    //  const char* json = payload;
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
      delay(2000);
      servoParkir1.write(0);
      parkir1 = true;
      delay(100);
    }
    else {
      servoParkir1.write(90);
      delay(2000);
      servoParkir1.write(0);
      //serial.println("Servo tuutp");
      parkir1 = false;
      delay(100);
    }
  }


  else if (a == "Book A2") {
    if (parkir2 == false) {
      servoParkir2.write(90);
      delay(2000);
      servoParkir2.write(0);
      parkir2 = true;
      delay(100);
    }
    else {
      servoParkir2.write(90);
      delay(2000);
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
      delay(2000);
      servoParkir3.write(0);
      parkir3 = true;
      delay(100);
    }
    else {
      servoParkir3.write(90);
      delay(2000);
      servoParkir3.write(0);
      //serial.println("Servo tuutp");
      parkir3 = false;
      delay(100);
    }
  }
}
