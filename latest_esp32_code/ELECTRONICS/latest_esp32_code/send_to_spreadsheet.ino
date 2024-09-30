void sendDataToGoogleSheets(String webAppUrl, String fullData) {
  HTTPClient http;
  String fullURL = webAppUrl + fullData;
  Serial.println("Full URL: " + fullURL);
  http.begin(fullURL);
  int httpCode = http.GET();

  if (httpCode > 0) {
    String response = http.getString();
    Serial.println("Response: " + response);
  } else {
    Serial.println("Error in HTTP request");
  }

  http.end();
}

void print_values(float arr[6], int index) {
  Serial.println("   Values for PZEM " + String(index));
  Serial.println("Voltage: " + String(arr[0]));
  Serial.println("Current: " + String(arr[1]));
  Serial.println("Power: " + String(arr[2]));
  Serial.println("Energy: " + String(arr[3]));
  Serial.println("Frequency: " + String(arr[4]));
  Serial.println("Power Factor: " + String(arr[5]));
  Serial.println("\n");
}