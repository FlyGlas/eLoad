//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This function is used to set the value of the MAX523x DAC
// INPUTS: Command, Value (0-4095)
// OUTPUT: void
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MAX523x(ADC_CMD thisCommand, int thisValue) {

  // Change SPI MODE
  SPI.setDataMode(SPI_MODE0);

  // MAX523x expects the command as the upper 3 bits of the MSB
  // and the DAC value as the 5 lower bits of the MSB in combination with the 7 upper bits of the LSB.
  // The lowest bit of the LSB is always 0.
  // Shift the bits to the right position:
  byte dataToSend_msb = ((thisCommand & 0b00000111) << 5) | ((thisValue & 0b0000111110000000) >> 7);
  //Serial.println(dataToSend_msb, BIN);
  byte dataToSend_lsb = (thisValue & 0b0000000001111111) << 1;
  //Serial.println(dataToSend_lsb, BIN);

  // take the chip select low to select the device
  digitalWrite(MAX523x_CSn, LOW);

  // transfer the MSB and LSB
  SPI.transfer(dataToSend_msb);
  SPI.transfer(dataToSend_lsb);

  // take the chip select high to de-select
  digitalWrite(MAX523x_CSn, HIGH);

  //Serial.print("New value: ");
  //Serial.println(thisValue);
}


