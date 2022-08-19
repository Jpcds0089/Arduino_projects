// ------------------------------------------------------------------------------------------------------------
// Defines/Variables
// ------------------------------------------------------------------------------------------------------------


#include <IRremote.h>
#define led1 2
#define led2 3
#define led3 4

// Receber sinal
#define receptor 12
#define led0 11
IRrecv rec(receptor);
decode_results resultado;
int rec_val;

// Mandar sinal
/*
IRsend irsend;
unsigned long send_val;
decode_results res;
decode_type_t tipo;
*/


// ------------------------------------------------------------------------------------------------------------
// Functions
// ------------------------------------------------------------------------------------------------------------


void OnLed(int pin) {
  digitalWrite(pin, HIGH);
}

void OffLed(int pin) {
  digitalWrite(pin, LOW);
}

void OnOffLed(int pin) {
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
  pinMode(led0, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  Serial.begin(9600);
  rec.enableIRIn();
}

void loop() {
  // Enviar sinal infra vermelho
  /*Formatos de evento
   * NEC - 32bits
   * RC5 - 12bits
   * RC6 - 20bits
   * SONY - 20bits
   * UNKNOWN
   
  if(digitalRead(bt1) == 1) {
    send_val = 0x18E7;
    irsend.sendRC5(send_val, 12);
    Serial.println(send_val, HEX);
    tipo = res.decode_type;
    Serial.println(tipo);
    delay(500);
  } else if(digitalRead(bt2) == 1) {
    send_val = 0x4AB5;
    irsend.sendRC5(send_val, 12);
    Serial.println(send_val, HEX);
    tipo = res.decode_type;
    Serial.println(tipo);
    delay(500);
  }
  */

  // Caso receba algum sinal infra vermelho
  if (rec.decode(&resultado)) {
    digitalWrite(led0, HIGH);
    rec_val = resultado.value;
    Serial.println(rec_val, HEX);
    switch (rec_val) {
      case 0xFFFFA25D:
        OnOffLed(led1);
        break;
    }
    rec.resume();
    delay(20);
    digitalWrite(led0, LOW);
  }
}
