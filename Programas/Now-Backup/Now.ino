

// ------------------------------------------------------------------------------------------------------------
// Defines/Variables
// ------------------------------------------------------------------------------------------------------------


/* Reles */
#define rele1 8
#define rele2 9
#define rele3 10

/* Leds */
#define led1 3
#define led2 5
#define led3 6
#define led0 11
unsigned long int tempoAnterior = 0;
unsigned long int beforceTime = 0;

/* Bluetooth e humidade */
#include <SoftwareSerial.h>
SoftwareSerial BTSerial(2, 4);
#define tempo_r 10 //60
#define tempo_l 10 //1800
#define espera_loop 1000
#define sensor_humidade 7
#define tempo_definir_humidade_media 5 //60
int estagio;
int humidade;
int count1 = 0;
int count2 = 0;
int count3 = 0;
int minutos = 0;
int segundos  = 0;
int parar_logica = 0;
int humidade_atual = 0;
int humidade_media = 0;
int status_do_rele = 0;
int tempo_rele_ligado = tempo_r;
int tempo_retornar_logica = tempo_l;
unsigned long int tempo_anterior = millis();
unsigned long int tempo_anterior1 = millis();
unsigned long int tempo_anterior2 = millis();


// ------------------------------------------------------------------------------------------------------------
// Functions
// ------------------------------------------------------------------------------------------------------------


/* Led */
void OnLed(int pin, int show = 0) {
  digitalWrite(pin, HIGH);
  if (show == 1) {
    Serial.print("Led(pin=");
    Serial.print(pin);
    Serial.println(")ligado.");
    Serial.println("");
  }
}
void OffLed(int pin, int show = 0) {
  digitalWrite(pin, LOW);
  if (show == 1) {
    Serial.print("Led(pin=");
    Serial.print(pin);
    Serial.println(")desligado.");
    Serial.println("");
  }
}
void OnOffLed(int pin, int show = 0) {
  digitalWrite(pin, !digitalRead(pin));
  if (digitalRead(pin) == 1) {
    if (show == 1) {
      Serial.print("Led(pin=");
      Serial.print(pin);
      Serial.println(")ligado.");
      Serial.println("");
    }
  } else {
    if (show == 1) {
      Serial.print("Led(pin=");
      Serial.print(pin);
      Serial.println(")desligado.");
      Serial.println("");
    }
  }
}

void AnalogLed(int pin1 = 0, int pin2 = 0, int pin3 = 0, int maxx = 255, int delayy = 50, int show = 0) {
  for (int i = -1; i < maxx;) {
    if (millis() - tempo_anterior2 > delayy) {
      i++;
      if (pin1 != 0) {
        analogWrite(pin1, i);
      }if (pin2 != 0) {
        analogWrite(pin2, i);
      } if (pin3 != 0) {
        analogWrite(pin3, i);
      } 
      tempo_anterior2 = millis();
    }
  }
  for (int i = maxx - 1; i > 0;) {
    if (millis() - tempo_anterior2 > delayy) {
      i--;
      if (pin1 != 0) {
        analogWrite(pin1, i);
      }if (pin2 != 0) {
        analogWrite(pin2, i);
      } if (pin3 != 0) {
        analogWrite(pin3, i);
      }
      tempo_anterior2 = millis();
    }
  }
}

/* Pc */
void OnOffPc(int pin, char key) {
  Serial.println("Tentando conectar-se ao pc");
  Serial.write(key);
  bool on = LOW;
  while (HIGH) {
    if (Serial.available()) {
      char situation = Serial.read();
      if (situation == key) {
        on = HIGH;
        Serial.println("Conectado.");
        Serial.println("Desligando.");
        Serial.println("");
        break;
      }
    } break;
  }

  if (on == LOW) {
    Serial.println("N??o foi poss??vel conectar-se ao pc");
    Serial.println("Desligando manualmente.");
    Serial.println("");
    digitalWrite(pin, 1);
    delay(10);
    digitalWrite(pin, 0);
    delay(100);
    digitalWrite(pin, 1);
  }
}

/* Others */
void OnOffRele(int pin) {
  digitalWrite(pin, !digitalRead(pin));
}

void toSend(char situation) {
  Serial.write(situation);
  Serial.println(situation);
}


