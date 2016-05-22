
// include library
#include <SPI.h>
#include "MyTypes.h"

// pins used for the hw connection
byte AD5144_CSn  = 7;
byte MAX523x_CSn = 8;
byte ADC_select  = 9;

// values of the dac channels A and B
int ADC_value_A = 0;
int ADC_value_B = 0;
boolean ADC_select_status = false;

// array for filter setting
int ADC_value_input[] = {  0, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1600, 1800, 2000, 2200, 2400, 2600, 2800, 3000, 3200, 3400, 3600, 3800, 4095};
int R_value_filter[]  = { 20,  30,  40,  50,  60,  80, 100, 120, 140, 160,  180,  200,  220,  240,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255};
int R_value_control[] = { 50,  60, 100, 120, 150, 180, 240, 250, 255, 255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255};


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
  ADC_select_status = false;
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
  if (ADC_select_status == true) // with ADC_select_status == true channel A is active
  { // with ADC_select_status == false channel B is active
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
  if (ADC_select_status == true)
  {
    if (ADC_value_B > ADC_value_A)
    {
      setFilter(ADC_value_B, JUMP_UP);
    }
    else
    {
      // Negative step
      setFilter(50, JUMP_DOWN);
    }
  }
  else
  {
    if (ADC_value_A > ADC_value_B)
    {
      setFilter(ADC_value_A, JUMP_UP);
    }
    else
    {
      // Negative step
      setFilter(50, JUMP_DOWN);
    }
  }

  Serial.println("Toggle channel...");
  delay(50);

  // perform switch between the channels
  ADC_select_status = !ADC_select_status;
  digitalWrite(ADC_select, ADC_select_status);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setFilter(int thisValue, JUMP_DIRECTION thisDirection)
{
  //
  if (thisDirection == JUMP_UP)
  {
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
    AD5144(POT_CH_B, POT_WRITE, thisValueFilter);
    AD5144(POT_CH_C, POT_WRITE, thisValueFilter);
    AD5144(POT_CH_D, POT_WRITE, thisValueControl);
    AD5144(POT_CH_ALL, POT_UPDATE, 0);
  }
  else if (thisDirection == JUMP_DOWN)
  {

    Serial.print("Negative step, adjust filter value to ");
    Serial.print(thisValue);
    Serial.println("...");

    AD5144(POT_CH_A, POT_WRITE, thisValue);
    AD5144(POT_CH_B, POT_WRITE, thisValue);
    AD5144(POT_CH_C, POT_WRITE, thisValue);
    AD5144(POT_CH_ALL, POT_UPDATE, 0);
  }
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

int multiMap(int val, int* _in, int* _out, uint8_t size)
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

