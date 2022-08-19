

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
unsigned long int tempoAnterior = 0;
unsigned long int beforceTime = 0;

/* Bluetooth e humidade */
#include <SoftwareSerial.h>
SoftwareSerial BTSerial(2, 4);
#define espera_loop 1000
#define tempo_bomba_ligada 60
#define tempo_definir_estado 5 //600
int humidade;
int pause = 0;
int count1 = 0;
int count2 = 0;
int count3 = 0;
int count5 = 0;
int minutos = 0;
int segundos  = 0;
int status_da_bomba = 0;
int status_da_conexao = 0;
int estado_atual_solo = 0;
int parar_Logica_da_bomba = 0;
int count4 = tempo_bomba_ligada;
unsigned long int tempo_anterior = millis();
unsigned long int tempo_anterior1 = millis();
unsigned long int tempo_anterior2 = millis();
int tempo_pausar_logica_bomba = tempo_definir_estado * 2;

/* Ir remote */
#include <IRremote.h>
#define receptor 12
#define led0 11
IRrecv rec(receptor);
decode_results resultado;
#define key0 0xFFFF9867
#define key1 0xFFFFA25D
#define key2 0x629D
#define key3 0xFFFFE21D
#define key4 0x22DD
#define key5 0x2FD
#define key6 0xFFFFC23D
#define key7 0xFFFFA663
#define key8 0xFFFFA857
#define key9 0xFFFFA2A7
#define keyOk 0x38C7
#define keyUp 0x18E7
#define keyDown 0x4AB5
#define keyRight 0x5AA5
#define keyLeft 0x10EF
#define keyAsterisk 0xFF
#define keyHash 0xFFFFB04F
int ir_output;


// ------------------------------------------------------------------------------------------------------------
// Source
// ------------------------------------------------------------------------------------------------------------


void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  rec.enableIRIn();
  pinMode(led0, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(rele1, OUTPUT);
  pinMode(rele2, OUTPUT);
  pinMode(rele3, OUTPUT);
  digitalWrite(rele1, HIGH);
  digitalWrite(rele2, HIGH);
}

void loop() {
  // Caso receba algum sinal infra vermelho
  if (rec.decode(&resultado)) {
    digitalWrite(led0, 1);

    ir_output = resultado.value;
    Switch(ir_output);

    rec.resume();
    delay(40);
    digitalWrite(led0, 0);
  }

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

  // Humidade do solo
  if (millis() - tempo_anterior >= espera_loop) {
    int i;
    segundos ++;
    humidade = map(analogRead(A0), 250, 500, 0, 100);

    if (segundos >= 60) {
      segundos = 0;
      minutos ++;
    } if (minutos >= 60) {
      minutos = 0;
    } if (humidade >= 100) {
      humidade = 100;
    } else if (humidade <= 0) {
      humidade = 0;
    }

    Logica_da_bomba(humidade);

    tempo_anterior = millis();
  }

  // Comunicação com o aplicativo
  if (millis() - tempo_anterior1 >= espera_loop) {
    BTSerial.print("*HX");
    BTSerial.print(segundos);
    BTSerial.print("Y");
    BTSerial.print(humidade);
    BTSerial.println("%");

    tempo_anterior1 = millis();
  }

  // Leds
  if (estado_atual_solo == 0) {
    for (int i = 0; i >= 255; i++) {
      analogWrite(led1, i);
      analogWrite(led2, i);
      analogWrite(led3, i);
      delay(30);
    }
    for (int i = 255; i >= 0; i--) {
      analogWrite(led1, i);
      analogWrite(led2, i);
      analogWrite(led3, i);
      if (millis() - tempo_anterior2 >= 300) {
        tempo_anterior2 = millis();
      }
    }
  } else {
    Serial.print("Fazendo leitura do solo --> ");
  } if (estado_atual_solo == 1) {
      Serial.print("Seco(humidade: ");
      Serial.print(humidade);
      Serial.println("%)");

      for (int i = 0; i >= 255; i++) {
        analogWrite(led1, i);
        Serial.println("Acendeu led1");
        if (millis() - tempo_anterior2 >= 300) {
          tempo_anterior2 = millis();
        }
      }
      for (int i = 255; i <= 0; i--) {
        analogWrite(led1, i);
        Serial.println("Apagou led1");
        if (millis() - tempo_anterior2 >= 300) {
          tempo_anterior2 = millis();
        }
      }
    }
    else if (estado_atual_solo == 2) {
    Serial.print("Húmido(humidade: ");
    Serial.print(humidade);
    Serial.println("%)");

    for (int i = 0; i >= 255; i++) {
      analogWrite(led2, i);
      Serial.println("Acendeu led2");
      if (millis() - tempo_anterior2 >= 300) {
        tempo_anterior2 = millis();
      }
    }
    for (int i = 255; i <= 0; i--) {
      analogWrite(led2, i);
      Serial.println("Apagou led2");
      if (millis() - tempo_anterior2 >= 300) {
        tempo_anterior2 = millis();
      }
    }
  } else if (estado_atual_solo == 3) {
    Serial.print("Extremamente húmido(humidade: ");
    Serial.print(humidade);
    Serial.println("%)");

    for (int i = 0; i >= 255; i++) {
      analogWrite(led3, i);
      Serial.println("Acendeu led3");
      if (millis() - tempo_anterior2 >= 300) {
        tempo_anterior2 = millis();
      }
    }
    for (int i = 255; i <= 0; i--) {
      analogWrite(led3, i);
      Serial.println("Apagou led3");
      if (millis() - tempo_anterior2 >= 300) {
        tempo_anterior2 = millis();
      }
    }
  }
}


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
    Serial.println("Não foi possível conectar-se ao pc");
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

