#include <Arduino.h>
#include <Encoder.h>

// Pines para encoders (cámbialos si deseas)
#define ENC_A_LEFT   12
#define ENC_B_LEFT   9
#define ENC_A_RIGHT  22
#define ENC_B_RIGHT  13

Encoder e_left(ENC_A_LEFT, ENC_B_LEFT);
Encoder e_right(ENC_A_RIGHT, ENC_B_RIGHT);

long prev_l = -999;
long prev_r = -999;
unsigned long timer = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Encoder Test - Raspberry Pi Pico");
}

void loop() {
  long lpos = e_left.read();
  long rpos = e_right.read();

  if (millis() - timer >= 20) {  // cada 20 ms
    timer = millis();

    if (lpos != prev_l || rpos != prev_r) {
      Serial.print(lpos);
      Serial.print(",");
      Serial.println(rpos);
      prev_l = lpos;
      prev_r = rpos;
    }
  }
}
