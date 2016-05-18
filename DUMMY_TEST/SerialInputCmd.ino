//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Serial data INPUT
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void  SerialInputCmd()
{
  char serial_data [21];               // hold incoming data
  int number_of_bytes_received = 0;    // number of received bytes
  boolean new_serial_input = false;    // whether the string is complete
  boolean true_input = false;

  if (Serial.available() > 1)
  {
    number_of_bytes_received = Serial.readBytesUntil(13, serial_data, 20); // read bytes (max. 20) from buffer, untill <CR> (13). store bytes in data. count the bytes recieved.
    new_serial_input = true;
    serial_data[number_of_bytes_received] = 0; // add a 0 terminator to the char array
  }
  else
  {
    new_serial_input = false;
  }


  if (new_serial_input == true)
  {
    if (string_contains(serial_data, "sva="))
    {
      true_input = true;
      setValueA(extract_value(serial_data));
    }
    if (string_contains(serial_data, "svb="))
    {
      true_input = true;
      setValueB(extract_value(serial_data));
    }
    if (string_contains(serial_data, "snv="))
    {
      true_input = true;
      setNextValue(extract_value(serial_data));
    }
    if (string_contains(serial_data, "tgl"))
    {
      true_input = true;
      toggleChannel();
    }
    if (string_contains(serial_data, "test"))
    {
      true_input = true;
      test(extract_value(serial_data));
    }

    if (true_input == false)
    {
      Serial.println("Invalid command!");
    }
  }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// extract int value from string (used in serial input)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int extract_value(String thisString) {
  int max = thisString.length() - 1; // the searchstring has to fit in the other one

  for (int i = 0; i <= max; i++)
  {
    if (thisString.substring(i, i + 1) == "=") {
      return thisString.substring(i + 1, thisString.length()).toInt();
    }
  }
  return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// searches in strings, true when found (used in serial input)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

boolean string_contains(String thisString, String thisSearch) {
  int max = thisString.length() - thisSearch.length(); // the searchstring has to fit in the other one
  for (int i = 0; i <= max; i++)
  {
    if (thisString.substring(i, i + thisSearch.length()) == thisSearch)
      return true;
  }
  return false; //or -1
}


