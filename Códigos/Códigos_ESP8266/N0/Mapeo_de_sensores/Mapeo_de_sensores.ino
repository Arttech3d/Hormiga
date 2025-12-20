#include <Servo.h>

/************************************************************************
 * CONFIGURACIÓN DEL MULTIPLEXOR Y SENSORES
 ************************************************************************/
#define MUX_A_PIN 15
#define MUX_B_PIN 13
#define MUX_C_PIN 12
#define MUX_EN_PIN 2
#define SENSOR_COUNT 8

// Orden original de los sensores
// 4,
const int real_pos[8] = {0,1,2,7,3,6,4,5};
float sensor_readings[SENSOR_COUNT];

#define PROXIMITY_THRESH 200
int VEL = 150;

/************************************************************************
 * CONFIGURACIÓN DE LOS MOTORES
 ************************************************************************/
#define WHEEL_R_PIN 16
#define WHEEL_L_PIN 0
#define WHEEL_R_MID 1595
#define WHEEL_L_MID 1585

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
  if(sensor_reading < 0) sensor_reading = 1;
  return sensor_reading;
}

void readSensors() {
  for (int i=0; i<SENSOR_COUNT; i++) {
    selectInput(i);
    sensor_readings[real_pos[i]] = readSensor();
  }
}

/************************************************************************
 * DIAGRAMA DE OCTÁGONO VISUAL
 ************************************************************************/
void printOctagonVisual() {
  bool obst[8];
  for (int i = 0; i < 8; i++) {
    obst[i] = sensor_readings[i] >= PROXIMITY_THRESH;
  }

  Serial.println("       Frente (S0)      ");
  Serial.print("           "); Serial.println(obst[4] ? "●" : "○");
  
  Serial.println("        /     \\        ");
  Serial.print(" S7 "); Serial.print(obst[1] ? "● " : "○ "); 
  Serial.print("       "); 
  Serial.print("S1 "); Serial.println(obst[7] ? "●" : "○");

  Serial.println("       |       |       ");
  Serial.print(" S6 "); Serial.print(obst[2] ? "● " : "○ "); 
  Serial.print("       "); 
  Serial.print("S2 "); Serial.println(obst[6] ? "●" : "○");

  Serial.println("        \\     /        ");
  Serial.print(" S5 "); Serial.print(obst[3] ? "●" : "○"); 
  Serial.print("       "); 
  Serial.print("S3 "); Serial.println(obst[5] ? "●" : "○");

  Serial.println("       Atrás (S4)      ");
  Serial.print("           "); Serial.println(obst[0] ? "●" : "○");

  Serial.println();
  Serial.println("Leyenda: ● = tapado, ○ = libre");
  Serial.println("Izquierda: S2 / Derecha: S6");
  Serial.println();
}

/************************************************************************
 * CONTROL DE MOVIMIENTO AUTÓNOMO
 ************************************************************************/
void updateVelocities() {
  // Interpretación invertida: adelante ↔ atrás, izquierda ↔ derecha
  bool obst_front   = sensor_readings[0] >= PROXIMITY_THRESH;  // antes 0
  bool obst_fleft   = sensor_readings[3] >= PROXIMITY_THRESH;  // antes 1
  bool obst_left    = sensor_readings[2] >= PROXIMITY_THRESH;  // antes 2
  bool obst_bleft   = sensor_readings[1] >= PROXIMITY_THRESH;  // antes 3
  bool obst_back    = sensor_readings[4] >= PROXIMITY_THRESH;  // antes 4
  bool obst_bright  = sensor_readings[7] >= PROXIMITY_THRESH;  // antes 5
  bool obst_right   = sensor_readings[6] >= PROXIMITY_THRESH;  // antes 6
  bool obst_fright  = sensor_readings[5] >= PROXIMITY_THRESH;  // antes 7

  if (obst_front || obst_fright || obst_fleft) {
    float sum_left  = sensor_readings[1] + sensor_readings[2] + sensor_readings[3];
    float sum_right = sensor_readings[7] + sensor_readings[6] + sensor_readings[5];

    if (sum_left < sum_right) {
      wheel_right.writeMicroseconds(WHEEL_R_MID - VEL);
      wheel_left.writeMicroseconds(WHEEL_L_MID - VEL);
    } else {
      wheel_right.writeMicroseconds(WHEEL_R_MID + VEL);
      wheel_left.writeMicroseconds(WHEEL_L_MID + VEL);
    }
  }
  else if (obst_left || obst_bleft) {
    wheel_right.writeMicroseconds(WHEEL_R_MID + VEL);
    wheel_left.writeMicroseconds(WHEEL_L_MID + VEL);
  }
  else if (obst_right || obst_bright) {
    wheel_right.writeMicroseconds(WHEEL_R_MID - VEL);
    wheel_left.writeMicroseconds(WHEEL_L_MID - VEL);
  }
  else if (obst_back) {
    wheel_right.writeMicroseconds(WHEEL_R_MID + VEL + 50);
    wheel_left.writeMicroseconds(WHEEL_L_MID - VEL - 50);
  }
  else {
    wheel_right.writeMicroseconds(WHEEL_R_MID + VEL);
    wheel_left.writeMicroseconds(WHEEL_L_MID - VEL);
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
  Serial.println("Robot autónomo listo y ejecutando...");
}

/************************************************************************
 * LOOP PRINCIPAL
 ************************************************************************/
void loop() {
  readSensors();
  updateVelocities();
  printOctagonVisual();  // Mostrar diagrama visual
  delay(500);            // Frecuencia de actualización
} 
