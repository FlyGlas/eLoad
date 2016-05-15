//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This function is used to set the value of the AD5144 digital potentiometer
// INPUTS: Channel, Command, Value (0-255)
// OUTPUT: void
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AD5144(POT_CH thisChannel, POT_CMD thisCommand, byte thisValue) {

  // Change SPI MODE
  SPI.setDataMode(SPI_MODE1);

  // Combine command and channel information
  byte dataToSend_msb = ((thisCommand & 0b00001111) << 4) | (thisChannel & 0b00001111);
  //Serial.println(dataToSend_msb, BIN);
  byte dataToSend_lsb = thisValue;
  //Serial.println(dataToSend_lsb, BIN);
  
  // take the chip select low to select the device
  digitalWrite(AD5144_CSn, LOW);

  // transfer the MSB and LSB
  SPI.transfer(dataToSend_msb);
  SPI.transfer(dataToSend_lsb);

  // take the chip select high to de-select
  digitalWrite(AD5144_CSn, HIGH);
}


