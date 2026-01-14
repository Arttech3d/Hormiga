#include <Servo.h>

#define WHEEL_R_PIN  14   // Motor derecho
#define WHEEL_L_PIN  11   // Motor izquierdo

#define WHEEL_R_MID 1590
#define WHEEL_L_MID 1590

int VELDR = 220;  // Velocidad motor derecho
int VELIZ = 100;  // Velocidad motor izquierdo

Servo wheel_right, wheel_left;

void setup() {
  Serial.begin(115200);

  wheel_right.attach(WHEEL_R_PIN);
  wheel_left.attach(WHEEL_L_PIN);

  // Motores en reposo
  wheel_right.writeMicroseconds(WHEEL_R_MID);
  wheel_left.writeMicroseconds(WHEEL_L_MID);

  Serial.println("Motores listos.");
}

void loop() {

  // --------- AVANZAR 5s ---------
  Serial.println("Avanzando 5s...");
  wheel_right.writeMicroseconds(WHEEL_R_MID - VELDR);
  wheel_left.writeMicroseconds(WHEEL_L_MID + VELIZ);
  delay(5000);

  // Pausar brevemente (opcional)
  wheel_right.writeMicroseconds(WHEEL_R_MID);
  wheel_left.writeMicroseconds(WHEEL_L_MID);
  delay(300);

  // --------- RETROCEDER 5s ---------
  Serial.println("Retrocediendo 5s...");
  wheel_right.writeMicroseconds(WHEEL_R_MID + VELDR);
  wheel_left.writeMicroseconds(WHEEL_L_MID - VELIZ);
  delay(5000);

  // Pausar brevemente (opcional)
  wheel_right.writeMicroseconds(WHEEL_R_MID);
  wheel_left.writeMicroseconds(WHEEL_L_MID);
  delay(300);
}
