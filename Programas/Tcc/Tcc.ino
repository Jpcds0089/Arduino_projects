// ------------------------------------------------------------------------------------------------------------
// Defines/Variables/Includes
// ------------------------------------------------------------------------------------------------------------

/* Leds */
#define led0 12

/* Reles */
#define rele1 8

/* Bluetooth e humidade */
#include <SoftwareSerial.h>
#define pin_txd 2
#define pin_rxd 4
SoftwareSerial BTSerial(pin_txd, pin_rxd);
#define tempo_r 60
#define tempo_l 1800
#define rele_ligado 0
#define rele_desligado 1
#define espera_loop 1000
#define sensor_humidade 7
#define pin_sensor_humidade A0
#define tempo_definir_humidade_media 60
String command;
String bt_command;
int humidade;
int count1 = 0;
int count2 = 0;
int count3 = 0;
int estagio = 1;
int minutos = 0;
int segundos = 0;
int humidade_atual = 0;
int humidade_media = 0;
int status_do_rele = 0;
int irrigacao_automatica = 0;
int tempo_rele_ligado = tempo_r;
int tempo_retornar_logica = tempo_l;
unsigned long int tempo_anterior = millis();
unsigned long int tempo_anterior1 = millis();


// ------------------------------------------------------------------------------------------------------------
// Functions
// ------------------------------------------------------------------------------------------------------------


void LogicaSwitch(String result) {
  Serial.print("Comando Para Execultar: ");
  Serial.println(result); 
  // "Y" == Irrigação automática ativada
  if (result == "*Y;"){
    if (irrigacao_automatica != 1) {
      irrigacao_automatica = 1;
      digitalWrite(sensor_humidade, 1);
      digitalWrite(rele1, rele_desligado);
      estagio = 1;
      count1 = 0;
      count2 = 0;
      count3 = 0;
    }

  // "y" == Irrigação automática desativada
  } else if (result == "*y;"){
    if (irrigacao_automatica != 0) {
      irrigacao_automatica = 0;
      digitalWrite(sensor_humidade, 0);
      digitalWrite(rele1, rele_desligado);
    }

  // "R" == Habilitar rele
  } else if (result == "*R;"){
    if (estagio != 0) {
      digitalWrite(rele1, rele_ligado);
      digitalWrite(sensor_humidade, 0);
      estagio = 0;
    }

  // "r" == Desabilitar rele
  } else if (result == "*r;"){
    if (estagio != 2) {
      tempo_retornar_logica = tempo_l;
      digitalWrite(rele1, rele_desligado);
      estagio = 2;
    }
  }
}

void LogicaIrrigacao() {
  if ((millis() - tempo_anterior >= espera_loop) and (irrigacao_automatica == 1)) {

    // Estágio 1: fazer leitura do solo
    if (estagio == 1) {
      // Leitura da humidade
      int i;
      segundos++;
      humidade = map(analogRead(pin_sensor_humidade), 0, 600, 0, 100); /*map(analogRead(A0), 250, 500, 0, 100);*/

      // Contando segundos e minutos
      if (segundos >= 60) {
        segundos = 0;
        minutos++;
      }
      if (minutos >= 60) {
        minutos = 0;
      }

      // Algumas regras da numeração da humidade
      if (humidade >= 100) {
        humidade = 100;
      } else if (humidade <= 0) {
        humidade = 0;
      }

      // Definindo humidade atual e calculando humidade média
      // Sensor 1: Extremamente húmido(10, 39) | Húmido(40 - 74) | Seco(75 - 94)
      // Sensor 2: Extremamente húmido(70, 100) | Húmido(30 - 69) | Seco(0 - 29)
      if ((humidade >= 0) and (humidade <= 29)) {
        humidade_atual = 1;
        count1++;
      } else {
        count1 = 0;
      }
      if ((humidade >= 30) and (humidade <= 69)) {
        humidade_atual = 2;
        count2++;
      } else {
        count2 = 0;
      }
      if ((humidade >= 70) and (humidade <= 100)) {
        humidade_atual = 3;
        count3++;
      } else {
        count3 = 0;
      }

      Serial.print("Humidade atual: ");
      Serial.print(humidade_atual);
      Serial.print("(");
      Serial.print(humidade);
      Serial.println("%)");

      // Definindo humidade média
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

      // Definindo o que fazer de acordo com a humidade média
      // humidade_media==1 Seco | humidade_media==2 Húmido | humidade_media==3 Extremamente húmido
      Serial.print("Humidade média: ");
      Serial.println(humidade_media);
      if (humidade_media == 1) {
        tempo_retornar_logica = tempo_l;
        digitalWrite(rele1, rele_ligado);
        estagio = 3;
      } else if ((humidade_media == 2) or (humidade_media == 3)) {
        estagio = 2;
      }

      // Estágio 2: Esperar um tempo determinado para fazer a próxima leitura da humidade do solo
    } else if (estagio == 2) {
      Serial.print("Próxima leitura em: ");
      Serial.print(tempo_retornar_logica);
      Serial.println("s");
      tempo_retornar_logica--;

      if (tempo_retornar_logica < 1) {
        digitalWrite(sensor_humidade, 1);
        tempo_retornar_logica = tempo_l;
        humidade_atual = 0;
        humidade_media = 0;
        estagio = 1;
        count1 = 0;
        count2 = 0;
        count3 = 0;
      }

      // Estágio 3: Irrigando caso o solo esteja seco (humidade média == 1)
    } else if (estagio == 3) {
      tempo_rele_ligado--;
      Serial.print("Desligando a bomba em: ");
      Serial.print(tempo_rele_ligado);
      Serial.println("s");
      if (tempo_rele_ligado < 1) {
        digitalWrite(rele1, rele_desligado);
        estagio = 2;
      }
    }
    tempo_anterior = millis();
  }
}

