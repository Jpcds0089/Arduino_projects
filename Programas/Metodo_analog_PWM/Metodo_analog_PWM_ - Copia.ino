#define led_vermelho 5
#define led_amarelo 3
#define led_azul 6
#define maximo 255
#define minimo 0
#define temp 5

void setup() {
  pinMode(led_vermelho , OUTPUT);
  pinMode(led_amarelo, OUTPUT);
  pinMode(led_azul, OUTPUT);
}
void aumentar_brilho(int led1) {
  for (int i = minimo; i < maximo; i++) {
    analogWrite(led1, i);
    delay(temp);
  }
  for (int i = maximo; i >= minimo; i--) {
    analogWrite(led1, i);
    delay(temp);
  }
}
void loop() {
  aumentar_brilho(led_vermelho);
  aumentar_brilho(led_amarelo);
  aumentar_brilho(led_azul);
}
