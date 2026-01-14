from machine import Pin, PWM
import time

# Pines de los motores
WHEEL_R_PIN = 14   # Motor derecho
WHEEL_L_PIN = 11   # Motor izquierdo

# Microsegundos de punto medio (stop)
WHEEL_R_MID = 1590
WHEEL_L_MID = 1590

# Velocidades
VELDR = 220
VELIZ = 100

# Crear PWM a 50 Hz
wheel_right = PWM(Pin(WHEEL_R_PIN))
wheel_left  = PWM(Pin(WHEEL_L_PIN))

wheel_right.freq(50)
wheel_left.freq(50)

# Función equivalente a writeMicroseconds de Arduino
def writeMicroseconds(pwm, us):
    pwm.duty_ns(us * 1000)   # convierte microsegundos → nanosegundos


# Colocar motores en reposo
writeMicroseconds(wheel_right, WHEEL_R_MID)
writeMicroseconds(wheel_left,  WHEEL_L_MID)
print("Motores listos en Raspberry Pi Pico.")


while True:

    # --------- AVANZAR 5s ---------
    print("Avanzando 5s...")
    writeMicroseconds(wheel_right, WHEEL_R_MID - VELDR)
    writeMicroseconds(wheel_left,  WHEEL_L_MID + VELIZ)
    time.sleep(5)

    # Pausa breve
    writeMicroseconds(wheel_right, WHEEL_R_MID)
    writeMicroseconds(wheel_left,  WHEEL_L_MID)
    time.sleep(0.3)

    # --------- RETROCEDER 5s ---------
    print("Retrocediendo 5s...")
    writeMicroseconds(wheel_right, WHEEL_R_MID + VELDR)
    writeMicroseconds(wheel_left,  WHEEL_L_MID - VELIZ)
    time.sleep(5)

    # Pausa breve
    writeMicroseconds(wheel_right, WHEEL_R_MID)
    writeMicroseconds(wheel_left,  WHEEL_L_MID)
    time.sleep(0.3)
