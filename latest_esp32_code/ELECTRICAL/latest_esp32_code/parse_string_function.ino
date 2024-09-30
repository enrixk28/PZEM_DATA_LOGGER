void parseString(String input) {
  int index = 0;
  int startIndex = 0;
  float tempArray[17];  // Temporary array to store the extracted float values
  String hourString;

  // Extract the hours value first
  int commaIndex = input.indexOf(',');
  if (commaIndex != -1) {
    hourString = input.substring(0, commaIndex);
    startIndex = commaIndex + 1;
  }

  Serial.println("Hour: " + hourString);

  // Iterate through the string and extract float values
  for (int i = startIndex; i <= input.length(); i++) {
    if (i == input.length() || input[i] == ',') {
      // Extract the substring representing the float value
      String floatValueString = input.substring(startIndex, i);

      // Parse the string to float and store in tempArray
      tempArray[index] = floatValueString.toFloat();
      index++;

      // Update the startIndex for the next value
      startIndex = i + 1;

      // Debugging output
      Serial.print("startIndex: ");
      Serial.print(startIndex);
      Serial.print(", i: ");
      Serial.print(i);
      Serial.print(", floatValueString: ");
      Serial.println(floatValueString);
    }
  }

  // Assign the parsed values to results1, results2, and results3 arrays
  for (int i = 0; i < 6; i++) {
    results1[i] = tempArray[i];
    results2[i] = tempArray[i + 5];
    results3[i] = tempArray[i + 11];
  }
}