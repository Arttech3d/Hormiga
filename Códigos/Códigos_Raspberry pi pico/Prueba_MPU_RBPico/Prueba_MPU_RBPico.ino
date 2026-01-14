PU#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  delay(100);

  if (!mpu.begin()) {
    Serial.println("MPU6050 NO encontrado");
    while (1) delay(10);
  }
  Serial.println("MPU6050 listo!");
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Aceleración
  Serial.print("AX: "); Serial.print(a.acceleration.x);
  Serial.print(" AY: "); Serial.print(a.acceleration.y);
  Serial.print(" AZ: "); Serial.print(a.acceleration.z);

  // Giroscopio
  Serial.print(" | GX: "); Serial.print(g.gyro.x);
  Serial.print(" GY: "); Serial.print(g.gyro.y);
  Serial.print(" GZ: "); Serial.print(g.gyro.z);

  // Temperatura
  Serial.print(" | T: "); Serial.print(temp.temperature);
  Serial.println(" C");

  delay(200);
}
