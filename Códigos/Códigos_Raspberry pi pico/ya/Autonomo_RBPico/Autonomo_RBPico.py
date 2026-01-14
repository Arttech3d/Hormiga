# ===============================
# Robot autónomo - Raspberry Pi Pico (MicroPython)
# ===============================

from machine import Pin, ADC, PWM
from time import sleep, sleep_us

# ===============================
# CONFIGURACIÓN DEL MULTIPLEXOR Y SENSORES
# ===============================

MUX_A_PIN = Pin(6, Pin.OUT)
MUX_B_PIN = Pin(7, Pin.OUT)
MUX_C_PIN = Pin(8, Pin.OUT)
MUX_EN_PIN = Pin(10, Pin.OUT)
SENSOR_ADC = ADC(26)  # ADC0 = GPIO26

SENSOR_COUNT = 8
real_pos = [0, 1, 2, 7, 3, 6, 4, 5]
sensor_readings = [0] * SENSOR_COUNT
PROXIMITY_THRESH = 40  # umbral de detección

# ===============================
# CONFIGURACIÓN DE LOS SERVOMOTORES
# ===============================

WHEEL_R_PIN = 14  
WHEEL_L_PIN = 11  
WHEEL_R_MID = 1500
WHEEL_L_MID = 1500

VELDR = 220   # Velocidad motor derecho
VELIZ = 100   # Velocidad motor izquierdo

wheel_right = PWM(Pin(WHEEL_R_PIN))
wheel_left = PWM(Pin(WHEEL_L_PIN))

# Servos: 50 Hz (20 ms)
wheel_right.freq(50)
wheel_left.freq(50)

# ===============================
# FUNCIONES DEL MULTIPLEXOR Y SENSORES
# ===============================

def select_input(i):
    MUX_A_PIN.value(i & 0x01)
    MUX_B_PIN.value((i >> 1) & 0x01)
    MUX_C_PIN.value((i >> 2) & 0x01)

def write_us(pwm, us):
    # Convierte microsegundos a duty_u16
    duty = int(us / 20000 * 65535)
    pwm.duty_u16(duty)

def read_sensor():
    sleep_us(500)
    env = SENSOR_ADC.read_u16() / 64  # escalar a 0–1023
    MUX_EN_PIN.value(1)               # encender emisores
    sleep_us(100)
    lectura = (SENSOR_ADC.read_u16() / 64) - env
    MUX_EN_PIN.value(0)               # apagar emisores
    if lectura < 0:
        lectura = 1
    return lectura

def read_sensors():
    print("\nLeyendo sensores...")
    for i in range(SENSOR_COUNT):
        select_input(i)
        sensor_readings[real_pos[i]] = read_sensor()
        print(f"Sensor {i}: {sensor_readings[real_pos[i]]:.2f}")

# ===============================
# CONTROL DE MOVIMIENTO AUTÓNOMO
# ===============================

def update_velocities():
    obst_front   = sensor_readings[0] >= PROXIMITY_THRESH
    obst_fleft   = sensor_readings[1] >= PROXIMITY_THRESH
    obst_left    = sensor_readings[2] >= PROXIMITY_THRESH
    obst_bleft   = sensor_readings[3] >= PROXIMITY_THRESH
    obst_back    = sensor_readings[4] >= PROXIMITY_THRESH
    obst_bright  = sensor_readings[5] >= PROXIMITY_THRESH
    obst_right   = sensor_readings[6] >= PROXIMITY_THRESH
    obst_fright  = sensor_readings[7] >= PROXIMITY_THRESH

    print("\n--- Análisis de obstáculos ---")
    if obst_front or obst_fright or obst_fleft:
        print("Obstáculo al frente. Girando...")
        sum_left  = sensor_readings[1] + sensor_readings[2] + sensor_readings[3]
        sum_right = sensor_readings[7] + sensor_readings[6] + sensor_readings[5]
        if sum_left < sum_right:
            print("→ Giro a la izquierda")
            write_us(wheel_right, WHEEL_R_MID + VELDR)
            write_us(wheel_left,  WHEEL_L_MID + VELIZ)
        else:
            print("→ Giro a la derecha")
            write_us(wheel_right, WHEEL_R_MID - VELDR)
            write_us(wheel_left,  WHEEL_L_MID - VELIZ)

    elif obst_left or obst_bleft:
        print("Obstáculo a la izquierda → girando a la derecha")
        write_us(wheel_right, WHEEL_R_MID - VELDR)
        write_us(wheel_left,  WHEEL_L_MID - VELIZ)

    elif obst_right or obst_bright:
        print("Obstáculo a la derecha → girando a la izquierda")
        write_us(wheel_right, WHEEL_R_MID + VELDR)
        write_us(wheel_left,  WHEEL_L_MID + VELIZ)

    elif obst_back:
        print("Obstáculo atrás → avanzando hacia adelante")
        write_us(wheel_right, WHEEL_R_MID - VELDR - 50)
        write_us(wheel_left,  WHEEL_L_MID + VELIZ + 50)

    else:
        print("Sin obstáculos → avanzando")
        write_us(wheel_right, WHEEL_R_MID - VELDR)
        write_us(wheel_left,  WHEEL_L_MID + VELIZ)

# ===============================
# SETUP
# ===============================

MUX_EN_PIN.value(0)
write_us(wheel_right, WHEEL_R_MID)
write_us(wheel_left, WHEEL_L_MID)

print("=======================================")
print(" Robot autónomo iniciado correctamente")
print(" Escaneando entorno y ajustando ruta...")
print("=======================================")
print("Usa VELDR y VELIZ para ajustar la velocidad individual de los motores.\n")

# ===============================
# LOOP PRINCIPAL
# ===============================

while True:
    print("\n--------------------------------------")
    print("Lectura de sensores y control de movimiento")
    print("--------------------------------------")
    read_sensors()
    update_velocities()
    sleep(0.5)
