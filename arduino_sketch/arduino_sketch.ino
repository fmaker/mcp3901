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

/* Arduino */
#define	 CS	53

/* MCP3901 */
#define  CH0    0x00
#define  CH1    0x03
#define  WIDTH  16

void setup(){

  // Setup serial
  Serial.begin(9600);
  Serial.println("MCP3901 Test v0.1");

  // Setup SPI
  pinMode(CS, OUTPUT);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV128); /* Slowest clock for testing */
  SPI.begin();
};

void loop() {
  byte ret;
  int ch1, ch2;

  /* Activate SPI device */
  digitalWrite(CS, LOW);

  /* Send device 0, read command for address 0x0 */
  SPI.transfer(0x1);

  /* Need to get two bytes from each adc and output them sequentially */

  while(1){

    ret = SPI.transfer(0x0);
  }

  /* Deactivate SPI device */
  digitalWrite(CS, HIGH);

};
