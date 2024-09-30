#include <SoftwareSerial.h>
SoftwareSerial arduinoESPSerial(8, 9); // RX, TX for Arduino to ESP Communication

// Define float variables
float voltage[3] = {220, 230, 225};
float current[3] = {4.67, 3.42, 1.32};
float power[3] = {12, 20.34, 53.12};
float energy[3] = {32, 57.54, 63.64};
float frequency[3] = {60, 59.8, 58.94};
float powerFactor[3] = {0.99, 0.97, 0.98};

void setup() {
  Serial.begin(9600);  // Start UART communication
  arduinoESPSerial.begin(9600);

  // Create a string to hold the concatenated float values
  String dataToSend[3];

  for (int i=0; i<3; i++) {
    dataToSend[i] += String(voltage[i], 2) + ",";
    dataToSend[i] += String(current[i], 2) + ",";
    dataToSend[i] += String(power[i], 2) + ",";
    dataToSend[i] += String(energy[i], 2) + ",";
    dataToSend[i] += String(frequency[i], 2) + ",";
    dataToSend[i] += String(powerFactor[i], 2) + ",";

    Serial.println(dataToSend[i]);
    // Send the concatenated string over UART
    arduinoESPSerial.print(dataToSend[i]);
    delay(500);
  }
}

void loop() {
}