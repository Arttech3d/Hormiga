// ===============================
// LECTURA DE VOLTAJE - ESP8266
// ===============================
#define PIN_ADC A0
#define VREF 3.3
#define ADC_RES 1023.0

void setup() {
  Serial.begin(115200);
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
