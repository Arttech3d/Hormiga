# Código para Raspberry Pi Pico en MicroPython
from machine import Pin, ADC
from time import sleep, sleep_us

# Pines del multiplexor
MUX_A_PIN = Pin(6, Pin.OUT)
MUX_B_PIN = Pin(7, Pin.OUT)
MUX_C_PIN = Pin(8, Pin.OUT)
MUX_EN_PIN = Pin(10, Pin.OUT)

# Pin ADC del sensor
SENSOR_ADC_PIN = ADC(26)

# Configuración inicial
SENSOR_COUNT = 8
real_pos = [0, 1, 2, 7, 3, 6, 4, 5]

print("HeRo 2.0 - Prueba de sensores IR")

def select_input(input_num):
    """Selecciona la entrada del multiplexor."""
    MUX_A_PIN.value(input_num & 0x01)
    MUX_B_PIN.value((input_num >> 1) & 0x01)
    MUX_C_PIN.value((input_num >> 2) & 0x01)

def leer_sensor():
    """Lee el valor del sensor con y sin emisor encendido."""
    sleep_us(500)
    fondo = SENSOR_ADC_PIN.read_u16()

    MUX_EN_PIN.value(1)  # Enciende emisores
    sleep_us(100)
    lectura = SENSOR_ADC_PIN.read_u16()
    MUX_EN_PIN.value(0)  # Apaga emisores

    return lectura - fondo

# Bucle principal
while True:
    for sensor in range(SENSOR_COUNT):
        select_input(sensor)
        lectura = leer_sensor()

        estado = "SI" if lectura > 3000 else "NO"  # Ajusta el umbral según el sensor
        print(f"Sensor {real_pos[sensor]}: {estado}")

        sleep(0.2)

    print("---------------------")
    sleep(0.5)
