# About PZEM_DATA_LOGGER.gs

This file is responsible for inserting rows inside a pre-built spreadsheet template using ESP 32. 

---
## How to use script file

All steps are followed [here](https://youtu.be/u7TYu61l0t4?si=HXf8RF_EPv9PDrhh).

### First 
Create a spreadsheet template and get the spreadsheet id inside the url. 

![Imgur](https://imgur.com/s853gpy.jpg)

you can also customize the template to your liking.

### Second 
Look and go to `App Script` by clicking on Extensions>Apps Script.

![Imgur](https://i.imgur.com/uVYjHQC.png)

Copy [PZEM_DATA_LOGGER.gs](https://github.com/enrixk28/PZEM_DATA_LOGGER/tree/main/google_script/PZEM_DATA_LOGGER.gs) and paste it inside App Script.

### Lastly 
Click the `deploy` button on the top right corner of Apps Script.

Locate the _url_ under **Web App** and paste it inside [latest_esp32_code.ino file](https://github.com/enrixk28/PZEM_DATA_LOGGER/tree/main/latest_esp32_code). 

![Imgur](https://imgur.com/7cxfawn.jpg)

Seen in line:51.

***Done! you can now insert data inside your google spreadsheet***

## Further Reading
- [Introduction](https://github.com/enrixk28/PZEM_DATA_LOGGER/tree/main/README.md)
- [Arduino](https://github.com/enrixk28/PZEM_DATA_LOGGER/tree/main/arduino_code/ARDUINO.md)
- [ESP32](https://github.com/enrixk28/PZEM_DATA_LOGGER/tree/main/latest_esp32_code/ESP32.md)