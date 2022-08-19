// ------------------------------------------------------------------------------------------------------------
// Defines/Variables
// ------------------------------------------------------------------------------------------------------------
#define led_r 4
#define led_g 3
#define led_b 2
#define botao 7
#define maximo 7
int key_down = 0;
int key_up = 0;
int ciclo = 0;


// ------------------------------------------------------------------------------------------------------------
// Functions
// ------------------------------------------------------------------------------------------------------------


void Vermelho() {
  digitalWrite(led_r, 1);
  digitalWrite(led_g, 0);
  digitalWrite(led_b, 0);
}
void Verde() {
  digitalWrite(led_r, 0);
  digitalWrite(led_g, 1);
  digitalWrite(led_b, 0);
}
void Azul() {
  digitalWrite(led_r, 0);
  digitalWrite(led_g, 0);
  digitalWrite(led_b, 1);
}
void Amarelo(){
  digitalWrite(led_r,1);
  digitalWrite(led_g,1);
  digitalWrite(led_b,0);
}
void Branco(){
  digitalWrite(led_r,1);
  digitalWrite(led_g,1);
  digitalWrite(led_b,1);
}
void Magenta(){
  digitalWrite(led_r,1);
  digitalWrite(led_g,0);
  digitalWrite(led_b,1);
}
void Ciano(){
  digitalWrite(led_r,0);
  digitalWrite(led_g,1);
  digitalWrite(led_b,1);
}
void TrocarLed() {
  if (ciclo == 0) {
    Vermelho();
  }else if (ciclo == 1) {
    Verde();
  }else if (ciclo == 2) {
    Azul();  
  }else if (ciclo == 3) {
    Amarelo();  
  }else if (ciclo == 4) {
    Magenta();  
  }else if (ciclo == 5) {
    Ciano();  
  }else if (ciclo == 6) {
    Branco();      
  }
  ciclo++;
  if(ciclo>maximo-1) {
    ciclo = 0;
  }
}
void Button() {
  if (digitalRead(botao) == HIGH) {
    key_down = 1;
    key_up = 0;
  }else {
    key_up = 1;
  }
  if ((key_down == 1) and (key_up == 1)) {
    key_down = 0;
    key_up = 0;
    TrocarLed();
  }
}
void Python() {
  while (Serial.available()) {
    char situacao = Serial.read();
    if (situacao == 'a') {
      TrocarLed();
    }else if (situacao == 'r') {
      Vermelho();
    }else if (situacao == 'g') {
      Verde();
    }else if (situacao == 'b') {
      Azul();
    }else if (situacao == 'y') {
      Amarelo();
    }else if (situacao == 'm') {
      Magenta();
    }else if (situacao == 'c') {
      Ciano();
    }else if (situacao == 'w') {
      Branco();
    }
  }
}

// ------------------------------------------------------------------------------------------------------------
// Source
// ------------------------------------------------------------------------------------------------------------


void setup() {
  pinMode(led_r, OUTPUT);
  pinMode(led_g, OUTPUT);
  pinMode(led_b, OUTPUT);
  pinMode(botao, INPUT);
  Serial.begin(9600);
}
void loop() {
  //Button();
  Python();
}
