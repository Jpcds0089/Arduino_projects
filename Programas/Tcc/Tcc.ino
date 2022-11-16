// ------------------------------------------------------------------------------------------------------------
// Defines/Variables/Includes
// ------------------------------------------------------------------------------------------------------------
  
/* Leds */
#define led0 12

/* Reles */
#define rele1 9

/* Serial */
String serial_command_recived;

/* Bluetooth serial */
#include <SoftwareSerial.h>
#define pin_txd 2
#define pin_rxd 3
String bt_command_recived;
SoftwareSerial BTSerial(pin_txd, pin_rxd);

/* Comunication logic */
#define communication_delay 1000  // 500 milisegundos
unsigned long int tempo_anterior1 = millis();

/* Irrigation logic */
#define rele_ligado 0
#define rele_desligado 1
#define sensor_humidade 8
#define pin_sensor_humidade A0
#define tempo_r 30 //2400  // 40 mintos
#define tempo_l 30 //10800  // 3 horas
#define espera_loop 1000  // 1 segundo
#define tempo_definir_humidade_media 30  //300  // 5 minutos
String bd;
bool send_informations = 0;
int logg = 0;
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
int irrigacao_automatica = 1;
int tempo_rele_ligado = tempo_r;
int tempo_retornar_logica = tempo_l;
unsigned long int tempo_anterior = millis();

/* Automatic irrigation manual control */
#define automatic_irrigation_led 5
#define irrigation_led 4
#define automatic_irrigation_button_pin 7
#define irrigation_button_pin 6
int key_up_btn1 = 0;
int key_down_btn1 = 0;
int key_up_btn2 = 0;
int key_down_btn2 = 0;
unsigned long int tempoDeEspera = 100;
unsigned long int tempoPrimeiroAssionamento = 0;
unsigned long int tempo_anterior3 = millis();


// ------------------------------------------------------------------------------------------------------------
// Functions
// ------------------------------------------------------------------------------------------------------------


void LogicaExeculsao(String result) {
  Serial.println(result);
  
  // "Y" == Irrigação automática ativada
  if (result == "*Y;") {
    if (irrigacao_automatica != 1) {
      irrigacao_automatica = 1;
      digitalWrite(sensor_humidade, 1);
      digitalWrite(rele1, rele_desligado);
      estagio = 1;
      count1 = 0;
      count2 = 0;
      count3 = 0;
      humidade_media = 0;
      send_informations = 1;
    }

    // "y" == Irrigação automática desativada
  } else if (result == "*y;") {
    if (irrigacao_automatica != 0) {
      irrigacao_automatica = 0;
      digitalWrite(sensor_humidade, 0);
      digitalWrite(rele1, rele_desligado);
      estagio = 0;
      send_informations = 1;
    }

    // "R" == Habilitar rele
  } else if (result == "*R;") {
    digitalWrite(rele1, rele_ligado);
    digitalWrite(sensor_humidade, 0);
    if (estagio != 0) {
      estagio = 2;
      send_informations = 1;
    }

    // "r" == Desabilitar rele
  } else if (result == "*r;") {
    digitalWrite(rele1, rele_desligado);
    if (estagio != 2) {
      tempo_retornar_logica = tempo_l;
      if (estagio != 0) {
        estagio = 2;
        send_informations = 1;
      }
    }
  }
}

