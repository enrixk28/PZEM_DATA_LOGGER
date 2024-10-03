# About ESP 32 code

This file is responsible for storing data received from an Arduino Uno unto:

- [Blynk](https://blynk.io)
- [Google Spreadsheet](https://docs.google.com/spreadsheets/)
- MicroSD 

## Important Details

> [!IMPORTANT]
> ESP32 needs to connect to wifi in order to transfer data to spreadsheet and blynk.

#### Blynk
```cpp
#define BLYNK_TEMPLATE_ID "" 
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""  
```
Must be inserted at the first line to be able to connect the _Blynk_ application.

### Google Spreadsheet
```cpp
String webAppUrl = "";
```
This connects to the [Apps Script](https://github.com/enrixk28/PZEM_DATA_LOGGER/tree/main/google_script/PZEM_DATA_LOGGER.gs) file.

## Further Reading
- [Introduction](https://github.com/enrixk28/PZEM_DATA_LOGGER/tree/main/README.md)
- [Arduino](https://github.com/enrixk28/PZEM_DATA_LOGGER/tree/main/arduino_code/ARDUINO.md)
- [Google Apps Script](https://github.com/enrixk28/PZEM_DATA_LOGGER/tree/main/google_script/APPSCRIPT.md)
