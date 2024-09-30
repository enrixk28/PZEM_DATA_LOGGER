#define BLYNK_TEMPLATE_ID "TMPL6x0IEVVhH"
#define BLYNK_TEMPLATE_NAME "AMBOT"
#define BLYNK_AUTH_TOKEN "NzTVNknbve7C7HixsTzsc0-ih0Zom0Oy"

#include <WiFi.h>
#include <SoftwareSerial.h>
#include <PZEM004Tv30.h>
#include <BlynkSimpleEsp32.h>

// Define the pins for Serial2
#define RX_PIN 22  // GPIO 16 for RX
#define TX_PIN 23  // GPIO 17 for TX

// Create a HardwareSerial object for Serial2
HardwareSerial mySerial(1);
// SoftwareSerial mySerial(22, 23);

// Initialize PZEM-004T with Serial2
PZEM004Tv30 pzem(mySerial, RX_PIN, TX_PIN);

// Your WiFi credentials
char ssid[] = "TUP_WiFi";
char pass[] = "ilovetup";

// Blynk Virtual Pins for monitoring
#define VPIN_VOLTAGE V0
#define VPIN_CURRENT V1
#define VPIN_POWER V2
#define VPIN_ENERGY V3
#define VPIN_FREQ V4
#define VPIN_PF V5

// Calibration factors
float currentCalibrationFactor = 0.5;  // Calibration factor for current (Amperes)
float voltageCalibrationFactor = 1.0;  // Calibration factor for voltage (Volts)
float powerCalibrationFactor = 1.0;    // Calibration factor for power (Watts)
float energyCalibrationFactor = 1.5;   // Calibration factor for energy (kWh)
float pfCalibrationFactor = 1.0;     // Calibration factor for power factor (as a percentage)

void setup() {
  // Initialize Serial Monitor (USB)
  Serial.begin(115200);

  // Initialize the hardware serial port (Serial2)
  mySerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);  // Using GPIO 16 (RX2) and GPIO 17 (TX2) for Serial2

  // Initialize Blynk connection
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();

  // Read and calibrate the values
  float voltage = pzem.voltage() * voltageCalibrationFactor;
  float current = pzem.current() * currentCalibrationFactor;
  float power = pzem.power() * powerCalibrationFactor;
  float energy = pzem.energy() * energyCalibrationFactor;
  float frequency = pzem.frequency();          // Read frequency
  float pf = pzem.pf() * pfCalibrationFactor;  // Read and calibrate power factor to percentage

  // Print the calibrated values to Serial Monitor
  Serial.print("Voltage: ");
  Serial.print(voltage, 2);  // Print with 2 decimal places
  Serial.println(" V");

  Serial.print("Current: ");
  Serial.print(current, 2);  // Print with 2 decimal places
  Serial.println(" A");

  Serial.print("Power: ");
  Serial.print(power, 2);  // Print with 2 decimal places
  Serial.println(" W");

  Serial.print("Energy: ");
  Serial.print(energy, 2);  // Print with 2 decimal places
  Serial.println(" kWh");

  Serial.print("Frequency: ");
  Serial.print(frequency, 2);  // Print with 2 decimal places
  Serial.println(" Hz");

  Serial.print("Power Factor: ");
  Serial.print(pf, 2);  // Print with 2 decimal places
  Serial.println(" %");

  // Send data to Blynk with 2 decimal places
  Blynk.virtualWrite(VPIN_VOLTAGE, String(voltage, 2));
  Blynk.virtualWrite(VPIN_CURRENT, String(current, 2));
  Blynk.virtualWrite(VPIN_POWER, String(power, 2));
  Blynk.virtualWrite(VPIN_ENERGY, String(energy, 2));
  Blynk.virtualWrite(VPIN_FREQ, String(frequency, 2));
  Blynk.virtualWrite(VPIN_PF, String(pf, 2));  // Send PF as percentage

  delay(1000);  // Wait for a second before the next reading
}
