// ------------------------------------------------------------------------------------------------------------
// Defines/Variables
// ------------------------------------------------------------------------------------------------------------

#include <Stepper.h>
#define en1 8
#define en2 9
#define en3 10
#define en4 11
#define led 5
#define led2 7
#define passos_por_giro 64
Stepper mp(passos_por_giro, en1, en3, en2, en4);


// ------------------------------------------------------------------------------------------------------------
// Functions
// ------------------------------------------------------------------------------------------------------------


void motor(int vel, int voltas, int loop, int tmp){
  mp.setSpeed(vel);
  for (int i = 0; i < 32 * loop; i++) {
    mp.step(passos_por_giro * voltas);
  }
  delay(tmp);
}


// ------------------------------------------------------------------------------------------------------------
// Source
// ------------------------------------------------------------------------------------------------------------


void setup() {
  pinMode(led, OUTPUT);
}

void loop() {
  digitalWrite(led, LOW);
  motor(500, 1, 1, 0);
  digitalWrite(led, HIGH);
  delay(3000);

  digitalWrite(led, LOW);
  motor(500, -1, 1, 0);
  digitalWrite(led, HIGH);
  delay(3000);
}