// ------------------------------------------------------------------------------------------------------------
// Source
// ------------------------------------------------------------------------------------------------------------


void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  pinMode(led0, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(rele1, OUTPUT);
  pinMode(rele2, OUTPUT);
  pinMode(rele3, OUTPUT);
  digitalWrite(rele1, HIGH);
  digitalWrite(rele2, HIGH);
  pinMode(sensor_humidade, OUTPUT);
  digitalWrite(sensor_humidade, HIGH);
}

void loop() {

  // Caso receba algo do Serial
  if (Serial.available()) {
    digitalWrite(led0, 1);

    char serial_output = Serial.read();
    Serial.print("Recebido do Serial: ");
    Serial.println(serial_output);
    Switch(serial_output);

    delay(40);
    digitalWrite(led0, 0);
  }

  // Caso receba algo do Serial do Bluetooth
  if (BTSerial.available()) {
    char bt_output = BTSerial.read();
    digitalWrite(led0, 1);
    Serial.write(bt_output);
    Serial.println("");

    Switch(bt_output);

    delay(40);
    digitalWrite(led0, 0);
  }

  // Humidade
  Logica_do_rele();
  
  // Comunica????o com o aplicativo
  if (millis() - tempo_anterior1 >= espera_loop) {
    // Mandando status da conex??o
    if (parar_logica == -2) {
      BTSerial.write("*YR255G0B0*");  
    } else {
      BTSerial.write("*YR0G255B0*");  
    }
    
    // Mandando status do rele
    if (digitalRead(rele1) == 0) {
      BTSerial.write("*RR0G255B0*");
    } else if (digitalRead(rele1) == 1) {
      BTSerial.write("*RR255G0B0*");
    }

    //Mandando humidade do solo
    BTSerial.write("*HX");
    BTSerial.write(segundos);
    BTSerial.write("Y");
    BTSerial.write(humidade);
    BTSerial.write("%");

    // Mandando est??gio atual da iriga????o


    tempo_anterior1 = millis();
  }
}


// ------------------------------------------------------------------------------------------------------------
// Important Functions
// ------------------------------------------------------------------------------------------------------------


/* Importants */
void Switch(char result) {
  switch (result) {
    case '1':
      OnOffLed(led1);
      break;
    case '2':
      OnOffLed(led2);
      break;
    case '3':
      OnOffLed(led3);
      break;
    case '*':
      OnOffPc(rele2, '*');
      break;
    case '#':
      OnOffRele(rele3);
      break;
    case 'R':
      digitalWrite(sensor_humidade, 0);
      digitalWrite(rele1, 0);
      if (parar_logica == -2) {
        parar_logica == -2;
      } else {
        parar_logica = -1;
      }
      break;
    case 'r':
      tempo_retornar_logica = tempo_l;
      digitalWrite(rele1, 1);
      if (parar_logica == -2) {
        parar_logica == -2;
      } else {
        parar_logica = 1;
      }
      break;
    case 'I':
      digitalWrite(sensor_humidade, 1);
      digitalWrite(rele1, 1);
      parar_logica = 0;
      break;
    case 'i':
      digitalWrite(sensor_humidade, 0);
      digitalWrite(rele1, 1);
      parar_logica = -2;
      break;
  }
}

