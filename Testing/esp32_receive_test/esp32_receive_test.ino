#include <WiFi.h>

// Define the pins for Serial2
#define RX_PIN 16  // GPIO 16 for RX
#define TX_PIN 17  // GPIO 17 for TX

// Create a HardwareSerial object for Serial2
HardwareSerial arduinoESPSerial(1);

float results1[6];
float results2[6];
float results3[6];

void print_values(float arr[6], int index) {
  Serial.println("   Values for PZEM "+(String)index);
  Serial.println("Voltage: "+(String)arr[0]);
  Serial.println("Current: "+(String)arr[1]);
  Serial.println("Power: "+(String)arr[2]);
  Serial.println("Energy: "+(String)arr[3]);
  Serial.println("Frequency: "+(String)arr[4]);
  Serial.println("Power Factor: "+(String)arr[5]);
  Serial.println("\n");
}

void setup() {
  Serial.begin(9600);
  arduinoESPSerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);

      // Initialize Blynk connection
  // Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  if (arduinoESPSerial.available()) {
    Serial.print("\033[0H\033[0J"); // Deletes previous prints in PuTTY Serial Monitor
    Serial.println("Data Received");

    parseString(arduinoESPSerial.readString());

    print_values(results1, 1);
    print_values(results2, 2);
    print_values(results3, 3);
  }
}

// Function to parse the received string
void parseString(String input) {
  int index = 0;
  int startIndex = 0;
  float tempArray[18]; // Temporary array to store the extracted float values
  String floatValueString;

  // Iterate through the string and extract float values
  for (int i = 0; i < input.length(); i++) {
    if (input[i] == ',' || i == input.length() - 1) {
      // Extract the substring representing the float value
      if (i == input.length() - 1) {
        floatValueString = input.substring(startIndex, i + 1);
      } else {
        floatValueString = input.substring(startIndex, i);
      }
      
      // Parse the string to float and store in tempArray
      tempArray[index] = floatValueString.toFloat();
      index++;
      
      // Update the startIndex for the next value
      startIndex = i + 1;
    }
  }
  
  // Assign the parsed values to results1, results2, and results3 arrays
  for (int i = 0; i < 6; i++) {
    results1[i] = tempArray[i];
    results2[i] = tempArray[i + 6];
    results3[i] = tempArray[i + 12];
  }
}
