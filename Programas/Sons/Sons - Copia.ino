#define som 10

void setup() {
  pinMode(som, OUTPUT);
}

void loop() {
  //Sintax tone()
  //tone(porta, frequencia)
  //noTone(porta)
  //tone(porta, frequencia, duracao)
  //a duracao vai de 31 - 4978
  
  tone(som, 4978, 250);
  delay(500);
  tone(som, 4978, 250);
  delay(5000);
}
