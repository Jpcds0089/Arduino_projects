
// ------------------------------------------------------------------------------------------------------------
// Defines/Variables
// ------------------------------------------------------------------------------------------------------------

/* Serial */
#include <SoftwareSerial.h>
SoftwareSerial BTSerial(10, 11);
long BTBAUD=9600;


/* Reles */
#define rele1 8

/* Leds */
#define led1 3
#define led2 4
#define led3 5
unsigned long int tempoAnterior = 0;
unsigned long int beforceTime = 0;


// ------------------------------------------------------------------------------------------------------------
// Functions
// ------------------------------------------------------------------------------------------------------------


/* Led */
void OnLed(int pin) {
  digitalWrite(pin, HIGH);
  Serial.print("Led(pin=");
  Serial.print(pin);
  Serial.println(")ligado.");
}
void OffLed(int pin) {
  digitalWrite(pin, LOW);
  Serial.print("Led(pin=");
  Serial.print(pin);
  Serial.println(")desligado.");
}
void OnOffLed(int pin) {
  digitalWrite(pin, !digitalRead(pin));
  if (digitalRead(pin) == 1) {
    Serial.print("Led(pin=");
    Serial.print(pin);
    Serial.println(")ligado.");
  } else {
    Serial.print("Led(pin=");
    Serial.print(pin);
    Serial.println(")desligado.");
  }
}
void OnOffLedTime(int pin, int tm) {
  digitalWrite(pin, !digitalRead(pin));
  /*delayMillis(tm);*/
  digitalWrite(pin, !digitalRead(pin));
}


/* Others */
void OnOffRele(int pin) {
  digitalWrite(pin, !digitalRead(pin));
}
void toSend(char situation) {
  Serial.write(situation);
  Serial.println(situation);
}


// ------------------------------------------------------------------------------------------------------------
// Source
// ------------------------------------------------------------------------------------------------------------


void setup() {
  Serial.begin(9600);
  BTSerial.begin(BTBAUD);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(rele1, OUTPUT);
}

void loop() {
  // Caso receba algo do Serial
  if (Serial.available()) {
    char situation = Serial.read();
    switch (situation) {
      case '1':
        OnOffLed(led1);
        break;
      case '2':
        OnOffLed(led2);
        break;
      case '3':
        OnOffLed(led3);
        break;
      case 'a':
        OnOffRele(rele1);
        break;
    }
    delay(40);
  }

  if (BTSerial.available()) {
    char sit = BTSerial.read();
    Serial.println(sit);
    Serial.write(BTSerial.read());
  }
  Serial.print("Humidade do solo: "); // > 400 Extremamente seca / < 200 Extremamente Molhada / 350 Meio termo
  Serial.println(analogRead(A0));
}
