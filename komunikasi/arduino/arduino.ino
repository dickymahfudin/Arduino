#define Pled1 2
#define Pled2 13
String dataIn;
String dt[10];
int i;

int led1, led2;
boolean parsing = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(Pled1, OUTPUT);
  pinMode(Pled2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  getdata();
  delay(100);
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
  //  Serial.println(dt[0]);
  //  Serial.println(dt[1]);
  led1 = dt[2].toInt();
  led2 = dt[1].toInt();

  if (led1 == 0) {
    digitalWrite(Pled1, LOW);
  }
  if (led1 == 1) {
    digitalWrite(Pled1, HIGH);
  }

  if (led2 == 0) {
    digitalWrite(Pled2, LOW);
  }
  if (led2 == 1) {
    digitalWrite(Pled2, HIGH);
  }

}
