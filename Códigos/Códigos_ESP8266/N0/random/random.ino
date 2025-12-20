#include <Servo.h>

/************************************************************************
 * CONFIGURACIÓN DEL MULTIPLEXOR Y SENSORES
 ************************************************************************/
#define MUX_A_PIN 15
#define MUX_B_PIN 13
#define MUX_C_PIN 12
#define MUX_EN_PIN 2
#define SENSOR_COUNT 8

const int real_pos[8] = {0,1,2,7,3,6,4,5};
float sensor_readings[SENSOR_COUNT];

#define PROXIMITY_THRESH 40  // Umbral de detección

/************************************************************************
 * CONFIGURACIÓN DE LOS MOTORES
 ************************************************************************/
#define WHEEL_R_PIN 16
#define WHEEL_L_PIN 0
#define WHEEL_R_MID 1590
#define WHEEL_L_MID 1590

// Velocidades base
int VELDR = 220;  // Velocidad motor derecho
int VELIZ = 100;  // Velocidad motor izquierdo

Servo wheel_right, wheel_left;

/************************************************************************
 * FUNCIONES DEL MULTIPLEXOR Y SENSORES
 ************************************************************************/
void selectInput(int input) {
  digitalWrite(MUX_A_PIN, input & 0x01);
  digitalWrite(MUX_B_PIN, (input >> 1) & 0x01);
  digitalWrite(MUX_C_PIN, (input >> 2) & 0x01);
}

float readSensor() {
  delayMicroseconds(500);
  float env = (float)analogRead(A0);
  analogWrite(MUX_EN_PIN, 1023);
  delayMicroseconds(100);
  float sensor_reading = (float)analogRead(A0) - env;
  analogWrite(MUX_EN_PIN, 0);
  if (sensor_reading < 0) sensor_reading = 1;
  return sensor_reading;
}

void readSensors() {
  Serial.println("\nLeyendo sensores...");
  for (int i = 0; i < SENSOR_COUNT; i++) {
    selectInput(i);
    sensor_readings[real_pos[i]] = readSensor();
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(sensor_readings[real_pos[i]]);
  }
}

/************************************************************************
 * CONTROL DE MOVIMIENTO AUTÓNOMO
 ************************************************************************/
void updateVelocities() {
  bool obst_front   = sensor_readings[0] >= PROXIMITY_THRESH;
  bool obst_fleft   = sensor_readings[1] >= PROXIMITY_THRESH;
  bool obst_left    = sensor_readings[2] >= PROXIMITY_THRESH;
  bool obst_bleft   = sensor_readings[3] >= PROXIMITY_THRESH;
  bool obst_back    = sensor_readings[4] >= PROXIMITY_THRESH;
  bool obst_bright  = sensor_readings[5] >= PROXIMITY_THRESH;
  bool obst_right   = sensor_readings[6] >= PROXIMITY_THRESH;
  bool obst_fright  = sensor_readings[7] >= PROXIMITY_THRESH;

  Serial.println("\n--- Análisis de obstáculos ---");

  // Obstáculo al frente o en esquinas frontales
  if (obst_front || obst_fright || obst_fleft) {
    Serial.println("Obstáculo al frente. Evaluando giro...");
    float sum_left  = sensor_readings[1] + sensor_readings[2] + sensor_readings[3];
    float sum_right = sensor_readings[7] + sensor_readings[6] + sensor_readings[5];

    if (sum_left < sum_right) {
      Serial.println("Girando a la izquierda...");
      wheel_right.writeMicroseconds(WHEEL_R_MID + VELDR);
      wheel_left.writeMicroseconds(WHEEL_L_MID + VELIZ);
    } else {
      Serial.println("Girando a la derecha...");
      wheel_right.writeMicroseconds(WHEEL_R_MID - VELDR);
      wheel_left.writeMicroseconds(WHEEL_L_MID - VELIZ);
    }
  }

  // Obstáculo en esquina o lado izquierdo
  else if (obst_left || obst_bleft) {
    Serial.println("Obstáculo en esquina o lado izquierdo. Girando más fuerte a la derecha...");
    int velR = VELDR + 80;  // Aumenta velocidad solo en este caso
    int velL = VELIZ + 80;
    wheel_right.writeMicroseconds(WHEEL_R_MID - velR);
    wheel_left.writeMicroseconds(WHEEL_L_MID - velL);
  }

  // Obstáculo en esquina o lado derecho
  else if (obst_right || obst_bright) {
    Serial.println("Obstáculo en esquina o lado derecho. Girando más fuerte a la izquierda...");
    int velR = VELDR + 80;  // Aumenta velocidad solo en este caso
    int velL = VELIZ + 80;
    wheel_right.writeMicroseconds(WHEEL_R_MID + velR);
    wheel_left.writeMicroseconds(WHEEL_L_MID + velL);
  }

  // Obstáculo trasero
  else if (obst_back) {
    Serial.println("Obstáculo detrás. Avanzando hacia adelante...");
    wheel_right.writeMicroseconds(WHEEL_R_MID - VELDR - 50);
    wheel_left.writeMicroseconds(WHEEL_L_MID + VELIZ + 50);
  }

  // Sin obstáculos
  else {
    Serial.println("Sin obstáculos detectados. Avanzando hacia adelante...");
    wheel_right.writeMicroseconds(WHEEL_R_MID - VELDR);
    wheel_left.writeMicroseconds(WHEEL_L_MID + VELIZ);
  }
}

/************************************************************************
 * SETUP
 ************************************************************************/
void setup() {
  pinMode(MUX_A_PIN, OUTPUT);
  pinMode(MUX_B_PIN, OUTPUT);
  pinMode(MUX_C_PIN, OUTPUT);
  pinMode(MUX_EN_PIN, OUTPUT);
  analogWrite(MUX_EN_PIN, 0);

  wheel_right.attach(WHEEL_R_PIN);
  wheel_left.attach(WHEEL_L_PIN);
  wheel_right.writeMicroseconds(WHEEL_R_MID);
  wheel_left.writeMicroseconds(WHEEL_L_MID);

  Serial.begin(115200);
  Serial.println("=======================================");
  Serial.println("   Robot autónomo iniciado correctamente");
  Serial.println("   Escaneando entorno y ajustando ruta...");
  Serial.println("=======================================");
  Serial.println("Usa VELDR y VELIZ para ajustar la velocidad individual de los motores.\n");
}

/************************************************************************
 * LOOP PRINCIPAL
 ************************************************************************/
void loop() {
  Serial.println("\n--------------------------------------");
  Serial.println("Lectura de sensores y control de movimiento");
  Serial.println("--------------------------------------");
  readSensors();
  updateVelocities();
  delay(300);
}
