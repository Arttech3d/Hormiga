#define MUX_A_PIN 21  // GPIO21
#define MUX_B_PIN 47  // GPIO47
#define MUX_C_PIN 45  // GPIO45
#define MUX_EN_PIN 48  // GPIO48
#define SENSOR_COUNT 8 /* MUX number of channels */
#define A0 7           /* ADC ESP32-S3 */

const int real_pos[8] = { 0, 1, 2, 7, 3, 6, 4, 5 }; /* Sort proximity sensors */
float sensor_readings[SENSOR_COUNT];

#define SERIAL_DATA false

struct __attribute__((packed)) Data_t {
  uint16_t ir_0 = 0;
  uint16_t ir_1 = 0;
  uint16_t ir_2 = 0;
  uint16_t ir_3 = 0;
  uint16_t ir_4 = 0;
  uint16_t ir_5 = 0;
  uint16_t ir_6 = 0;
  uint16_t ir_7 = 0;
};

Data_t data_;

void setup() {
  Serial.begin(115200);

  pinMode(MUX_A_PIN, OUTPUT);
  pinMode(MUX_B_PIN, OUTPUT);
  pinMode(MUX_C_PIN, OUTPUT);
  pinMode(MUX_EN_PIN, OUTPUT);

  digitalWrite(MUX_EN_PIN, LOW);  // emisores apagados al inicio
}

void selectInput(int input) {
  digitalWrite(MUX_A_PIN, input & 0x01);
  digitalWrite(MUX_B_PIN, (input >> 1) & 0x01);
  digitalWrite(MUX_C_PIN, (input >> 2) & 0x01);
}

float readSensor() {
  // 1. Lectura de luz ambiente
  delayMicroseconds(500);
  float env = (float)analogRead(A0);

  // 2. Encender emisores
  digitalWrite(MUX_EN_PIN, HIGH);
  delayMicroseconds(100);

  // 3. Lectura con emisores encendidos
  float sensor_reading = (float)analogRead(A0) - env;

  // 4. Apagar emisores
  digitalWrite(MUX_EN_PIN, LOW);
  delayMicroseconds(80);

  return sensor_reading;
}

void readSensors() {
  for (int sensor = 0; sensor < SENSOR_COUNT; sensor++) {
    selectInput(sensor);
    sensor_readings[real_pos[sensor]] = readSensor();
  }
}

void loop() {
  readSensors();

  Serial.printf("%f,",  sensor_readings[0]);
  Serial.printf("%f,",  sensor_readings[1]);
  Serial.printf("%f,",  sensor_readings[2]);
  Serial.printf("%f,",  sensor_readings[3]);
  Serial.printf("%f,",  sensor_readings[4]);
  Serial.printf("%f,",  sensor_readings[5]);
  Serial.printf("%f,",  sensor_readings[6]);
  Serial.printf("%f\n", sensor_readings[7]);

  delayMicroseconds(100000);
}
