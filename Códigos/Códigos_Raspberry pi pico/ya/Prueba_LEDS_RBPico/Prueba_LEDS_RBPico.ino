#include <Adafruit_NeoPixel.h>

#define PIN 2
#define NUM_LEDS 2

Adafruit_NeoPixel leds(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  leds.begin();
}

void loop() {

  // 1 Mantener color estable por 5 segundos
  setColor(0, 150, 255);   // azul/cian
  delay(5000);

  // 2 Parpadear durante 5 segundos
  parpadearTiempo(0, 150, 255, 300, 5000);
}


// -------- FUNCIONES ---------

void setColor(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds.setPixelColor(i, leds.Color(r, g, b));
  }
  leds.show();
}


// Parpadear N milisegundos
void parpadearTiempo(uint8_t r, uint8_t g, uint8_t b, int tiempoParpadeo, int duracionTotal) {

  unsigned long inicio = millis();

  while (millis() - inicio < duracionTotal) {

    // Encendido
    setColor(r, g, b);
    delay(tiempoParpadeo);

    // Apagado
    setColor(0, 0, 0);
    delay(tiempoParpadeo);
  }
}
