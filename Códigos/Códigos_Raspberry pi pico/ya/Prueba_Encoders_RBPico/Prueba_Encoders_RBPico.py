from machine import Pin
import time

# Pines
encA_L = Pin(12, Pin.IN, Pin.PULL_UP)
encB_L = Pin(9, Pin.IN, Pin.PULL_UP)

encA_R = Pin(22, Pin.IN, Pin.PULL_UP)
encB_R = Pin(13, Pin.IN, Pin.PULL_UP)

# Contadores
left_count = 0
right_count = 0

# Estados previos
last_state_L = encA_L.value()
last_state_R = encA_R.value()

def left_irq(pin):
    global left_count, last_state_L
    state = pin.value()
    if state != last_state_L:
        if encB_L.value() != state:
            left_count += 1
        else:
            left_count -= 1
        last_state_L = state

def right_irq(pin):
    global right_count, last_state_R
    state = pin.value()
    if state != last_state_R:
        if encB_R.value() != state:
            right_count += 1
        else:
            right_count -= 1
        last_state_R = state

# Interrupciones
encA_L.irq(trigger=Pin.IRQ_RISING | Pin.IRQ_FALLING, handler=left_irq)
encA_R.irq(trigger=Pin.IRQ_RISING | Pin.IRQ_FALLING, handler=right_irq)

print("Encoder Test - Pico MicroPython")

while True:
    print(left_count, ",", right_count)
    time.sleep_ms(20)

