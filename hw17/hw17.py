from time import sleep
from picamera import PiCamera
from PIL import Image

import serial
ser = serial.Serial(port='/dev/ttyS0', baudrate = 115200, timeout=0.5)

camera = PiCamera()
camera.resolution = (640, 480)


while True:
    camera.capture('image.jpg')

    img_data = Image.open("image.jpg")
    img_data = img_data.convert('RGB')

    width, height = img_data.size

    alpha = 0.95
    beta = 0.05

    accum = 0
    count = 0

    for i in range(width):
        r, g, b = img_data.getpixel((i, 50))
        sum = r+g+b
        if sum >= 300:
            accum = accum + i
            count = count + 1
    if count != 0:
        avgval = int (accum * 100 / count / 640)
        ser.write((str(avgval)+ '\n').encode())
        print(avgval)
    