void LogicaIrrigacao() {
  if ((millis() - tempo_anterior >= espera_loop) and (irrigacao_automatica == 1)) {

    // Estágio 1: fazer leitura do solo
    if (estagio == 1) {
      // Log
      Print("Fazendo a leitura do solo");

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

      // Log
      if (logg == 1) {
        Serial.print("Humidade atual: ");
        Serial.print(humidade);
        Serial.println("%");
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

      // Definindo humidade média
      // humidade_media==1 Seco | humidade_media==2 Húmido | humidade_media==3 Extremamente húmido
      if (count1 >= tempo_definir_humidade_media) {
        humidade_media = 1;
        digitalWrite(sensor_humidade, 0);

        // Log
        if (logg == 1) {
          Serial.println("Humidade_media: 1(Seco)");
        }
        
        // Desblockeando a função de comunicação
        send_informations = 1;
        
        // Salvando informações em uma variável
        bd += ("bd = {"); 
        bd += ("hm: 1, t: " + millis()); 

      } else if (count2 >= tempo_definir_humidade_media) {
        humidade_media = 2;
        digitalWrite(sensor_humidade, 0);

        // Log
        if (logg == 1) {
          Serial.println("Humidade_media: 2(Húmido)");
        }        

        // Desblockeando a função de comunicação
        send_informations = 1;
        
        // Salvando informações em uma variável
        bd += ("bd = {"); 
        bd += ("hm: 2, t: " + millis()); 

      } else if (count3 >= tempo_definir_humidade_media) {
        humidade_media = 3;
        digitalWrite(sensor_humidade, 0);

        // Log
        if (logg == 1) {
          Serial.println("Humidade_media: 3(Extremamente Húmido)");
        }        

        // Desblockeando a função de comunicação
        send_informations = 1;
        
        // Salvando informações em uma variável
        bd += ("bd = {"); 
        bd += ("bd = {hm: 3, t: " + millis()); 
      }

      // Definindo o que fazer de acordo com a humidade média
      // humidade_media==1 Seco | humidade_media==2 Húmido | humidade_media==3 Extremamente húmido
      if (humidade_media == 1) {
        tempo_retornar_logica = tempo_l;
        digitalWrite(rele1, rele_ligado);
        estagio = 3;
        
        // Desblockeando a função de comunicação
        send_informations = 1;
        
        // Salvando informações em uma variável
        bd += (", init_i: " + millis()); 
      
      } else if ((humidade_media == 2) or (humidade_media == 3)) {
        estagio = 2;
      
        // Desblockeando a função de comunicação
        send_informations = 1;
        
        // Salvando informações em uma variável
        bd += (", init_s: " + millis()); 
      }

      // Estágio 2: Esperar um tempo determinado para fazer a próxima leitura da humidade do solo
    } else if (estagio == 2) {
      tempo_retornar_logica--;
      
      // Log
      if (logg == 1) {
        Serial.print("Retornando a leitura em: ");
        Serial.print(tempo_retornar_logica);
        Serial.println("s");
      }

      if (tempo_retornar_logica < 1) {
        digitalWrite(sensor_humidade, 1);
        tempo_retornar_logica = tempo_l;
        humidade_atual = 0;
        humidade_media = 0;
        estagio = 1;
        count1 = 0;
        count2 = 0;
        count3 = 0;
        
        // Desblockeando a função de comunicação
        send_informations = 1;
        
        // Salvando informações em uma variável
        bd += (", finish_s: " + millis()); 
        bd += ("}, "); 
      }

      // Estágio 3: Irrigando caso o solo esteja seco (humidade média == 1)
    } else if (estagio == 3) {
      tempo_rele_ligado--;

      // Log
      if (logg == 1) {
        Serial.println("Parando de irrigar em: " + tempo_rele_ligado);
      }

      if (tempo_rele_ligado < 1) {
        digitalWrite(rele1, rele_desligado);
        estagio = 2;
        
        // Desblockeando a função de comunicação
        send_informations = 1;
        
        // Salvando informações em uma variável
        bd += (", finish_i: " + millis()); 
        bd += ("}, "); 
      }
    }
    tempo_anterior = millis();
  }
}

void LogicaComunicacao() {
  if (millis() - tempo_anterior1 >= communication_delay) {
    // Iniciando a string que receberá as informações atuais do programa que serão enviadas via bluetooth
    String command_to_send = "";
    
    // Mandando status da irrigação
    if (irrigacao_automatica == 1) {
      //BTSerial.write("*y1;");
      command_to_send += "*y1;,";
      Print("Irrigacao Automática: True");
    } else {
      //BTSerial.write("*y0;");
      command_to_send += "*y0;,";
    }

    // Mandando status do rele
    if (digitalRead(rele1) == rele_ligado) {
      //BTSerial.write("*r1;");
      command_to_send += "*r1;,";
      Print("Irrigacao: True");
    } else {
      //BTSerial.write("*r0;");
      command_to_send += "*r0;,";
    }
    
    // Mandando estágio atual da irigaçâo
    if (estagio == 1) {
      if (humidade_media == 1) {
        //BTSerial.write("*h1;");
        command_to_send += "*h1;,";
        Print("Humidade Média: Seca");
      } else if (humidade_atual == 2) {
        //BTSerial.write("*h2;");
        command_to_send += "*h2;,";
        Print("Humidade Média: Húmida");
      } else if (humidade_atual == 3) {
        //BTSerial.write("*h3;");
        command_to_send += "*h3;,";
        Print("Humidade Média: Extremamente Húmida");
      } else {
        //BTSerial.write("*h0;");
        command_to_send += "*h0;,";
      }
    } else if (estagio == 2) {
      //BTSerial.write("*s1;");
      command_to_send += "*s1;,";
      Print("Esperando: True");
    } else if (estagio == 3) {
      //BTSerial.write("*i1;");
      command_to_send += "*i1;,";
      Print("Irrigando: True");
    }

    // Mandando a string com as informações via bluetooth
    BTSerial.println(command_to_send);
    
    // Log
    if (logg == 1) {
      Serial.println("Comando enviado:" + command_to_send);    
    }
    
    send_informations = 0;
    tempo_anterior1 = millis();
  }
}

void IrrigationManualControl() {
  // Definindo se o botão da irrigação automática está apertado ou não
  if (digitalRead(automatic_irrigation_button_pin) == LOW) {
    key_up_btn1 = 0;
    key_down_btn1 = 1;
    Serial.println("Apertou");
  } if ((digitalRead(automatic_irrigation_button_pin) == HIGH) and (key_down_btn1 == 1)) {
    key_up_btn1 = 1;
    Serial.println("Soltou");
  }
  
  // Definindo se o botão da irrigação está apertado ou não
  if (digitalRead(irrigation_button_pin) == LOW) {
    key_up_btn2 = 0;
    key_down_btn2 = 1;
    Serial.println("Apertou");
  } if ((digitalRead(irrigation_button_pin) == HIGH) and (key_down_btn2 == 1)) {
    key_up_btn2 = 1;
    Serial.println("Soltou");
  }

  // Execultando ações caso o botão foi apertado
  if (key_down_btn1 == 1 and key_up_btn1 == 1) {
    
    // Habilitando a irrigação automática
    if (irrigacao_automatica == 0) {
      irrigacao_automatica = 1;
      digitalWrite(sensor_humidade, 1);
      digitalWrite(rele1, rele_desligado);
      estagio = 1;
      count1 = 0;
      count2 = 0;
      count3 = 0;
      humidade_media = 0;
      send_informations = 1;
    } 
    
    // Desabilitando a irrigação automática
    else {
      irrigacao_automatica = 0;
      digitalWrite(sensor_humidade, 0);
      digitalWrite(rele1, rele_desligado);
      estagio = 0;
      send_informations = 1;
    }
    key_up_btn1 = 0; 
    key_down_btn1 = 0;

  } if (key_down_btn2 == 1 and key_up_btn2 == 1) {
    Serial.println("Btn da irrigação foi ativado");

    // Habilitando a irrigação
    if (digitalRead(rele1) == rele_desligado) {
      digitalWrite(rele1, rele_ligado);
      digitalWrite(sensor_humidade, 0);
      if (estagio != 0) {
        estagio = 2;
        send_informations = 1;
      }
    }

    // Desabilitando a irrigação
    else {
      digitalWrite(rele1, rele_desligado);
      if (estagio != 2) {
        tempo_retornar_logica = tempo_l;
        if (estagio != 0) {
          estagio = 2;
          send_informations = 1;
        }
      }
    }
    key_up_btn2 = 0; 
    key_down_btn2 = 0;

  }

  // Ligando os leds
  if (irrigacao_automatica == 1) {
    digitalWrite(automatic_irrigation_led, 1);
  } else {
    digitalWrite(automatic_irrigation_led, 0);
  }
  if (digitalRead(rele1) == rele_ligado) {
    digitalWrite(irrigation_led, 1);
  } else {
    digitalWrite(irrigation_led, 0);
  }
}

void OnOffRele(int pin) {
  digitalWrite(pin, !digitalRead(pin));
}

void Print(String text) {
  if (logg == 1) {
    Serial.println(text);
  }
}


// ------------------------------------------------------------------------------------------------------------
// Source
// ------------------------------------------------------------------------------------------------------------


void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  pinMode(led0, OUTPUT);
  pinMode(automatic_irrigation_button_pin, INPUT_PULLUP);
  pinMode(irrigation_button_pin, INPUT_PULLUP);
  pinMode(automatic_irrigation_led, OUTPUT);
  pinMode(irrigation_led, OUTPUT);
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
    serial_command_recived += serial_output;
    if (serial_output == ';') {
      //Serial.print("Comando Recebido: ");
      //Serial.println(serial_command_recived);

      if (serial_command_recived.substring(0, 1) == "*") {
        LogicaExeculsao(serial_command_recived);
      }
    }

    delay(40);
    digitalWrite(led0, 0);
  }

  // Caso receba algo do Serial do Bluetooth
  if (BTSerial.available()) {
    digitalWrite(led0, 1);
    char bt_output = BTSerial.read();
    bt_command_recived += bt_output;

    if (bt_output == ';') {
      Serial.print("Comando Recebido: ");
      Serial.println(bt_command_recived);

      if (bt_command_recived.substring(0, 1) == "*") {
        LogicaExeculsao(bt_command_recived);
        bt_command_recived = "";
      } else {
        bt_command_recived = "";
      }
    }

    delay(40);
    digitalWrite(led0, 0);
  }

  // Lógica da irrigação
  LogicaIrrigacao();

  // Lógica comunicação com o aplicativo
  LogicaComunicacao();

  // Controle manual da irrigação automática e da bomba/válvula solenoid
  IrrigationManualControl();
}