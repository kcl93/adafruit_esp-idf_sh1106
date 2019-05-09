# Adafruit library for SH1106 OLEDs using esp-idf

This library is based on wonho-maker's fork of the Adafruit SSD1306 library as well as the standard Adafruit GFX library.

The target of this library is to allow users to control any SH1106 based oled display via SPI using the esp-idf framework for ESP32 microcontrollers instead of the Arduino framework.
Both I2C and software SPI are currently not supported as they are relatively slow and there are two HW SPI interfaces anyway available with the ESP32.


Adafruit_SH1106: https://github.com/wonho-maker/Adafruit_SH1106

Adafruit-GFX-Library: https://github.com/adafruit/Adafruit-GFX-Library
