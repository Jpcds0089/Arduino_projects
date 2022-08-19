int led_vermelho = 2, led_amarelo = 3, led_azul = 4;

void setup() {
  Serial.begin(9600);
  pinMode(led_vermelho, OUTPUT);
  pinMode(led_amarelo, OUTPUT);
  pinMode(led_azul, OUTPUT);
}

void loop() {
  while (Serial.available()) {
    char situacao = Serial.read();
    Serial.println("OIIIII");
    // Caso o led esteja desligado: liga-lo
    if (situacao == 'a') {
      digitalWrite(led_vermelho, HIGH);
    }
    if (situacao == 'b') {
      digitalWrite(led_amarelo, HIGH);
    }
    if (situacao == 'c') {
      digitalWrite(led_azul, HIGH);
    }

    // Caso o led ja esteja ligado: desliga-lo
    if (situacao == 'A') {
      digitalWrite(led_vermelho, LOW);
    }
    if (situacao == 'B') {
      digitalWrite(led_amarelo, LOW);
    }
    if (situacao == 'C') {
      digitalWrite(led_azul, LOW);
    }

    // Desligando todos os leds
    if (situacao == 'd') {
      digitalWrite(led_vermelho, HIGH);
      digitalWrite(led_amarelo, HIGH);
      digitalWrite(led_azul, HIGH);
    }
    if (situacao == 'D') {
      digitalWrite(led_vermelho, LOW);
      digitalWrite(led_amarelo, LOW);
      digitalWrite(led_azul, LOW);
    }
  }
}
