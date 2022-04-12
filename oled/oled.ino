#include <Adafruit_SH1106.h>
Adafruit_SH1106 display(21, 22);


void setup()   {
  Serial.begin(115200);
  /* initialize OLED with I2C address 0x3C */
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  //size 1 21 Huruf, size 2 10 huruf

  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0); // `120,56
  display.println("   Nisa   ");
  display.setTextSize(1);
  display.setCursor(0, 28); // `120,56
  display.println("    Eskaibnariyati  ");
  display.setTextSize(2);
  display.setCursor(0, 50); // `120,56
  display.println(" 065118709");
  display.display();
  delay(3000);
  display.clearDisplay();

  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("Antrian Ke");
  display.setTextSize(4);
  display.setCursor(45, 28); // `120,56
  display.println("1");
  display.display();
  delay(3000);
  display.clearDisplay();


  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println(" Lihat No");
  display.setCursor(0, 25);
  display.println("Antrian Di");
  display.setCursor(0, 45);
  display.println("   Layar");
  display.display();
  delay(3000);
  display.clearDisplay();

  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("Kartu Tidk");
  display.setCursor(0, 28);
  display.println(" Terdaftar");
  display.display();
  delay(3000);
  display.clearDisplay();

  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Poli Penyakit Dalam");
  display.setTextSize(2);
  display.setCursor(0, 28);
  display.println(" Tempelkan");
  display.setCursor(0, 45);
  display.println("   EKTP");
  display.display();
  delay(3000);
  display.clearDisplay();

  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0, 28);
  display.println("Loading...");
  display.display();
  delay(3000);
  display.clearDisplay();

}
void loop() {
  /* set text size, color, cursor position,
    set buffer with  Hello world and show off*/
}
