
// include library
#include <SPI.h>
#include "MyTypes.h"

// pins used for the hw connection
const byte AD5144_CSn  = 7;
const byte MAX523x_CSn = 8;
const byte ADC_select  = 9;

// values of the dac channels A and B
int ADC_value_A = 0;
int ADC_value_B = 0;
boolean ADC_select_status = 0;

// array for filter setting
const int ADC_value_input[] = {  0, 200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000, 2200, 2400, 2600, 2800, 3000, 3200, 3400, 3600, 3800, 4095};
const int R_value_filter[]  = { 50,  80,  90, 100, 115,  120,  125,  140,  145,  150,  155,  160,  170,  180,  200,  220,  230,  240,  255,  255,  255};
const int R_value_control[] = { 50,  80,  90, 100, 115,  120,  125,  140,  145,  150,  155,  160,  170,  180,  200,  220,  230,  240,  255,  255,  255};


void setup()
{
  // config serial communication
  Serial.begin(9600);

  // start the SPI library
  SPI.begin();
  SPI.setDataMode(SPI_MODE1);
  SPI.setClockDivider(SPI_CLOCK_DIV2);

  // initalize the chip select pins:
  pinMode(AD5144_CSn,  OUTPUT);
  pinMode(MAX523x_CSn, OUTPUT);
  pinMode(ADC_select,  OUTPUT);

  // set pins high (low active)
  digitalWrite(AD5144_CSn,  HIGH);
  digitalWrite(MAX523x_CSn, HIGH);

  // set pins low (high active)
  digitalWrite(ADC_select, LOW);

  // set all digi pots to max. value
  AD5144(POT_CH_ALL, POT_WRITE, 255);
  AD5144(POT_CH_ALL, POT_UPDATE, 0);

  // set all adc to 0
  MAX523x(ADC_LOAD_AB_UPDATE_ALL, 0);
}


void loop() {

  SerialInputCmd();

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setValueA(int thisValue)
{
  ADC_value_A = thisValue;
  MAX523x(ADC_LOAD_A_UPDATE_ALL, ADC_value_A);
  Serial.print("Set value A: ");
  Serial.println(ADC_value_A);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setValueB(int thisValue)
{
  ADC_value_B = thisValue;
  MAX523x(ADC_LOAD_B_UPDATE_ALL, ADC_value_B);
  Serial.print("Set value B: ");
  Serial.println(ADC_value_B);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setNextValue(int thisValue)
{
  if (digitalRead(ADC_select) == true) // with ADC_select = true channel A is active
  { // with ADC_select = false channel B is active
    setValueB(thisValue);
  }
  else
  {
    setValueA(thisValue);
  }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void toggleChannel()
{
  // set filter values for the switch between the channels
  if (digitalRead(ADC_select) == true)
  {
    setFilter(ADC_value_B);
  }
  else
  {
    setFilter(ADC_value_A);
  }

  Serial.println("Toggle channel...");

  // perform switch between the channels
  digitalWrite(ADC_select, !digitalRead(ADC_select));
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setFilter(int thisValue)
{
  //

  int thisValueFilter  = multiMap(thisValue, ADC_value_input, R_value_filter,  sizeof(ADC_value_input));
  int thisValueControl = multiMap(thisValue, ADC_value_input, R_value_control, sizeof(ADC_value_input));

  Serial.print("Set filter value to ");
  Serial.print(thisValueFilter);
  Serial.print(" and control value to ");
  Serial.print(thisValueControl);
  Serial.print(" for new output value ");
  Serial.print(thisValue);
  Serial.println("...");


  AD5144(POT_CH_A, POT_WRITE, thisValueFilter);
  AD5144(POT_CH_B, POT_WRITE, thisValueControl);
  AD5144(POT_CH_ALL, POT_UPDATE, 0);
}



void test(int thisValue)
{
  Serial.println(multiMap(thisValue, ADC_value_input, R_value_filter,  sizeof(ADC_value_input)));
  Serial.println(multiMap(thisValue, ADC_value_input, R_value_control, sizeof(ADC_value_input)));
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// note: the _in array should have increasing values
// URL: http://playground.arduino.cc/Main/MultiMap
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int multiMap(int val, const int* _in, const int* _out, uint8_t size)
{
  // take care the value is within range
  // val = constrain(val, _in[0], _in[size-1]);
  if (val <= _in[0]) return _out[0];
  if (val >= _in[size - 1]) return _out[size - 1];

  // search right interval
  uint8_t pos = 1;  // _in[0] allready tested
  while (val > _in[pos]) pos++;

  // this will handle all exact "points" in the _in array
  if (val == _in[pos]) return _out[pos];

  // interpolate in the right segment for the rest
  return (val - _in[pos - 1]) * (_out[pos] - _out[pos - 1]) / (_in[pos] - _in[pos - 1]) + _out[pos - 1];
}



//  AD5144(POT_CH_A, POT_WRITE, 50);
//  AD5144(POT_CH_B, POT_WRITE, 100);
//  AD5144(POT_CH_C, POT_WRITE, 150);
//  AD5144(POT_CH_D, POT_WRITE, 200);
//  AD5144(POT_CH_ALL, POT_UPDATE, 0);
//
//  MAX523x(ADC_LOAD_A_UPDATE_ALL, 1000);
//  MAX523x(ADC_LOAD_B_UPDATE_ALL, 3000);
//  delay(2500);
//
//
//  AD5144(POT_CH_ALL, POT_WRITE, 0);
//  AD5144(POT_CH_ALL, POT_UPDATE, 0);
//
//  MAX523x(ADC_LOAD_A, 2000);
//  MAX523x(ADC_LOAD_B, 4000);
//  MAX523x(ADC_UPDATE_ALL, 0);
//  delay(2500);

