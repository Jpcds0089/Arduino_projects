// ------------------------------------------------------------------------------------------------------------
// Defines/Variables
// ------------------------------------------------------------------------------------------------------------


#include <IRremote.h>
#define receptor 12
#define rele1 8
#define led0 11
IRrecv rec(receptor);
decode_results resultado;
int rec_val;


// ------------------------------------------------------------------------------------------------------------
// Functions
// ------------------------------------------------------------------------------------------------------------


void OnOffRele(int rele) {
  if (digitalRead(rele) == LOW) {
    digitalWrite(rele, HIGH);
  } else {
    digitalWrite(rele, LOW);
  }
}


// ------------------------------------------------------------------------------------------------------------
// Source
// ------------------------------------------------------------------------------------------------------------


void setup() {
  pinMode(rele1, OUTPUT);
  Serial.begin(9600);
  rec.enableIRIn();
}

void loop() {
  // Caso receba algum sinal infra vermelho
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
