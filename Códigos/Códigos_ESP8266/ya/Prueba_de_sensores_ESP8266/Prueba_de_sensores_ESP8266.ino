#define MUX_A_PIN 15  // D8
#define MUX_B_PIN 13  // D7
#define MUX_C_PIN 12  // D6
#define MUX_EN_PIN 2  // D4
#define SENSOR_COUNT 8

const int real_pos[8] = { 0,1,2,7,3,6,4,5 };

#define SENSOR_ADC_PIN A0   // único pin ADC del ESP8266

void setup() {
  Serial.begin(115200);
  pinMode(MUX_A_PIN, OUTPUT);
  pinMode(MUX_B_PIN, OUTPUT);
  pinMode(MUX_C_PIN, OUTPUT);
  pinMode(MUX_EN_PIN, OUTPUT);

  digitalWrite(MUX_EN_PIN, LOW); // emisores apagados
  Serial.println("HeRo 2.0 - Prueba de sensores IR");
}

void loop() {
  for (int sensor = 0; sensor < SENSOR_COUNT; sensor++) {
    selectInput(sensor);
    float lectura = leerSensor();

    Serial.print("Sensor ");
    Serial.print(real_pos[sensor]);
    Serial.print(": ");
    Serial.println(lectura > 50 ? "SI" : "NO"); // umbral ajustable

    delay(200);
  }
  Serial.println("---------------------");
  delay(500);
}

void selectInput(int input){
  digitalWrite(MUX_A_PIN, input & 0x01);
  digitalWrite(MUX_B_PIN, (input >> 1) & 0x01);
  digitalWrite(MUX_C_PIN, (input >> 2) & 0x01);
}

float leerSensor(){ 
  delayMicroseconds(500);
  float fondo = analogRead(SENSOR_ADC_PIN);

  digitalWrite(MUX_EN_PIN, HIGH); // enciende emisores
  delayMicroseconds(100);

  float lectura = analogRead(SENSOR_ADC_PIN);
  digitalWrite(MUX_EN_PIN, LOW);  // apaga emisores

  return lectura - fondo;
}

