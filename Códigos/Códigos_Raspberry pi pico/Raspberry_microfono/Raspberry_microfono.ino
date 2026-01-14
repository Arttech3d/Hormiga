#include <I2S.h>

#define I2S_SD   16   // Datos
#define I2S_SCK  18   // Bit Clock
#define I2S_WS   19   // Word Select

const int SAMPLE_RATE = 16000;
const int BUFFER_SIZE = 256;

int32_t buffer[BUFFER_SIZE];

void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println("Iniciando INMP441 con Raspberry Pi Pico...");

  I2S.setDATA(I2S_SD);
  I2S.setBCLK(I2S_SCK);
  I2S.setWS(I2S_WS);

  if (!I2S.begin(I2S_PHILIPS_MODE, SAMPLE_RATE, 32)) {
    Serial.println("Error al iniciar I2S");
    while (1);
  }

  Serial.println("I2S iniciado correctamente");
}

void loop() {
  int samples = 0;
  double sum = 0;

  while (samples < BUFFER_SIZE) {
    if (I2S.available()) {
      int32_t sample = I2S.read();
      sample >>= 8;              // Ajuste de 24 a 16 bits aprox
      buffer[samples++] = sample;
      sum += (double)sample * sample;
    }
  }

  double rms = sqrt(sum / BUFFER_SIZE);
  Serial.print("RMS: ");
  Serial.println(rms);

  delay(100);
}
