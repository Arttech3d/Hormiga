// ===============================
// LECTURA DE VOLTAJE - ESP32
// ===============================
#define PIN_ADC 5        // Pin ADC (solo entrada)
#define VREF 3.3
#define ADC_RES 4095.0    // 12 bits

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
}

void loop() {
  int adc_raw = analogRead(PIN_ADC);
  float voltaje = (adc_raw * VREF) / ADC_RES;

  Serial.print("ADC: ");
  Serial.print(adc_raw);
  Serial.print("  Voltaje: ");
  Serial.print(voltaje, 3);
  Serial.println(" V");

  delay(200);
}

