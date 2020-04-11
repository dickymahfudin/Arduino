#define BLYNK_PRINT Serial
#include <PZEM004Tv30.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

PZEM004Tv30 pzem(12, 14);  // D6, D5, (RX,TX) connect to TX,RX of PZEM
LiquidCrystal_I2C lcd(0x3f, 20, 4);

char auth[] = "9xSpQPwhPoiHQl2TqDefj3XOWrjZ2YtQ";
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Dism";
char pass[] = "RoUmAhKITA";
char server[] = "119.18.158.238";
int port = 3579;

int relay = D0;
float voltage, current, power, energy, frequency, pf;
float pajak;
float saldo;
int onlineMode;
int temp = 0;
int start;

float kwh900 = 1467;
float kwh400 = 1114;
int minimun_saldo = 100;
int minimumNotif = 200;

float perkwh = 0;
float kwh, wh, harga, duit_jumlah;
float watthasil;

int duration = 10;
int loopDuration;
bool habis = false;
BLYNK_CONNECTED() {
  Blynk.syncVirtual(V1, V2, V5);
}
WidgetLCD tampil(V0);
BLYNK_WRITE(V1)
{
  int a = param.asInt();
  saldo = a;

  Serial.println(a);
}
BLYNK_WRITE(V2)
{
  onlineMode = param.asInt();
  Serial.println(start);
}
BLYNK_WRITE(V5)
{
  lcd.clear();
  int data = param.asInt();
  if (data == 1) perkwh = kwh900;
  else if (data == 0) perkwh = kwh400;
  Serial.println(perkwh);
  lcd.setCursor(0, 0);
  lcd.print("        MODE        ");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("  PERKWH = " + String(perkwh));
  delay(5000);
  lcd.clear();
}

void setup() {
  Serial.begin(115200);
  lcd.init();
  pinMode(relay, OUTPUT);
  lcd.backlight();
  Blynk.begin(auth, ssid, pass, server, port);
  lcd.setCursor(0, 0);
  lcd.print(" Connecting to WIFI");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    lcd.setCursor(0, 0);
    lcd.print(" Connecting to WIFI");
  }
  delay(2000);

  tampil.print(0, 0, "     MONITOR    ");
  tampil.print(0, 1, "   POWER METER  ");
  lcd.setCursor(0, 0);
  lcd.print("       MONITOR      ");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("    POWER METER     ");
  delay(3000);
  lcd.clear();
}

void loop() {
  Blynk.run();
  sensor();

  if (onlineMode == 1) {
    if (saldo >= minimun_saldo) {
      tampil.clear();
      String a = String(duit_jumlah) + "   ";
      tampil.print(0, 0, "Saldo:Rp.");
      tampil.print(9, 0, saldo);
      tampil.print(0, 1, "Pakai:RP.");
      tampil.print(9, 1, duit_jumlah);
      Blynk.virtualWrite(V1, saldo);
      Blynk.virtualWrite(V10, duit_jumlah);
      saldo = saldo - harga;
      //      digitalWrite(led, HIGH);
      digitalWrite(relay, HIGH);
      habis =  false;
    }

    else {
      digitalWrite(relay, LOW);
      tampil.print(0, 0, "   Saldo Anda   ");
      tampil.print(0, 1, "     Kurang     ");

      if (!habis) {
        Blynk.notify("Saldo Anda Habis Harap Di Isi");
        habis = true;
      }
    }
    loopDuration++;
    notif();
  }
  else {
    String V = "V = " + String(voltage);
    String I = "A = " + String(current);
    tampil.clear();
    tampil.print(0, 0, V);
    tampil.print(0, 1, I);
    digitalWrite(relay, HIGH);
    //    digitalWrite(led, HIGH);
  }
  delay(1000);
  Serial.println();
  Serial.println();
}

void sensor() {
  voltage = pzem.voltage();
  if ( !isnan(voltage) ) {
    Serial.print("Voltage: "); Serial.print(voltage); Serial.println("V");
  } else {
    voltage = 0;
    Serial.println("Error reading voltage");
  }

  current = pzem.current();
  if ( !isnan(current) ) {
    Serial.print("Current: "); Serial.print(current); Serial.println("A");
  } else {
    current = 0;
    Serial.println("Error reading current");
  }

  power = pzem.power();
  if ( !isnan(power) ) {
    Serial.print("Power: "); Serial.print(power); Serial.println("W");
  } else {
    power = 0;
    Serial.println("Error reading power");
  }

  energy = pzem.energy();
  if ( !isnan(energy) ) {
    Serial.print("Energy: "); Serial.print(energy, 3); Serial.println("kWh");
  } else {
    energy = 0;
    Serial.println("Error reading energy");
  }

  frequency = pzem.frequency();
  if ( !isnan(frequency) ) {
    Serial.print("Frequency: "); Serial.print(frequency, 1); Serial.println("Hz");
  } else {
    frequency = 0;
    Serial.println("Error reading frequency");
  }

  pf = pzem.pf();
  if ( !isnan(pf) ) {
    Serial.print("PF: "); Serial.println(pf);
  } else {
    pf = 0;
    Serial.println("Error reading power factor");
  }

  lcd.setCursor(0, 0);
  lcd.print("    MONITOR DATA    ");
  lcd.setCursor(0, 1);
  lcd.print("I= ");
  lcd.print(current);
  lcd.print("  Ampere");

  lcd.setCursor(0, 2);
  lcd.print("V= ");
  lcd.print(voltage);
  lcd.print("  Volt");

  lcd.setCursor(0, 3);
  lcd.print("P= ");
  lcd.print(power);
  lcd.print("  W");
  hitung();
}

void hitung() {
  kwh = perkwh / 1000;
  wh = kwh / 3600;

  pajak = (power * wh) * 0.03;
  Serial.print("pajak : ");
  Serial.println(pajak, 20);
  harga = (power * wh) + pajak;

  Serial.print("Spajak : ");
  Serial.println(harga, 7);
  duit_jumlah = duit_jumlah += harga;
  watthasil = watthasil + power;
  Serial.print("Rp ");
  Serial.println(duit_jumlah, 7);

  Blynk.virtualWrite(V11, power);
  Blynk.virtualWrite(V3, watthasil);
  Blynk.virtualWrite(V4, power);
}

void notif() {
  Serial.println("Loop " + String(loopDuration));
  if (loopDuration == duration) {
    if (saldo <= minimun_saldo) {
      Blynk.notify("Saldo Anda Habis Harap Di Isi");
    }
    if ((saldo <= minimumNotif) && (saldo >= minimun_saldo)) {
      Blynk.notify("Sisah Saldo Anda RP." + String(saldo));
    }
    loopDuration = 0;
  }
}
