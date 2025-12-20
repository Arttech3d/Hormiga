#include <ESP32Servo.h>

/************************************************************************
 * CONFIGURACIÓN DE LOS MOTORES
 ************************************************************************/
#define WHEEL_R_PIN 1     // GPIO1
#define WHEEL_L_PIN 41    // GPIO41

#define WHEEL_R_MID 1590
#define WHEEL_L_MID 1590

// Velocidades
int VELDR = 100;
int VELIZ = 220;

Servo wheel_right;
Servo wheel_left;

/************************************************************************
 * SETUP
 ************************************************************************/
void setup() {
  Serial.begin(115200);

  // Configuración PWM para ESP32
  wheel_right.setPeriodHertz(50); // 50 Hz típico de servo/ESC
  wheel_left.setPeriodHertz(50);

  wheel_right.attach(WHEEL_R_PIN, 1000, 2000);
  wheel_left.attach(WHEEL_L_PIN, 1000, 2000);

  // Motores detenidos
  wheel_right.writeMicroseconds(WHEEL_R_MID);
  wheel_left.writeMicroseconds(WHEEL_L_MID);

  Serial.println("=======================================");
  Serial.println(" Robot ESP32-S3 listo");
  Serial.println("=======================================");
}

/************************************************************************
 * LOOP
 ************************************************************************/
void loop() {
  // Avanzar
  Serial.println("Avanzando 5 segundos...");
  wheel_right.writeMicroseconds(WHEEL_R_MID - VELDR);
  wheel_left.writeMicroseconds(WHEEL_L_MID + VELIZ);
  delay(5000);

  // Detener
  wheel_right.writeMicroseconds(WHEEL_R_MID);
  wheel_left.writeMicroseconds(WHEEL_L_MID);
  delay(2000);

  // Retroceder
  Serial.println("Retrocediendo 5 segundos...");
  wheel_right.writeMicroseconds(WHEEL_R_MID + VELDR);
  wheel_left.writeMicroseconds(WHEEL_L_MID - VELIZ);
  delay(5000);

  // Detener
  wheel_right.writeMicroseconds(WHEEL_R_MID);
  wheel_left.writeMicroseconds(WHEEL_L_MID);
  delay(2000);
}
