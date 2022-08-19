// ------------------------------------------------------------------------------------------------------------
// Defines/Variables
// ------------------------------------------------------------------------------------------------------------


// Ir remote
#include <IRremote.h>
#define receptor 12
#define led0 11
IRrecv rec(receptor);
decode_results resultado;
int rec_val;
// Microfone
#define som 7
// Rele
#define rele1 8


// ------------------------------------------------------------------------------------------------------------
// Functions
// ------------------------------------------------------------------------------------------------------------

void OnRele(int pin) {
  digitalWrite(pin, HIGH);
}

void OffRele(int pin) {
  digitalWrite(pin, LOW);
}

void OnOffRele(int pin) {
  if (digitalRead(pin) == LOW) {
    digitalWrite(pin, HIGH);
  } else {
    digitalWrite(pin, LOW);
  }
}


// ------------------------------------------------------------------------------------------------------------
// Source
// ------------------------------------------------------------------------------------------------------------


void setup() {
  pinMode(rele1, OUTPUT);
  pinMode(led0, OUTPUT);
  pinMode(som, INPUT);
  Serial.begin(9600);
  rec.enableIRIn();
  digitalWrite(rele1, LOW);
}

void loop() {
  // Microfone
  if (digitalRead(som) == HIGH) {
    OnOffRele(rele1);
  }
  
  // Infra vermelho
  if (rec.decode(&resultado)) {
    digitalWrite(led0, HIGH);
    rec_val = resultado.value;
    //Serial.println(rec_val, HEX);
    switch (rec_val) {
      case 0x38C7:
        OnOffRele(rele1);
        break;
    }
    rec.resume();
    delay(30);
    digitalWrite(led0, LOW);
  }
}
