#include <Adafruit_NeoPixel.h>

#define PIN 48        
#define NUM_LEDS 3   // 0 = LED de la placa, 1 y 2 = PCB

Adafruit_NeoPixel leds(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  leds.begin();
  leds.clear();   // Asegura que todo inicie apagado
  leds.show();
}

void loop() {

  // 1 Mantener color estable por 5 segundos
  setColorPCB(0, 150, 255);   // azul/cian
  delay(5000);

  // 2 Parpadear durante 5 segundos
  parpadearTiempoPCB(0, 150, 255, 300, 5000);
}

// -------- FUNCIONES ---------

// Solo LEDS de la PCB (1 y 2)
void setColorPCB(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 1; i < NUM_LEDS; i++) {   // empieza en 1
    leds.setPixelColor(i, leds.Color(r, g, b));
  }
  leds.show();
}

void apagarPCB() {
  for (int i = 1; i < NUM_LEDS; i++) {
    leds.setPixelColor(i, 0);
  }
  leds.show();
}

// Parpadear por tiempo
void parpadearTiempoPCB(uint8_t r, uint8_t g, uint8_t b, int tiempoParpadeo, int duracionTotal) {

  unsigned long inicio = millis();

  while (millis() - inicio < duracionTotal) {
    setColorPCB(r, g, b);
    delay(tiempoParpadeo);

    apagarPCB();
    delay(tiempoParpadeo);
  }
}
