const int pinAnalogico = A1;
const float voltajeReferencia = 5.0;  // 5V para Arduino Nano

void setup() {
  Serial.begin(9600);
  Serial.println("Lectura de voltaje en A0");
}

void loop() {
  int valorADC = analogRead(pinAnalogico);
  
  // Convertir el valor ADC a voltaje
  float voltaje = (valorADC * voltajeReferencia) / 1023.0;

  Serial.print("ADC: ");
  Serial.print(valorADC);
  Serial.print("  |  Voltaje: ");
  Serial.print(voltaje, 3); // 3 decimales
  Serial.println(" V");

  delay(500);
}
