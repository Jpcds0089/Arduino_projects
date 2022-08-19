// ------------------------------------------------------------------------------------------------------------
// Defines/Variables
// ------------------------------------------------------------------------------------------------------------


#define maximo 6
#define button 7
#define led_test 2
#define delay_1 50
int key_up = 0;
int key_down = 0;
int alterar_modo_dos_leds = 0;
int leds [] = {2, 3, 4, 5, 6, 7};


// ------------------------------------------------------------------------------------------------------------
// Functions
// ------------------------------------------------------------------------------------------------------------


void ligar_leds(int maxx) {
  // Ligar leds
  for (int i = 0; i < maxx; i++) {
    digitalWrite(leds[i], HIGH);
    delay(delay_1);
  }
}
void desligar_leds(int maxx) {
  // Desligar leds
  for (int i = 0; i < maxx; i++) {
    digitalWrite(leds[i], LOW);
    delay(delay_1);
  }
}
void ligar_leds_reverso(int maxx) {
  // Ligar leds
  for (int i = maxx; i < maxx + 1 and i >= 0 ; i--) {
    digitalWrite(leds[i], HIGH);
    delay(delay_1);
  }
}
void desligar_leds_reverso(int maxx) {
  // Desligar leds
  for (int i = maxx; i < maxx + 1 and i >= 0; i--) {
    digitalWrite(leds[i], LOW);
    delay(delay_1);
  }
}
void Button() {
  if (digitalRead(button) == HIGH) {
    digitalWrite(led_test, HIGH);
    key_up = 0;
    key_down = 1;
  } else {
    digitalWrite(led_test, LOW);
    key_up = 1;
  }
  if (key_down == 1 and key_up == 1) {
    if (alterar_modo_dos_leds == 0) {
      alterar_modo_dos_leds = 1;
    } else if (alterar_modo_dos_leds == 1) {
      alterar_modo_dos_leds = 2;
    } else if (alterar_modo_dos_leds == 2) {
      alterar_modo_dos_leds = 3;
    } else {
      alterar_modo_dos_leds = 0;
    }
    key_up = 0;
    key_down = 0;
  }
}


// ------------------------------------------------------------------------------------------------------------
// Source
// ------------------------------------------------------------------------------------------------------------


void setup() {
  // Definindo leds
  for (int i = 0; i < maximo; i++) {
    pinMode(leds[i], OUTPUT);
  }
  pinMode(led_test, OUTPUT);
  pinMode(button, INPUT);
}
void loop() {
  Button();
  if (alterar_modo_dos_leds == 0) {
    ligar_leds(maximo);
    desligar_leds(maximo);
    //ligar_leds_reverso(maximo);
    //desligar_leds_reverso(maximo);
  }
  if (alterar_modo_dos_leds == 1) {
    ligar_leds(maximo);
    desligar_leds(maximo);
  }
  if (alterar_modo_dos_leds == 2) {
    ligar_leds(maximo);
    desligar_leds(maximo);
  }
  if (alterar_modo_dos_leds == 3) {
    ligar_leds(maximo);
    desligar_leds(maximo);
    ligar_leds(maximo);
    desligar_leds(maximo);
  }
}
