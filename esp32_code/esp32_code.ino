#define BLYNK_TEMPLATE_ID "TMPL6x0IEVVhH"
#define BLYNK_TEMPLATE_NAME "AMBOT"
#define BLYNK_AUTH_TOKEN "NzTVNknbve7C7HixsTzsc0-ih0Zom0Oy"

#include <WiFi.h>
#include <PZEM004Tv30.h>
#include <BlynkSimpleEsp32.h>

// Define the pins for Serial2
#define RX_PIN 16  // GPIO 16 for RX
#define TX_PIN 17  // GPIO 17 for TX

// Create a HardwareSerial object for Serial2
HardwareSerial arduinoESPSerial(1);

// Your WiFi credentials
char ssid[] = "TUPT_WiFi";
char pass[] = "ilovetup";

// Blynk Virtual Pins for monitoring
#define VPIN_VOLTAGE V0
#define VPIN_CURRENT V1
#define VPIN_POWER V2
#define VPIN_ENERGY V3
#define VPIN_FREQ V4
#define VPIN_PF V5

// float receivedArray[6];
float results1[6];
float results2[6];
float results3[6];

// Function to receive and decode float array over SoftwareSerial
void readFloatArray(float arr[], int size) {
  while (arduinoESPSerial.available() < size * sizeof(float));  // Wait until all bytes are available
  arduinoESPSerial.readBytes((byte*)arr, size * sizeof(float));  // Read the entire float array
}

// // Function to receive and decode float array over SoftwareSerial
// void readFloatArray(float arr[], int size) {
//   byte byteBuffer[sizeof(float)];
//   int arrayIndex = 0;
  
//   while (arduinoESPSerial.available() && arrayIndex < size) {
//     for (int i = 0; i < sizeof(float); i++) {
//       while (!arduinoESPSerial.available());  // Wait until the byte is available
//       byteBuffer[i] = arduinoESPSerial.read();
//     }
//     arr[arrayIndex] = *(float *)byteBuffer;  // Convert byte array back to float
//     arrayIndex++;
//   }
// }

// Function to print a float array
void printFloatArray(float arr[], int size) {
  for (int i = 0; i < size; i++) {
    Serial.println(arr[i], 4);  // Print each float with 4 decimal precision
  }
}

void setup() {
  // Initialize Serial Monitor (USB)
  Serial.begin(9600);
  arduinoESPSerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);

  // Initialize Blynk connection
  // Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  // Blynk.run();

  // Wait until we have enough data for the 3 arrays
  if (arduinoESPSerial.available()) {
    readFloatArray(results1, 6);  // Read the first array
    // delay(500);
    readFloatArray(results2, 6);  // Read the second array
    // delay(500);
    readFloatArray(results3, 6);  // Read the third array

    // Print the arrays in order
    Serial.println("PZEM 1:");
    printFloatArray(results1, 6);

    Serial.println("PZEM 2:");
    printFloatArray(results2, 6);

    Serial.println("PZEM 3:");
    printFloatArray(results3, 6);
    
    // delay(1000);  // Print again every 1 second
  }








  // if (arduinoESPSerial.available()) {
  //   Serial.println("Receiving data from Arduino");
  //   readFloatArray(receivedArray, 6);
  //   // Print received array for verification
  //   for (int i = 0; i < 6; i++) {
  //     Serial.println(receivedArray[i]);
  //   }
  // }

  delay(500);  // Optional delay between prints
//  
//   // Send data to Blynk with 2 decimal places
//   Blynk.virtualWrite(VPIN_VOLTAGE, String(voltage, 2));
//   Blynk.virtualWrite(VPIN_CURRENT, String(current, 2));
//   Blynk.virtualWrite(VPIN_POWER, String(power, 2));
//   Blynk.virtualWrite(VPIN_ENERGY, String(energy, 2));
//   Blynk.virtualWrite(VPIN_FREQ, String(frequency, 2));
//   Blynk.virtualWrite(VPIN_PF, String(pf, 2));  // Send PF as percentage

  // delay(1000);  // Wait for a second before the next reading
}
