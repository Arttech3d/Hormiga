from machine import Pin, I2C
import time
from mpu6050 import MPU6050

i2c = I2C(0, scl=Pin(5), sda=Pin(4), freq=400000)

mpu = MPU6050(i2c)

while True:
    print("Aceleración:", mpu.get_accel_data())
    print("Giroscopio:", mpu.get_gyro_data())
    print("Temperatura:", mpu.get_temp(), "°C")
    print("-----------------------")
    time.sleep(0.5)
