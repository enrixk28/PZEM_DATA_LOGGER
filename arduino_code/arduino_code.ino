#include <SoftwareSerial.h>
#include <PZEM004Tv30.h>

// Define SoftwareSerial objects for each PZEM
SoftwareSerial pzemSerial1(2, 3);  // RX, TX for PZEM1
SoftwareSerial pzemSerial2(4, 5);  // RX, TX for PZEM2
SoftwareSerial pzemSerial3(6, 7);  // RX, TX for PZEM3
SoftwareSerial arduinoESPSerial(8, 9); // RX, TX for Arduino to ESP Communication

// Initialize PZEM objects for each module
PZEM004Tv30 pzem1(pzemSerial1);
PZEM004Tv30 pzem2(pzemSerial2);
PZEM004Tv30 pzem3(pzemSerial3);

// Calibration factors
float currentCalibrationFactor = 0.5;  // Calibration factor for current (Amperes)
float voltageCalibrationFactor = 1.0;  // Calibration factor for voltage (Volts)
float powerCalibrationFactor = 1.0;    // Calibration factor for power (Watts)
float energyCalibrationFactor = 1.5;   // Calibration factor for energy (kWh)
float pfCalibrationFactor = 1.0;     // Calibration factor for power factor (as a percentage)

// Interval for reading values (1 hour in milliseconds)
// const unsigned long interval = 360000;
const unsigned long interval = 12000; // temporary interval ( 5 seconds )
unsigned long previousMillis = 0;

// // Read values from PZEM004T
// void read_values(PZEM004Tv30 pzem, float results[6]) {
//     results[0] = pzem.voltage() * voltageCalibrationFactor;
//     results[1] = pzem.current() * currentCalibrationFactor;
//     results[2] = pzem.power() * powerCalibrationFactor;
//     results[3] = pzem.energy() * energyCalibrationFactor;
//     results[4] = pzem.frequency();
//     results[5] = pzem.pf() * pfCalibrationFactor;
// }

// Read values from PZEM004T
void read_values(PZEM004Tv30 pzem, float results[6]) {
    results[0] = 250.00;
    results[1] = 0.04;
    results[2] = 169.00;
    results[3] = 0.04;
    results[4] = 60.00;
    results[5] = 0.04;
}

// Print results
void print_values(float results[6]) {
  if (!isnan(results[0])) {
    Serial.println((String)"Voltage : "+results[0]+" V");
    Serial.println((String)"Current : "+results[1]+" A");
    Serial.println((String)"Power : "+results[2]+" W");
    Serial.println((String)"Energy : "+results[3]+" Wh");
    Serial.println((String)"Frequency : "+results[4]+" Hz");
    Serial.println((String)"Power Factor : "+results[5]+" PF");
  } else {
    Serial.println("Error reading parameters from PZEM");
  }
  Serial.println("\n");
}

// Function to send float array over SoftwareSerial
void sendFloatArray(float arr[], int size) {
  String dataToSend;
  for (int i = 0; i < size; i++) {
    dataToSend += String(arr[i], 2) + ",";
  }
  Serial.println("Data To Send: "+dataToSend);
  arduinoESPSerial.print(dataToSend);
}

void setup() {
  // Initialize Serial for debugging
  Serial.begin(9600);

  // Start SoftwareSerial for each PZEM module
  pzemSerial1.begin(9600);
  pzemSerial2.begin(9600);
  pzemSerial3.begin(9600);
  arduinoESPSerial.begin(9600);
}

void loop() {
  // Time when device starts running
  unsigned long currentMillis = millis();
  // unsigned long hours = currentMillis / 1000 / 60 / 60; // 1 hour conversion
  unsigned long hours = currentMillis / 1000; // 1 second conversion

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    Serial.print("\033[0H\033[0J"); // Deletes previous prints in PuTTY Serial Monitor

    // Read data from PZEM1
    Serial.println("Reading from PZEM1");
    float results1[6];
    read_values(pzem1, results1);
    print_values(results1);

    // Read data from PZEM2
    Serial.println("Reading from PZEM2");
    float results2[6];
    read_values(pzem2, results2);
    print_values(results2);

    // Read data from PZEM3
    Serial.println("Reading from PZEM3");
    float results3[6];
    read_values(pzem3, results3);
    print_values(results3);

  //  Send data to ESP32 using SoftwareSerial UART 
  //  Serial.println("Sending Data Test");
  //  arduinoESPSerial.write("Hello");
    String hourString = String(hours);
    Serial.println("Duration: " + hourString + " h");
    arduinoESPSerial.print(hourString + ",");

    sendFloatArray(results1, 6);
    sendFloatArray(results2, 6);
    sendFloatArray(results3, 6);
  }
}
