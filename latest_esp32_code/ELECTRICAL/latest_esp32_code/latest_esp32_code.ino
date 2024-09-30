#define BLYNK_PRINT Serial // For debugging
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""  // Electrical

// Fundamental Libraries used in this code
#include "FS.h" // Library for File Manipulation
#include "SD.h" // Library for SD card
#include "SPI.h" // Communication Protocol for SD card
#include <Arduino.h> 
#include <WiFi.h> 
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <BlynkSimpleEsp32.h>

// Define the pins for Serial2
#define RX_PIN 16  // GPIO 16 for RX
#define TX_PIN 17  // GPIO 17 for TX

// Blynk Virtual Pins for monitoring
#define VPIN_VOLTAGE V0
#define VPIN_CURRENT V1
#define VPIN_POWER V2
#define VPIN_ENERGY V3
#define VPIN_FREQ V4
#define VPIN_PF V5

// Create a HardwareSerial object for Serial2
HardwareSerial arduinoESPSerial(1);

// Your WiFi credentials
const char *ssid = "";
const char *pass = "";

String webAppUrl = "https://script.google.com/macros/s/AKfycbxz4uQqxDnlH5b_beEEaAmhYGL-hbfuyJhzdRg1cSTLYzzVLVL3LsFc5zzt4Gcl66a8/exec";
String fullData;
String hourString; 

float results1[6];
float results2[6];
float results3[6];

const int maxFloats = 18;     // Maximum number of floats to extract
float floatArray[maxFloats];  // Array to store the extracted floats
int floatCount = 0;           // Counter for the number of floats extracted

String convertValuesToString(float arr[6], int index) {
  String dataString;
  String choices[6] = { "voltage", "current", "power", "energy", "frequency", "pf" };
  for (int i = 0; i < 6; i++) {
    dataString += "&";
    dataString += (String) "pzem" + index + "_" + choices[i] + "=" + arr[i];
  }
  return dataString;
}

String compileData(float arr1[6], float arr2[6], float arr3[6], String fullData) {
  fullData += convertValuesToString(arr1, 1);
  fullData += convertValuesToString(arr2, 2);
  fullData += convertValuesToString(arr3, 3);
  return fullData;
}

//Function prototype for seperated files
void print_values(float arr[6], int index);
void sendDataToGoogleSheets(String webAppUrl, String fullData);
void writeFile(fs::FS &fs, const char *path, const char *message);
void appendFile(fs::FS &fs, const char *path, const char *message);
void parseString(String input);

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, pass);

  // Initialize Blynk connection
  Serial.println("Initializing Blynk");
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Serial.println("Blynk Initialized");

  arduinoESPSerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Check for SD card Status
  #ifdef REASSIGN_PINS
    SPI.begin(sck, miso, mosi, cs);
    if (!SD.begin(cs)) {
  #else
    if (!SD.begin()) {
  #endif
      Serial.println("Card Mount Failed");
      return;
    }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);

  // Assign Headers for PZEM Sensor readings
  String pzemHeaders =
    "Duration,"
    "Data Logged from PZEM022 Sensor 1,,,,,, "
    "Data Logged from PZEM022 Sensor 2,,,,,, "
    "Data Logged from PZEM022 Sensor 3,,,,,\n"
    "Hour (h),"
    "Voltage (V), Current (A), Power (W), Energy (Wh), Frequency (Hz), Power Factor (PF), "
    "Voltage (V), Current (A), Power (W), Energy (Wh), Frequency (Hz), Power Factor (PF), "
    "Voltage (V), Current (A), Power (W), Energy (Wh), Frequency (Hz), Power Factor (PF)\n";

  // Check and create file for EE readings
  if (!SD.exists("/pzemReadings_EE.csv")) {
    writeFile(SD, "/pzemReadings_EE.csv", pzemHeaders.c_str());
  }

}

void loop() {
  // Serial.println("Running Blynk");
  Blynk.run();
  // Serial.println("Waiting for Data...");
  if (arduinoESPSerial.available()) {
    Serial.print("\033[0H\033[0J");  // Deletes previous prints in PuTTY Serial Monitor
    Serial.println("Data Received");
    
    String data = arduinoESPSerial.readString();
    parseString(data);
    Serial.println("Data Parsed:" + data);

    print_values(results1, 1);
    print_values(results2, 2);
    print_values(results3, 3);

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("WiFi Connected Successfully");
      String fullData = "?sts=write";
      fullData = compileData(results1, results2, results3, fullData);
      sendDataToGoogleSheets(webAppUrl, fullData);
    }

    appendFile(SD, "/pzemReadings_EE.csv", (data + "\n").c_str());

    // Send data to Blynk with 2 decimal places
    Blynk.virtualWrite(VPIN_VOLTAGE, String(results1[0], 2));
    Blynk.virtualWrite(VPIN_CURRENT, String(results1[1], 2));
    Blynk.virtualWrite(VPIN_POWER, String(results1[2], 2));
    Blynk.virtualWrite(VPIN_ENERGY, String(results1[3], 2));
    Blynk.virtualWrite(VPIN_FREQ, String(results1[4], 2));
    Blynk.virtualWrite(VPIN_PF, String(results1[5], 2));  // Send PF as percentage
  }
}


