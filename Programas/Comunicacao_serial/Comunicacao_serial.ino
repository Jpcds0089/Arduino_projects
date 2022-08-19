// ------------------------------------------------------------------------------------------------------------
// Defines/Variables
// ------------------------------------------------------------------------------------------------------------


#define led0 11
int valor_lido;


// ------------------------------------------------------------------------------------------------------------
// Source
// ------------------------------------------------------------------------------------------------------------


void setup() {
  Serial.begin(9600);
  pinMode(led0, OUTPUT);
  Serial.println("Digite 1 para ligar, ou 0 para desligar o led.");
}

void loop() {
  if (Serial.available() > 0) {
    valor_lido = Serial.read();
  }
  if (valor_lido == '1') {
    Serial.println("Led ligado.");
    digitalWrite(led0, 1);
    valor_lido = -1;
  } else if (valor_lido == '0') {
    Serial.println("Led desligado.");
    digitalWrite(led0, 0);
    valor_lido = -1;
  }
}
