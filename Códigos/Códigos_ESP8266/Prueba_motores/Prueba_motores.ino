#include <Servo.h>

/************************************************************************
 * CONFIGURACIÓN DE LOS MOTORES
 ************************************************************************/
#define WHEEL_R_PIN 16 //D0
#define WHEEL_L_PIN 0  //D3
#define WHEEL_R_MID 1590
#define WHEEL_L_MID 1590

// Velocidades de los motores
int VELDR = 100;  // Velocidad motor derecho
int VELIZ = 220;  // Velocidad motor izquierdo

Servo wheel_right, wheel_left;

/************************************************************************
 * SETUP
 ************************************************************************/
void setup() {
  // Configuración de los pines de los motores
  wheel_right.attach(WHEEL_R_PIN);
  wheel_left.attach(WHEEL_L_PIN);
  
  // Inicia los motores en posición media (detenidos)
  wheel_right.writeMicroseconds(WHEEL_R_MID);
  wheel_left.writeMicroseconds(WHEEL_L_MID);
  
  // Inicializa el puerto serie para monitorear
  Serial.begin(115200);
  Serial.println("=======================================");
  Serial.println("   Robot controlado por motores.");
  Serial.println("=======================================");
}

/************************************************************************
 * LOOP PRINCIPAL
 ************************************************************************/
void loop() {
  // Avanzar por 5 segundos
  Serial.println("Avanzando 5 segundos...");
  wheel_right.writeMicroseconds(WHEEL_R_MID - VELDR);  // Motor derecho avanza
  wheel_left.writeMicroseconds(WHEEL_L_MID + VELIZ);   // Motor izquierdo avanza
  delay(5000); // Espera 5 segundos
  
  // Retroceder por 5 segundos
  Serial.println("Retrocediendo 5 segundos...");
  wheel_right.writeMicroseconds(WHEEL_R_MID + VELDR);  // Motor derecho retrocede
  wheel_left.writeMicroseconds(WHEEL_L_MID - VELIZ);   // Motor izquierdo retrocede
  delay(5000); // Espera 5 segundos
}
