from machine import Pin
import time
import neopixel

PIN = 2
NUM_LEDS = 2

leds = neopixel.NeoPixel(Pin(PIN), NUM_LEDS)

# -------- FUNCIONES ---------

def setColor(r, g, b):
    for i in range(NUM_LEDS):
        leds[i] = (r, g, b)
    leds.write()

def parpadearTiempo(r, g, b, tiempoParpadeo, duracionTotal):
    inicio = time.ticks_ms()

    while time.ticks_diff(time.ticks_ms(), inicio) < duracionTotal:
        # Encendido
        setColor(r, g, b)
        time.sleep_ms(tiempoParpadeo)

        # Apagado
        setColor(0, 0, 0)
        time.sleep_ms(tiempoParpadeo)

# -------- MAIN LOOP ---------

while True:
    # Color fijo 5 segundos
    setColor(0, 150, 255)
    time.sleep(5)

    # Parpadeo 5 segundos
    parpadearTiempo(0, 150, 255, 300, 5000)
