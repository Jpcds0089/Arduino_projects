// ------------------------------------------------------------------------------------------------------------
// Defines/Variables
// ------------------------------------------------------------------------------------------------------------

# define an5 A5

// ------------------------------------------------------------------------------------------------------------
// Functions
// ------------------------------------------------------------------------------------------------------------

void MyConstrain(int val, int val_min, int val_max) {
  if ((val >= val_min) and (val <= val_max)) {
    Serial.println(val);
  }else if(val < val_min){
    Serial.println(val_min);
  }else if(val > val_max){
    Serial.println(val_max);
  }
}

// ------------------------------------------------------------------------------------------------------------
// Source
// ------------------------------------------------------------------------------------------------------------

void setup() {
  pinMode(an5, INPUT);
  Serial.begin(9600);
}

void loop() {
  //constrain(val_inicial, val_min_quero_saber, val_max_quero_saber)
  //Serial.println(constrain(analogRead(an5), 500, 800));
  MyConstrain(analogRead(an5), 500, 800);
  delay(250);
}
