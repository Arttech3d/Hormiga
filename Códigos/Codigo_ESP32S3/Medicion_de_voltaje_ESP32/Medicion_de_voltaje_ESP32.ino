/*
====================================
LECTURA DE 2 SENSORES – ESP32-S3
====================================

- Sensor 1 → GPIO1
- Sensor 2 → GPIO2
- ADC: 12 bits (0–4095)
- Voltaje máximo ADC: 3.3 V

IMPORTANTE:
------------------------------------
- NO exceder 3.3 V en ningún pin
- Algunos pines pueden no ser ADC según la placa
*/

#define PIN_ADC1 1   // GPIO1
#define PIN_ADC2 2   // GPIO2

#define VREF 3.3
#define ADC_RES 4095.0

void setup() {
  Serial.begin(115200);

  // Configurar resolución ADC
  analogReadResolution(12);

  // Opcional: mejorar estabilidad
  analogSetAttenuation(ADC_11db);  
  // ADC_11db permite medir hasta ~3.3V
}

void loop() {
  // Leer sensores
  int adc1 = analogRead(PIN_ADC1);
  int adc2 = analogRead(PIN_ADC2);

  // Convertir a voltaje
  float voltaje1 = (adc1 * VREF) / ADC_RES;
  float voltaje2 = (adc2 * VREF) / ADC_RES;

  // Mostrar resultados
  Serial.print("Sensor 1 -> ADC: ");
  Serial.print(adc1);
  Serial.print(" | Voltaje: ");
  Serial.print(voltaje1, 3);
  Serial.print(" V");

  Serial.print("   ||   ");

  Serial.print("Sensor 2 -> ADC: ");
  Serial.print(adc2);
  Serial.print(" | Voltaje: ");
  Serial.print(voltaje2, 3);
  Serial.println(" V");

  delay(100);
}
