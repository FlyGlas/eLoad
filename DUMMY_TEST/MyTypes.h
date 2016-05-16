#ifndef MyTypes_h
#define MyTypes_h

// typedef for digital potentiometer AD5144
typedef enum _POT_CH_
{
  POT_CH_A   = 0,
  POT_CH_B   = 1,
  POT_CH_C   = 2,
  POT_CH_D   = 3,
  POT_CH_ALL = 8
} POT_CH;

typedef enum _POT_CMD_
{
  POT_WRITE  = 2,
  POT_UPDATE = 6
} POT_CMD;


// typedef for ADC MAX523x
typedef enum _ADC_CMD_
{
  ADC_LOAD_A             = 1,
  ADC_LOAD_A_UPDATE_ALL  = 2,
  ADC_LOAD_AB_UPDATE_ALL = 3,
  ADC_UPDATE_ALL         = 4,
  ADC_LOAD_B             = 5,
  ADC_LOAD_B_UPDATE_ALL  = 6
} ADC_CMD;

#endif