void Logica_do_rele() {
  if (millis() - tempo_anterior >= espera_loop) {
    if (parar_logica == 0) {
      // Definindo est??gio: 1 fazendo leitura do solo
      estagio = 1;

      // Leitura da humidade
      int i;
      segundos ++;
      humidade = map(analogRead(A0), 0, 600, 0, 100); /*map(analogRead(A0), 250, 500, 0, 100);*/

      // Contando segundos e minutos
      if (segundos >= 60) {
        segundos = 0;
        minutos ++;
      } if (minutos >= 60) {
        minutos = 0;
      } 

      // Algumas regras da numera????o da humidade
      if (humidade >= 100) {
        humidade = 100;
      } else if (humidade <= 0) {
        humidade = 0;
      }

      // Definindo humidade atual e calculando humidade m??dia
      // Sensor 1: Extremamente h??mido(10, 39) | H??mido(40 - 74) | Seco(75 - 94)
      // Sensor 2: Extremamente h??mido(70, 100) | H??mido(30 - 69) | Seco(0 - 29)
      if ((humidade >= 0) and (humidade <= 29)) {
        humidade_atual = 1;
        count1 ++;
      } else {
        count1 = 0;
      } if ((humidade >= 30) and (humidade <= 69)) {
        humidade_atual = 2;
        count2 ++;
      } else {
        count2 = 0;
      } if ((humidade >= 70) and (humidade <= 100)) {
        humidade_atual = 3;
        count3 ++;
      } else {
        count3 = 0;
      }

      Serial.print("Humidade atual: ");
      Serial.print(humidade_atual);
      Serial.print("(");
      Serial.print(humidade);
      Serial.println("%)");

      // Definindo humidade m??dia
      if (count1 >= tempo_definir_humidade_media) {
        humidade_media = 1;
        digitalWrite(sensor_humidade, 0);
      } else if (count2 >= tempo_definir_humidade_media) {
        humidade_media = 2;
        digitalWrite(sensor_humidade, 0);
      } else if (count3 >= tempo_definir_humidade_media) {
        humidade_media = 3;
        digitalWrite(sensor_humidade, 0);
      }

      // Definindo o que fazer de acordo com a humidade atual
      // Seco(led1) | H??mido(led2) | Extremamente h??mido(led3)
      if (humidade_media  == 0) {
        if (humidade_atual == 1) {
          AnalogLed(led1, 0, 0, 155, 4);
          analogWrite(led1, 155 );
        } else if (humidade_atual == 2) {
          AnalogLed(led2, 0, 0, 150, 4);
        } else if (humidade_atual == 3) {
          AnalogLed(led3, 0, 0, 150, 4);
        }
      } else {
        OffLed(led1);
        OffLed(led2);
        OffLed(led3);
      }

      Serial.print("Humidade m??dia: ");
      Serial.println(humidade_media);

      // Definindo o que fazer de acordo com a humidade m??dia
      // humidade_media==1 Seco | humidade_media==2 H??mido | humidade_media==3 Extremamente h??mido
      if (humidade_media == 1) {
        tempo_retornar_logica = tempo_l;
        digitalWrite(rele1, 0);
        parar_logica = 2;
        OnLed(led1);
      } if (humidade_media == 2) {
        parar_logica = 1;
        OnLed(led2);
      } if (humidade_media == 3) {
        parar_logica = 1;
        OnLed(led3);
      }
    } else if (parar_logica == 1) {
      // Definindo est??gio: 2 Esperando para fazer a pr??xima leitura do solo
      estagio = 2;
      
      Serial.print("Pr??xima leitura em: ");
      Serial.print(tempo_retornar_logica);
      Serial.println("s");
      tempo_retornar_logica --;
      OffLed(led1);
      OffLed(led2);
      OffLed(led3);

      if (tempo_retornar_logica < 1) {
        estagio = 0;
        digitalWrite(sensor_humidade, 1);
        tempo_retornar_logica = tempo_l;
        humidade_atual = 0;
        humidade_media = 0;
        parar_logica = 0;
        OffLed(led2);
        OffLed(led3);
        count1 = 0;
        count2 = 0;
        count3 = 0;
      }
    } else if (parar_logica == 2) {
      if (tempo_rele_ligado < 1) {
        Serial.print("Pr??xima leitura em: ");
        Serial.print(tempo_retornar_logica);
        Serial.println("s");
        tempo_retornar_logica --;
        digitalWrite(rele1, 1);
        OffLed(led1);
        OffLed(led2);
        OffLed(led3);
  
        if (tempo_retornar_logica < 1) {
          digitalWrite(sensor_humidade, 1);
          tempo_retornar_logica = tempo_l;
          tempo_rele_ligado = tempo_r;
          humidade_atual = 0;
          humidade_media = 0;
          parar_logica = 0;
          count1 = 0;
          count2 = 0;
          count3 = 0;
        }
      } else {
        // Definindo est??gio: 3 Irrigando
        estagio = 3;

        tempo_rele_ligado --;
        Serial.print("Desligando a bomba em: ");
        Serial.print(tempo_rele_ligado);
        Serial.println("s");
      }
    }
    tempo_anterior = millis();
  }
}
