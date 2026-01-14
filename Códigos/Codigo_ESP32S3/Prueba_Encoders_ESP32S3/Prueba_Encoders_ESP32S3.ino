/************************************************************************
 *  BASIC ENCODER TEST - ESP32-S3
 ************************************************************************/

#include <Encoder.h>

/* ================= CONFIGURACIÓN ================= */
// Usa GPIO reales del ESP32-S3
#define ENC_A_RIGHT  6  //6
#define ENC_B_RIGHT  5 //16
  #define ENC_A_LEFT   16 //16
  #define ENC_B_LEFT   8  //5


/* ================= ENCODERS ================= */
Encoder e_left(ENC_A_LEFT, ENC_B_LEFT);
Encoder e_right(ENC_A_RIGHT, ENC_B_RIGHT);

/* ================= VARIABLES ================= */
long prevlpos = -999;
long prevrpos = -999;
unsigned long timer = 0;

/* ================= SETUP ================= */
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Basic Encoder Test - ESP32-S3");
}

/* ================= LOOP ================= */
void loop() {
  long lpos = e_left.read();
  long rpos = e_right.read();

  if (millis() - timer >= 20) {
    timer = millis();

    if (lpos != prevlpos || rpos != prevrpos) {
      Serial.print(lpos);
      Serial.print(",");
      Serial.println(rpos);

      prevlpos = lpos;
      prevrpos = rpos;
    }
  }
}
