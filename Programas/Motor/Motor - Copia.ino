// ------------------------------------------------------------------------------------------------------------
// Defines/Variables
// ------------------------------------------------------------------------------------------------------------

#include <Servo.h>
#define an5 A5
Servo s1;
int ang_s1;

// ------------------------------------------------------------------------------------------------------------
// Functions
// ------------------------------------------------------------------------------------------------------------



// ------------------------------------------------------------------------------------------------------------
// Source
// ------------------------------------------------------------------------------------------------------------

void setup() {
  s1.attach(8);
  ang_s1 = 0; 
  s1.write(ang_s1);
}

void loop() {
  ang_s1 = 0; 
  //ang_s1 = map(analogRead(an5), 0, 1023, 0, 180);
  s1.write(ang_s1);
  delay(2000);

  ang_s1 = 90; 
  s1.write(ang_s1);
  delay(2000);
}
