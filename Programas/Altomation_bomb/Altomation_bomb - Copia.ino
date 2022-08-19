// ------------------------------------------------------------------------------------------------------------ \\
// Global Defines and Variables
// ------------------------------------------------------------------------------------------------------------ \\


#define led1 2
#define led2 3
#define led3 4
#define rele1 8
#define sensor A0
#define on_off_sensor 7
#define humidity_dry 150
#define humidity_wet 450
#define humidity_very_wet 750
#define humidity_waint_time 1000

int value;
int time1;
int time2;
int time3;
bool bomb = LOW;
int segundo = 1000;
int minuto = 60000;
int delay_turn_on_bomb = (minuto * 4);


// ------------------------------------------------------------------------------------------------------------ \\
// Functions
// ------------------------------------------------------------------------------------------------------------ \\


void Led(int r, int g, int b) {
  if (r == 1) {
    digitalWrite(led1, 1);
  } else {
    digitalWrite(led1, 0);
  }
  if (g == 1) {
    digitalWrite(led2, 1);
  } else {
    digitalWrite(led2, 0);
  }
  if (b == 1) {
    digitalWrite(led3, 1);
  } else {
    digitalWrite(led3, 0);
  }
}


/* Notes
  Seco: 150-30
  Pontinha na agua: 480
  1/3 na agua: 580
  2/3 na agua: 680
  3/3 na agua: 740*/


// ------------------------------------------------------------------------------------------------------------ \\
// Source
// ------------------------------------------------------------------------------------------------------------ \\


void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(rele1, OUTPUT);
  pinMode(sensor, INPUT);
  pinMode(on_off_sensor, OUTPUT);
  Serial.begin(9600);
  Led(1, 1, 1);
  digitalWrite(rele1, HIGH);
  digitalWrite(on_off_sensor, HIGH);
}

void loop() {
  value = analogRead(sensor);
  Serial.println(value);

  // Definindo estado do terreno
  if (humidity_dry >= value) {
    time1 ++;
  } else {
    time1 = 0;
  }
  if ((value >= humidity_dry) and (value <= humidity_wet)) {
    time2 ++;
  } else {
    time2 = 0;
  }
  if ((value >= humidity_wet) and (value <= humidity_very_wet)) {
    time3 ++;
  } else {
    time3  = 0;
  }

  // Definindo o que fazer dependendo do estado(seco, molhado, encharcado) do terreno
  if (time1 >= 59) {
    time1 = 0;
    Led(1, 0, 0);
    digitalWrite(rele1, LOW);
    digitalWrite(on_off_sensor, LOW);
    Serial.println("Dispositivo ligado.");
    if (bomb == LOW) {
      bomb = HIGH;
      Serial.print("Esperan a bomba ligada por: ");
      Serial.println(delay_turn_on_bomb);
      delay(delay_turn_on_bomb);
    } else {
      Serial.print("Esperan a bomba ligada por: ");
      Serial.println(delay_turn_on_bomb);
      delay(delay_turn_on_bomb / 1.5);
      delay(delay_turn_on_bomb / 1.5);
      digitalWrite(on_off_sensor, HIGH);
    }
  } else if (time2 >= 59) {
    bomb = LOW;
    Led(0, 1, 0);
    digitalWrite(rele1, HIGH);
    Serial.println("Dispositivo desligado.");
  } else if (time3 >= 59) {
    bomb = LOW;
    Led(0, 0, 1);
    digitalWrite(rele1, HIGH);
    Serial.println("Dispositivo desligado.");
  }

  if ((time1 <= 59) and (time2 <= 59) and (time3 <= 59)) {
    Led(1, 1, 1);
  }
  delay(segundo);
}
