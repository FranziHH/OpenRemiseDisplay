# Use ESP32-C3-Super-Mini as OpenRemise Display


Start Develop own OpenRemise Display

Current State: alpha

### Used Hardware

- ESP32-C3 Super Mini
- any 128x64 OLED Display 
    - SSD1306
    - SH1106
    - SSD1309

### Hardware Connection

    OLED VCC -> C3 3.3V
    OLED GND -> C3 GND
    OLED SDA -> C3 GPIO 8
    OLED SCL -> C3 GPIO 9

    RX/TX Kreuzverbindung
    
    Btn 1 -> GPIO 4
    Btn 2 -> GPIO 5
    Btn 3 -> GPIO 6
    Btn 4 -> GPIO 7
      |-- -> GND
    
    


    