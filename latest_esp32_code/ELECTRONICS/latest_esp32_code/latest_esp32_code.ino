#define BLYNK_PRINT Serial // For debugging
#define BLYNK_TEMPLATE_ID "TMPL6x0IEWhH"
#define BLYNK_TEMPLATE_NAME "THESIS"
#define BLYNK_AUTH_TOKEN "XS0ZKMIPYW87s7sp69qeZJEliXgraNfx"  // Electronics

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
#define VPIN_VOLTAGE1 V0
#define VPIN_CURRENT1 V1
#define VPIN_POWER1 V2
#define VPIN_ENERGY1 V3
#define VPIN_FREQ1 V4
#define VPIN_PF1 V5

#define VPIN_VOLTAGE2 V6
#define VPIN_CURRENT2 V7
#define VPIN_POWER2 V8
#define VPIN_ENERGY2 V9
#define VPIN_FREQ2 V10
#define VPIN_PF2 V11

#define VPIN_VOLTAGE3 V12
#define VPIN_CURRENT3 V13
#define VPIN_POWER3 V14
#define VPIN_ENERGY3 V15
#define VPIN_FREQ3 V16
#define VPIN_PF3 V17

#define VPIN_PHASE V18

// Create a HardwareSerial object for Serial2
HardwareSerial arduinoESPSerial(1);

// Your WiFi credentials
const char *ssid = "";
const char *pass = "";

String webAppUrl = "";
String fullData;
String hourString; 

float results1[6];
float results2[6];
float results3[6];

const int maxFloats = 18;     // Maximum number of floats to extract
float floatArray[maxFloats];  // Array to store the extracted floats
int floatCount = 0;           // Counter for the number of floats extracted
int selectedLine = 0;

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

  // // Check for SD card Status
  // #ifdef REASSIGN_PINS
  //   SPI.begin(sck, miso, mosi, cs);
  //   if (!SD.begin(cs)) {
  // #else
  //   if (!SD.begin()) {
  // #endif
  //     Serial.println("Card Mount Failed");
  //     return;
  //   }

  // uint8_t cardType = SD.cardType();

  // if (cardType == CARD_NONE) {
  //   Serial.println("No SD card attached");
  //   return;
  // }

  // Serial.print("SD Card Type: ");
  // if (cardType == CARD_MMC) {
  //   Serial.println("MMC");
  // } else if (cardType == CARD_SD) {
  //   Serial.println("SDSC");
  // } else if (cardType == CARD_SDHC) {
  //   Serial.println("SDHC");
  // } else {
  //   Serial.println("UNKNOWN");
  // }

  // uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  // Serial.printf("SD Card Size: %lluMB\n", cardSize);

  // // Assign Headers for PZEM Sensor readings
  // String pzemHeaders =
  //   "Duration,"
  //   "Data Logged from PZEM022 Sensor 1,,,,,, "
  //   "Data Logged from PZEM022 Sensor 2,,,,,, "
  //   "Data Logged from PZEM022 Sensor 3,,,,,\n"
  //   "Hour (h),"
  //   "Voltage (V), Current (A), Power (W), Energy (Wh), Frequency (Hz), Power Factor (PF), "
  //   "Voltage (V), Current (A), Power (W), Energy (Wh), Frequency (Hz), Power Factor (PF), "
  //   "Voltage (V), Current (A), Power (W), Energy (Wh), Frequency (Hz), Power Factor (PF)\n";

  // // Check and create file for ECE readings
  // if (!SD.exists("/pzemReadings_ECE.csv")) {
  //   writeFile(SD, "/pzemReadings_ECE.csv", pzemHeaders.c_str());
  // }

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

    // appendFile(SD, "/pzemReadings_ECE.csv", (data + "\n").c_str());

    // Send data to Blynk with 2 decimal places
    Blynk.beginGroup();
    Blynk.virtualWrite(VPIN_VOLTAGE1, String(results1[0], 2));
    Blynk.virtualWrite(VPIN_CURRENT1, String(results1[1], 2));
    Blynk.virtualWrite(VPIN_POWER1, String(results1[2], 2));
    Blynk.virtualWrite(VPIN_ENERGY1, String(results1[3], 2));
    Blynk.virtualWrite(VPIN_FREQ1, String(results1[4], 2));
    Blynk.virtualWrite(VPIN_PF1, String(results1[5], 2));  // Send PF as percentage
    Blynk.endGroup();

    Blynk.beginGroup();
    Blynk.virtualWrite(VPIN_VOLTAGE2, String(results2[0], 2));
    Blynk.virtualWrite(VPIN_CURRENT2, String(results2[1], 2));
    Blynk.virtualWrite(VPIN_POWER2, String(results2[2], 2));
    Blynk.virtualWrite(VPIN_ENERGY2, String(results2[3], 2));
    Blynk.virtualWrite(VPIN_FREQ2, String(results2[4], 2));
    Blynk.virtualWrite(VPIN_PF2, String(results2[5], 2));  // Send PF as percentage
    Blynk.endGroup();

    Blynk.beginGroup();
    Blynk.virtualWrite(VPIN_VOLTAGE3, String(results3[0], 2));
    Blynk.virtualWrite(VPIN_CURRENT3, String(results3[1], 2));
    Blynk.virtualWrite(VPIN_POWER3, String(results3[2], 2));
    Blynk.virtualWrite(VPIN_ENERGY3, String(results3[3], 2));
    Blynk.virtualWrite(VPIN_FREQ3, String(results3[4], 2));
    Blynk.virtualWrite(VPIN_PF3, String(results3[5], 2));  // Send PF as percentage
    Blynk.endGroup();
  }
}