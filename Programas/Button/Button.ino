// ------------------------------------------------------------------------------------------------------------
// Defines/Variables
// ------------------------------------------------------------------------------------------------------------


# define button 7
# define led_azul 2
int key_up = 0;
int key_down = 0;
int alterar_modo_dos_leds = 0;
unsigned long tempoDeEspera = 5000;
unsigned long tempoPrimeiroAssionamento = 0;


// ------------------------------------------------------------------------------------------------------------
// Functions
// ------------------------------------------------------------------------------------------------------------


void AlterarModoDosLeds(int led) {
  if (alterar_modo_dos_leds == 1) {
    if (digitalRead(led) == HIGH) {
      digitalWrite(led, LOW);
    }else {
      digitalWrite(led, HIGH);
    }
    alterar_modo_dos_leds = 0;
  }
}

void ReadKeys() {
  if (digitalRead(button) == HIGH) {
    key_up = 0;
    tempoPrimeiroAssionamento = millis();
    Serial.println(tempoPrimeiroAssionamento);
     if (millis() -  tempoPrimeiroAssionamento > tempoDeEspera) {
      key_down = 1;
    }
  }else {
    key_up = 1;
    //tempoPrimeiroAssionamento = 0;  
    //Serial.println(tempoPrimeiroAssionamento);
    //delay(500);
  }
  if (key_down == 1 and key_up == 1) {
    alterar_modo_dos_leds = 1;
    Serial.println("Execultando");
    key_up = 0; 
    key_down = 0;
  }
}


// ------------------------------------------------------------------------------------------------------------
// Source
// ------------------------------------------------------------------------------------------------------------


void setup() {
  pinMode(led_azul, OUTPUT);
  pinMode(button, INPUT);
  Serial.begin(9600);
}

void loop() {
  ReadKeys();
  AlterarModoDosLeds(led_azul);
}
