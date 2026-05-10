# Use ESP32-C3-Super-Mini as OpenRemise Display

### Changes

Optionally, the internal blue LED (GPIO8) is used as an indicator for the track voltage.

## Upgrade OpenRemise Hardware Rev 0.1.0 => 0.1.2

Two resistors and some soldering skills are required:
- 1 x 14.3 kΩ
- 1 x 470 Ω

See image for connections.

<a href="images/IMG_20260510_123416_001.jpg"><img src="images/IMG_20260510_123416_001.jpg" width="500" alt=""></a>   

<a href="images/2026-05-10_15-49-15.png"><img src="images/2026-05-10_15-49-15.png" width="500" alt=""></a>   


### With a large 2.4" Display 

<a href="images/display/IMG_20260419_114529.jpg"><img src="images/display/IMG_20260419_114529.jpg" width="500" alt=""></a>

### Compact Version with 1.3" Display

<a href="images/display/IMG_20260424_202140.jpg"><img src="images/display/IMG_20260424_202140.jpg" width="500" alt=""></a>

The two reset buttons are connected here to reset the display synchronously.

### Used Hardware

- ESP32-C3 Super Mini
- any 128x64 OLED Display 
    - SSD1306
    - SH1106
    - SSD1309
- optional: W5500 SPI Ethernet Module - TCP/IP, compatible with WIZ820io 


### Hardware Connection

<a href="images/display/connection_schema.png"><img src="images/display/connection_schema.png" width="500" alt=""></a>


    OLED VCC -> C3 3.3V
    OLED GND -> C3 GND
    OLED SDA -> C3 GPIO 8
    OLED SCL -> C3 GPIO 9

    RX/TX Cross connection
    
    Btn 1 -> GPIO 4 Switch Views
    Btn 2 -> GPIO 5 (currently not in use)
    Btn 3 -> GPIO 6 (currently not in use)
    Btn 4 -> GPIO 7 (currently not in use)
      |-- -> GND
    
## Original Data

<table>
  <tr>
    <td valign="top">
      <a href="images/display/IMG_20260419_120004.jpg"><img src="images/display/IMG_20260419_120004.jpg" width="300" alt=""></a>
    </td>
    <td valign="top">
      <a href="images/display/IMG_20260419_120010.jpg"><img src="images/display/IMG_20260419_120010.jpg" width="300" alt=""></a>
    </td>
    <td valign="top">
      <a href="images/display/IMG_20260419_120047.jpg"><img src="images/display/IMG_20260419_120047.jpg" width="300" alt=""></a>
    </td>
  </tr>
</table>

## Extended Data

<table>
  <tr>
    <td valign="top">
      <a href="images/display/IMG_20260419_114633.jpg"><img src="images/display/IMG_20260419_114633.jpg" width="300" alt=""></a>
    </td>
    <td valign="top">
      <a href="images/display/IMG_20260419_114645.jpg"><img src="images/display/IMG_20260419_114645.jpg" width="300" alt=""></a>
    </td>
    <td valign="top">
      <a href="images/display/IMG_20260419_114746.jpg"><img src="images/display/IMG_20260419_114746.jpg" width="300" alt=""></a>
    </td>
  </tr>
  <tr>
    <td valign="top">
      <a href="images/display/IMG_20260419_114826.jpg"><img src="images/display/IMG_20260419_114826.jpg" width="300" alt=""></a>
    </td>
    <td valign="top">
      <a href="images/display/IMG_20260419_114837.jpg"><img src="images/display/IMG_20260419_114837.jpg" width="300" alt=""></a>
    </td>
    <td valign="top">
      <a href="images/display/IMG_20260419_115042.jpg"><img src="images/display/IMG_20260419_115042.jpg" width="300" alt=""></a>
    </td>
  </tr>
  <tr>
    <td valign="top">
      <a href="images/display/IMG_20260419_114903.jpg"><img src="images/display/IMG_20260419_114903.jpg" width="300" alt=""></a>
    </td>
    <td valign="top">
      <a href="images/display/IMG_20260419_114934.jpg"><img src="images/display/IMG_20260419_114934.jpg" width="300" alt=""></a>
    </td>
    <td valign="top">
      <a href="images/display/IMG_20260419_115459.jpg"><img src="images/display/IMG_20260419_115459.jpg" width="300" alt=""></a>
    </td>
  </tr>
</table>

    