/*
 MCP3901 Test Read
 Frank Maker
 04/28/12
 
 This example controls an Microchip MCP3901 16-bit ADC
 
 The MCP3901 has a SPI interface with the following settings:
 * MSB first
 * CPOL = 0, CPHA = 0 (MODE = 0)
  - SCK low when idle
  - Latch on rising edge

  -------------------------------------
  | A6 | A5 | A4 | A3 | A2 | A1 | R/W |
  -------------------------------------
  |         |                   |     |
  | Device  | Register Address  | R/W |


  Arduino Mega -> MCP3901 Evaluation Board connections:
  * RESET -> 48
  * SDO -> 50
  * SDI -> 51
  * SCK -> 52
  * CS -> 53

  NOTE: These settings are defaults so no configuration is necessary
  Both ADC channels can be read back continously by setting
  the STATUS/COM register
  * READ<1:0> = 0b10 // Loops on register types (just ADCs)
  * DRMODE<1:0> = 0b00 // DR signal when both ADCs are done

*/

#include <SPI.h>
#include <math.h>

/* Arduino */
#define	 RESET	48
#define	 CS	53
#define  CHAR_BIT	8

/* MCP3901 */
#define  BIT_16 0xFFFF
#define  NUM_CHAN    2
#define  WIDTH    2
#define  VREF    2.37F
#define  BITS    16
#define  GAIN    1
#define  SINC_ORDER    3

/* Global */
float volt_per_cnt;

void setup()
{

  // Setup serial
  Serial.begin(9600);
  Serial.println("MCP3901 Test v0.1");

  // Setup reset
  pinMode(RESET, OUTPUT);
  digitalWrite(RESET, LOW);
  digitalWrite(RESET, HIGH);

  // Setup SPI
  pinMode(CS, OUTPUT);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV128); /* Slowest clock for testing */
  SPI.begin();

  volt_per_cnt = 2.37F / (pow(2,BITS-1)) / GAIN / SINC_ORDER;
  Serial.println(volt_per_cnt);
};

float convert(int counts)
{
  return counts * volt_per_cnt;
}

void print_chans(float ch0, unsigned int ch0_cnts, float ch1, unsigned int ch1_cnts)
{
  Serial.print("CH0:");
  Serial.print(ch0);
  Serial.print(" (");
  Serial.print(ch0_cnts, DEC);
  Serial.print(") ");

  Serial.print("CH1:");
  Serial.print(ch1);
  Serial.print(" (");
  Serial.print(ch1_cnts, DEC);
  Serial.print(") ");

  Serial.print('\r');
  
}

void loop()
{

  int i, j;
  float ch0, ch1;
  unsigned int ch0_cnts, ch1_cnts;

  /* Activate SPI device */
  digitalWrite(CS, LOW);
  
  /* Send device 0, read command for address 0x0 */
  SPI.transfer(0x1);

  while (1) {

    for(i=0; i<NUM_CHAN; i++){
      float value;
      unsigned int cnts = 0;

      for(j=0; j<WIDTH; j++){
	byte ret;
	ret = SPI.transfer(0x0);

	cnts += ret;
	cnts <<= (WIDTH - 1 - j) * CHAR_BIT;

      }
      value = convert(cnts);

      switch(i){
      case 0:
	ch0 = value;
	ch0_cnts = cnts;
	break;
      case 1:
	ch1 = value;
	ch1_cnts = cnts;
	break;
      }

    }
    print_chans(ch0, ch0_cnts, ch1, ch1_cnts);
    delay(100);

  }

  /* Deactivate SPI device */
  digitalWrite(CS, HIGH);
  
};
