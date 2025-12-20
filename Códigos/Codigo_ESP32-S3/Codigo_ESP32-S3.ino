// ====== CONFIGURACIÓN DE PINES PARA ESP32-S3 ======
#include <Servo.h>

/******** MULTIPLEXOR 74HC4051 ********/
#define MUX_A_PIN 4
#define MUX_B_PIN 5
#define MUX_C_PIN 6
#define MUX_EN_PIN 7

#define SENSOR_COUNT 8
float sensor_readings[SENSOR_COUNT];
const int real_pos[8] = {0, 1, 2, 7, 3, 6, 4, 5};

#define PROXIMITY_THRESH 40
#define SENSOR_ADC_PIN 8    // ADC1_CH7

/******** MOTORES (SERVOS) ********/
#define WHEEL_R_PIN 18
#define WHEEL_L_PIN 17

#define WHEEL_R_MID 1590
#define WHEEL_L_MID 1590

int VELDR = 220;
int VELIZ = 100;

Servo wheel_right, wheel_left;

/******** FUNCIONES DEL MUX ********/
void selectInput(int input) {
  digitalWrite(MUX_A_PIN, input & 0x01);
  digitalWrite(MUX_B_PIN, (input >> 1) & 0x01);
  digitalWrite(MUX_C_PIN, (input >> 2) & 0x01);
}

float readSensor() {
  delayMicroseconds(500);
  float env = analogRead(SENSOR_ADC_PIN);

  digitalWrite(MUX_EN_PIN, HIGH);
  delayMicroseconds(100);
  float sensor_value = analogRead(SENSOR_ADC_PIN) - env;

  digitalWrite(MUX_EN_PIN, LOW);

  if (sensor_value < 0) sensor_value = 1;
  return sensor_value;
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

/******** CONTROL DE MOVIMIENTO ********/
void updateVelocities() {

  bool obst_front  = sensor_readings[0] >= PROXIMITY_THRESH;
  bool obst_fleft  = sensor_readings[1] >= PROXIMITY_THRESH;
  bool obst_left   = sensor_readings[2] >= PROXIMITY_THRESH;
  bool obst_bleft  = sensor_readings[3] >= PROXIMITY_THRESH;
  bool obst_back   = sensor_readings[4] >= PROXIMITY_THRESH;
  bool obst_bright = sensor_readings[5] >= PROXIMITY_THRESH;
  bool obst_right  = sensor_readings[6] >= PROXIMITY_THRESH;
  bool obst_fright = sensor_readings[7] >= PROXIMITY_THRESH;

  Serial.println("\n--- Análisis de obstáculos ---");

  if (obst_front || obst_fright || obst_fleft) {
    
    Serial.println("Obstáculo al frente. Girando...");

    float sum_left = sensor_readings[1] + sensor_readings[2] + sensor_readings[3];
    float sum_right = sensor_readings[7] + sensor_readings[6] + sensor_readings[5];

    if (sum_left < sum_right) {
      Serial.println("Giro a la izquierda.");
      wheel_right.writeMicroseconds(WHEEL_R_MID + VELDR);
      wheel_left.writeMicroseconds(WHEEL_L_MID + VELIZ);

    } else {
      Serial.println("Giro a la derecha.");
      wheel_right.writeMicroseconds(WHEEL_R_MID - VELDR);
      wheel_left.writeMicroseconds(WHEEL_L_MID - VELIZ);
    }

  } else if (obst_left || obst_bleft) {

    Serial.println("Obstáculo a la izquierda. Giro derecha...");
    wheel_right.writeMicroseconds(WHEEL_R_MID - VELDR);
    wheel_left.writeMicroseconds(WHEEL_L_MID - VELIZ);

  } else if (obst_right || obst_bright) {

    Serial.println("Obstáculo a la derecha. Giro izquierda...");
    wheel_right.writeMicroseconds(WHEEL_R_MID + VELDR);
    wheel_left.writeMicroseconds(WHEEL_L_MID + VELIZ);

  } else if (obst_back) {

    Serial.println("Obstáculo atrás. Avanzando...");
    wheel_right.writeMicroseconds(WHEEL_R_MID - VELDR - 50);
    wheel_left.writeMicroseconds(WHEEL_L_MID + VELIZ + 50);

  } else {

    Serial.println("Libre. Avanzando...");
    wheel_right.writeMicroseconds(WHEEL_R_MID - VELDR);
    wheel_left.writeMicroseconds(WHEEL_L_MID + VELIZ);
  }
}

/******** SETUP ********/
void setup() {

  pinMode(MUX_A_PIN, OUTPUT);
  pinMode(MUX_B_PIN, OUTPUT);
  pinMode(MUX_C_PIN, OUTPUT);
  pinMode(MUX_EN_PIN, OUTPUT);

  digitalWrite(MUX_EN_PIN, LOW);

  wheel_right.attach(WHEEL_R_PIN);
  wheel_left.attach(WHEEL_L_PIN);

  wheel_right.writeMicroseconds(WHEEL_R_MID);
  wheel_left.writeMicroseconds(WHEEL_L_MID);

  Serial.begin(115200);

  Serial.println("Robot autónomo con ESP32-S3 iniciado");
}

/******** LOOP ********/
void loop() {
  readSensors();
  updateVelocities();
  delay(500);
}
