# ESP32-S3 and Nextion with ESP-IDF

### How to use
1. git clone `https://github.com/octarudin/esp32s3-nextion-idf.git`
2. Build the project
3. Flash the project
4. See your nextion display. `Hello World` will be seen.


### Hardware Required
Microcontroller: ESP32-S3-N16R8
Display Module: Any Nextion Display

### Pin Configuration
| Nextion  | ESP32-S3 |
|----------|----------|
| TXD      | 1 (TX)   |
| RXD      | 2 (RX)   |

The UART connection between these devices is not cross-connection (RX-TX). UART channel is using UART2.

### Circuit
<figure>  
  <img src="docs/circuits.jpg" width="60%">  
  <figcaption>  
    Figure 1. Circuit diagram showing the connection between the microcontroller and nextion module    
  </figcaption>  
</figure>  


