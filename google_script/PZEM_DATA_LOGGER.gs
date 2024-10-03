function doGet(e) {
  Logger.log(JSON.stringify(e));
  var result = 'Ok';
  if (e.parameter == 'undefined') {
    result = 'No Parameters';
  }
  else {
    var sheet_id = '1PjI2aHKEletrjGqA2eQCanaIvnrz10zWkDVelOTWnlA';  // Spreadsheet ID.
    var sheet_name1 = "PZEM Reading EE";  // Sheet Name for PZEM sensors in Electrical Hallway.
    // var sheet_name2 = "PZEM Reading ECE";  // Sheet Name for PZEM sensors in Electronics Hallway.

    var sheet_open = SpreadsheetApp.openById(sheet_id);
    var sheet_target1 = sheet_open.getSheetByName(sheet_name1);
    // var sheet_target2 = sheet_open.getSheetByName(sheet_name2);

    var newRow_EE = sheet_target1.getLastRow() + 1;
    // var newRow_ECE = sheet_target2.getLastRow() + 1;

    // Collect data for each sensor
    var pzem1Data = [];
    var pzem2Data = [];
    var pzem3Data = [];

    // Date and time for all sensors
    var Curr_Date = Utilities.formatDate(new Date(), "Asia/Manila", 'dd/MM/yyyy');
    var Curr_Time = Utilities.formatDate(new Date(), "Asia/Manila", 'HH:mm:ss');

    // Fill date and time in the arrays for each sensor
    pzem1Data.push(Curr_Date, Curr_Time);
    pzem2Data.push(Curr_Date, Curr_Time);
    pzem3Data.push(Curr_Date, Curr_Time);

    var sts_val = '';

    for (var param in e.parameter) {
      Logger.log('In for loop, param=' + param);
      var value = stripQuotes(e.parameter[param]);
      Logger.log(param + ':' + e.parameter[param]);

      switch (param) {
        // PZEM 1 Data
        case 'pzem1_voltage':
          pzem1Data[2] = value; // Voltage (PZEM1)
          break;
        case 'pzem1_current':
          pzem1Data[3] = value; // Current (PZEM1)
          break;
        case 'pzem1_power':
          pzem1Data[4] = value; // Power (PZEM1)
          break;
        case 'pzem1_energy':
          pzem1Data[5] = value; // Energy (PZEM1)
          break;
        case 'pzem1_frequency':
          pzem1Data[6] = value; // Frequency (PZEM1)
          break;
        case 'pzem1_pf':
          pzem1Data[7] = value; // Power Factor (PZEM1)
          break;

        // PZEM 2 Data
        case 'pzem2_voltage':
          pzem2Data[2] = value; // Voltage (PZEM2)
          break;
        case 'pzem2_current':
          pzem2Data[3] = value; // Current (PZEM2)
          break;
        case 'pzem2_power':
          pzem2Data[4] = value; // Power (PZEM2)
          break;
        case 'pzem2_energy':
          pzem2Data[5] = value; // Energy (PZEM2)
          break;
        case 'pzem2_frequency':
          pzem2Data[6] = value; // Frequency (PZEM2)
          break;
        case 'pzem2_pf':
          pzem2Data[7] = value; // Power Factor (PZEM2)
          break;

        // PZEM 3 Data
        case 'pzem3_voltage':
          pzem3Data[2] = value; // Voltage (PZEM3)
          break;
        case 'pzem3_current':
          pzem3Data[3] = value; // Current (PZEM3)
          break;
        case 'pzem3_power':
          pzem3Data[4] = value; // Power (PZEM3)
          break;
        case 'pzem3_energy':
          pzem3Data[5] = value; // Energy (PZEM3)
          break;
        case 'pzem3_frequency':
          pzem3Data[6] = value; // Frequency (PZEM3)
          break;
        case 'pzem3_pf':
          pzem3Data[7] = value; // Power Factor (PZEM3)
          break;

        case 'sts':
          sts_val = value;
          break;

        default:
          result += ", unsupported parameter";
      }
    }

    // Writing data to Google Sheets
    if (sts_val == 'write') {
      // Write PZEM 1 Data
      var newRangePzem1 = sheet_target1.getRange(newRow_EE, 1, 1, pzem1Data.length);  // Starting at column A
      newRangePzem1.setValues([pzem1Data]);

      // Write PZEM 2 Data
      var newRangePzem2 = sheet_target1.getRange(newRow_EE, 9, 1, pzem2Data.length);  // Starting at column I
      newRangePzem2.setValues([pzem2Data]);

      // Write PZEM 3 Data
      var newRangePzem3 = sheet_target1.getRange(newRow_EE, 17, 1, pzem3Data.length);  // Starting at column Q
      newRangePzem3.setValues([pzem3Data]);

      result = pzem1Data + pzem2Data + pzem3Data;
      return ContentService.createTextOutput(result);
    }

    // Conditions for sending data back to ESP32 (if needed)
    if (sts_val == 'read') {
      // Logic to send the data back (if you need to implement it)
    }
  }
}

function stripQuotes(value) {
  return value.replace(/^["']|['"]$/g, "");
}