void LogicaComunicacao() {
  if (millis() - tempo_anterior1 >= espera_loop) {

    // Mandando status do rele
    if (digitalRead(rele1) == 0) {
      BTSerial.write("*RR0G255B0*");
    } else if (digitalRead(rele1) == 1) {
      BTSerial.write("*RR255G0B0*");
    }

    // Mandando humidade do solo
    BTSerial.write("*HX");
    BTSerial.print(segundos);
    BTSerial.write("Y");
    BTSerial.print(humidade);
    BTSerial.write("%*");

    // Mandando status da conexão
    if (irrigacao_automatica == 0) {
      BTSerial.write("*YR255G0B0*");
      BTSerial.write("*T *");
      Serial.println("Irrigaçâo altomática desativada");
    } else {
      BTSerial.write("*YR0G255B0*");
    }

    // Mandando estágio atual da irigaçâo
    if (estagio == 1) {
      if (humidade_atual == 1) {
        BTSerial.write("*THumidade atual: ");
        BTSerial.print(humidade);
        BTSerial.write("% (seco)*");
      } else if (humidade_atual == 2) {
        BTSerial.write("*THumidade atual: ");
        BTSerial.print(humidade);
        BTSerial.write("% (humido)*");
      } else if (humidade_atual == 3) {
        BTSerial.write("*THumidade atual: ");
        BTSerial.print(humidade);
        BTSerial.write("% (encharcado)*");
      }
    } else if (estagio == 2) {
      BTSerial.write("*TRefazendo a leitura em: ");
      BTSerial.print(tempo_retornar_logica);
      BTSerial.write("s*");
    } else if (estagio == 3) {
      BTSerial.write("*TIrrigando*");
    }

    tempo_anterior1 = millis();
  }
}

/* Others */
void OnOffRele(int pin) {
  digitalWrite(pin, !digitalRead(pin));
}


// ------------------------------------------------------------------------------------------------------------
// Source
// ------------------------------------------------------------------------------------------------------------


void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  pinMode(led0, OUTPUT);
  pinMode(rele1, OUTPUT);
  digitalWrite(rele1, rele_desligado);
  pinMode(sensor_humidade, OUTPUT);
  digitalWrite(sensor_humidade, HIGH);
}

void loop() {

  // Caso receba algo do Serial
  if (Serial.available()) {
    digitalWrite(led0, 1);

    char serial_output = Serial.read();
    command += serial_output;
    if (serial_output == ';') {
      Serial.print("Comando Recebido: ");
      Serial.println(command); 

      if (command.substring(0,1) == "*") {
        LogicaSwitch(command);
      }
    }

    delay(40);
    digitalWrite(led0, 0);
  }

  // Caso receba algo do Serial do Bluetooth
  if (BTSerial.available()) {
    digitalWrite(led0, 1);
    char bt_output = BTSerial.read();
    bt_command += bt_output;
    
    if (bt_output == ';') {
      Serial.print("Comando Recebido: ");
      Serial.println(bt_command); 
      
      if (bt_command.substring(0,1) == "*") {
        LogicaSwitch(bt_command);
        bt_command = "";
      } else {
        bt_command = "";
      }
    }

    delay(40);
    digitalWrite(led0, 0);
  }

  // Lógica da irrigação
  LogicaIrrigacao();

  // Lógica comunicação com o aplicativo
  LogicaComunicacao();
}