
// inslude the SPI library:
#include <SPI.h>

// pins used for the connection with the DAC
const int MAX523x_CSn   = 2;
const int MAX523x_CLRn  = 7;
const int MAX523x_LDACn = 8;

void setup() {

  // config serial communication
  Serial.begin(9600);

  // start the SPI library:
  SPI.begin();

  // initalize the chip select pin, clear pin and load DACs pin:
  pinMode(MAX523x_CSn, OUTPUT);
  pinMode(MAX523x_CLRn, OUTPUT);
  pinMode(MAX523x_LDACn, OUTPUT);

  // set pins high (low active)
  digitalWrite(MAX523x_CSn, HIGH);
  digitalWrite(MAX523x_CLRn, HIGH);
  digitalWrite(MAX523x_LDACn, HIGH);
}


void loop() {

  for (int i = 0; i < 4096; i += 1) {
    MAX523x(0b001, i);
    MAX523x(0b101, 4095 - i);
    //MAX523x(0b100, 0);
    digitalWrite(MAX523x_LDACn, LOW);
    digitalWrite(MAX523x_LDACn, HIGH);
  }
  digitalWrite(MAX523x_CLRn, LOW);
  digitalWrite(MAX523x_CLRn, HIGH);
  delay(100);

  // print heart in x-y-mode of scope
  //  for (float i = -1.0f; i < 1.0f; i += 0.01f) {
  //    MAX523x(0b001, (int)(3000.0f*((-3.0f*i*i+2.0f*abs(i)+1.0f)*sin(i)+0.7)));
  //    MAX523x(0b101, (int)(3000.0f*(-3.0f*i*i+2.0f*abs(i)+1.0f)*cos(i)));
  //    MAX523x(0b100, 0);
  //}
}



void MAX523x(byte thisCommand, int thisValue) {

  //0b000 and 0b111 are reserved for special function
  if (thisCommand != 0b000 && thisCommand != 0b111)
  {
    // MAX523x expects the command as the upper 3 bits of the MSB
    // and the DAC value as the 5 lower bits of the MSB in combination with the 7 upper bits of the LSB.
    // The lowest bit of the LSB is always 0.
    // So shift the bits to the right position:
    byte dataToSend_msb = ((thisCommand & 0b00000111) << 5) | ((thisValue & 0b0000111110000000) >> 7);
    byte dataToSend_lsb = (thisValue & 0b0000000001111111) << 1;

    // take the chip select low to select the device:
    digitalWrite(MAX523x_CSn, LOW);

    // transfer the MSB and LSB:
    SPI.transfer(dataToSend_msb);
    SPI.transfer(dataToSend_lsb);

    // take the chip select high to de-select:
    digitalWrite(MAX523x_CSn, HIGH);
  }
}