/* Importants */
void Switch(char result) {
  switch (result) {
    case key1:
      OnOffLed(led1);
      break;
    case key2:
      OnOffLed(led2);
      break;
    case key3:
      OnOffLed(led3);
      break;
    case key0:
      OnOffRele(rele1);
      break;
    case keyAsterisk:
      OnOffPc(rele2, '*');
      break;
    case keyHash:
      OnOffRele(rele3);
      break;
    case '1':
      OnOffLed(led1);
      break;
    case '2':
      OnOffLed(led2);
      break;
    case '3':
      OnOffLed(led3);
      break;
    case 'B':
      digitalWrite(rele1, 0);
      break;
    case 'b':
      digitalWrite(rele1, 1);
      break;
    case '*':
      OnOffPc(rele2, '*');
      break;
    case '#':
      OnOffRele(rele3);
      break;
  }
}

void Logica_da_bomba(int humidade) {
  // Sintax = Serial.print("Humidade do solo: "); // > 500.0 Extremamente seca / < 250.0 Extremamente Molhada / 350.0 Meio termo

  if (parar_Logica_da_bomba == 0) {
    if (pause == 0) {
      if (humidade >= 70) {
        count1 ++;
        estado_atual_solo = 1;
        if (count1 >= tempo_definir_estado) {
          Serial.println("");
          Serial.println("");
          Serial.println("Ligando a bomba");
          digitalWrite(rele1, 0);
          Serial.println("Bomba ligada!");
          Serial.println("Irrigando!");
          Serial.println("");
          estado_atual_solo = 0;
          count1 = 0;
          pause = 1;
        }
      } else {
        count1 = 0;
      }
      if ((humidade >= 40) and (humidade <= 69)) {
        count2 ++;
        estado_atual_solo = 2;
        if (count2 >= tempo_definir_estado) {
          parar_Logica_da_bomba = 1;
        }
      } else {
        count2 = 0;
      } if (humidade <= 39) {
        count3 ++;
        estado_atual_solo = 3;
        if (count3 >= tempo_definir_estado) {
          parar_Logica_da_bomba = 1;
        }
      } else {
        count3 = 0;
      }
    } else {
      count4 --;
      estado_atual_solo = 0;
      Serial.print("Desligando a bomba em: ");
      Serial.print(count4);
      Serial.println("s");
      if (count4 <= 0) {
        digitalWrite(rele1, 1);
        Serial.println("Bomba desligada!");
        Serial.println("Irrigação finalizada!");
        Serial.println("");
        pause = 0;
        OffLed(led1);
        OffLed(led2);
        OffLed(led3);
        parar_Logica_da_bomba = 1;
        count4 = tempo_bomba_ligada;
      }
    }
  } else {
    estado_atual_solo = 0;
    tempo_pausar_logica_bomba --;
    Serial.println("Esperando para retornar a leitura do solo");
  } if (tempo_pausar_logica_bomba <= 0) {
    parar_Logica_da_bomba = 0;
    tempo_pausar_logica_bomba = tempo_definir_estado * 2;
    Serial.println("");
    Serial.println("Retornando leitura do solo");
    Serial.println("");
  }
}
