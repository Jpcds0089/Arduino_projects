// ------------------------------------------------------------------------------------------------------------
// Defines/Variables
// ------------------------------------------------------------------------------------------------------------


#define led_r 2
#define led_g 3
#define led_b 4
#define btn 7


// ------------------------------------------------------------------------------------------------------------
// Classes
// ------------------------------------------------------------------------------------------------------------


class Button {
  public:
    int key_down, key_up, pino;
    Button(int p) {
      pino = p;
      key_down = key_up = 0;
    }
    bool press() {
      if (digitalRead(pino) == HIGH) {
        key_down = 1;
        key_up = 0;
      } else {
        key_up = 1;
      }
      if ((key_down == 1) and (key_up == 1)) {
        key_down = 0;
        key_up = 0;
        return true;
      } else {
        return false;
      }
    }
};

class Color {
  public:
    int pinoR, pinoG, pinoB, ciclo, maximo;
    Color(int pr, int pg, int pb): pinoR(pr), pinoG(pg), pinoB(pb) {
      ciclo = 0;
      maximo = 3;
    }
    void changeLed() {
      if (ciclo == 0) {
        corLed(1, 0, 0);
      } else if (ciclo == 1) {
        corLed(0, 1, 0);
      } else if (ciclo == 2) {
        corLed(0, 0, 1);
      }
      ciclo++;
      if (ciclo > maximo - 1) {
        ciclo = 0;
      }
    }
  private:
    void corLed(int r, int g, int b) {
      digitalWrite(pinoR, r);
      digitalWrite(pinoG, g);
      digitalWrite(pinoB, b);
    }
};
Button button(btn);
Color color(led_r, led_g, led_b);


// ------------------------------------------------------------------------------------------------------------
// Source
// ------------------------------------------------------------------------------------------------------------


void setup() {
  digitalWrite(led_r, OUTPUT);
  digitalWrite(led_g, OUTPUT);
  digitalWrite(led_b, OUTPUT);
  digitalWrite(btn, INPUT);
}

void loop() {
  if (button.press()) {
    color.changeLed();
  }
}
