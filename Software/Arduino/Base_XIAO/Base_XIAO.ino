#include <fast_samd21_tc4.h>


#define bitcheck(byte,nbit) ((byte) & (1<<(nbit)))

// analog input
#define SENSOR_PIN (D0)

// digital output
#define NODE_A (10)
#define NODE_A_INDEX (7)
#define NODE_B (8)
#define NODE_B_INDEX (5)
#define NODE_C (1)
#define NODE_C_INDEX (3)
#define NODE_D (3)
#define NODE_D_INDEX (1)

#define NODE_nA (9)
#define NODE_nA_INDEX (6)
#define NODE_nB (7)
#define NODE_nB_INDEX (4)
#define NODE_nC (2)
#define NODE_nC_INDEX (2)
#define NODE_nD (6)
#define NODE_nD_INDEX (0)

#define NUMBER_OF_PUN_STATES  (128u)


// A A' B B' C C' D D'
uint32_t pinStateList[NUMBER_OF_PUN_STATES] = {0b10010101, 0b10010101, 0b10101010, 0b01101010, 0b10100101, 0b10010101, 0b01101010, 0b01101010, 0b10100101, 0b10010101, 0b01101010, 0b01101010, 0b10100101, 0b10100101, 0b01101010, 0b01101010, 0b10100101, 0b10100101, 0b01101010, 0b01011010, 0b10101001, 0b10100101, 0b01011010, 0b01011010, 0b10101001, 0b10100101, 0b01011010, 0b01011010, 0b10101001, 0b10101001, 0b01011010, 0b01011010, 0b10101001, 0b10101001, 0b01011010, 0b01010110, 0b10101010, 0b10101001, 0b01010110, 0b01010110, 0b10101010, 0b10101001, 0b01010110, 0b01010110, 0b10101010, 0b10101010, 0b01010110, 0b01010110, 0b10101010, 0b10101010, 0b01010110, 0b01010101, 0b01101010, 0b10101010, 0b01010101, 0b01010101, 0b01101010, 0b10101010, 0b01010101, 0b01010101, 0b01101010, 0b01101010, 0b01010101, 0b01010101, 0b01101010, 0b01101010, 0b01010101, 0b10010101, 0b01011010, 0b01101010, 0b10010101, 0b10010101, 0b01011010, 0b01101010, 0b10010101, 0b10010101, 0b01011010, 0b01011010, 0b10010101, 0b10010101, 0b01011010, 0b01011010, 0b10010101, 0b10100101, 0b01010110, 0b01011010, 0b10100101, 0b10100101, 0b01010110, 0b01011010, 0b10100101, 0b10100101, 0b01010110, 0b01010110, 0b10100101, 0b10100101, 0b01010110, 0b01010110, 0b10100101, 0b10101001, 0b01010101, 0b01010110, 0b10101001, 0b10101001, 0b01010101, 0b01010110, 0b10101001, 0b10101001, 0b01010101, 0b01010101, 0b10101001, 0b10101001, 0b01010101, 0b01010101, 0b10101001, 0b10101010, 0b10010101, 0b01010101, 0b10101010, 0b10101010, 0b10010101, 0b01010101, 0b10101010, 0b10101010, 0b10010101, 0b10010101, 0b10101010, 0b10101010};
volatile uint16_t currentPinState = 0;

void TC4_Handler(void) 
{
  volatile uint32_t pinState = pinStateList[currentPinState];
 
  digitalWrite(NODE_A,  bitcheck(pinState, NODE_A_INDEX));
  digitalWrite(NODE_B,  bitcheck(pinState, NODE_B_INDEX));
  digitalWrite(NODE_C,  bitcheck(pinState, NODE_C_INDEX));
  digitalWrite(NODE_D,  bitcheck(pinState, NODE_D_INDEX));
  digitalWrite(NODE_nA, bitcheck(pinState, NODE_nA_INDEX));
  digitalWrite(NODE_nB, bitcheck(pinState, NODE_nB_INDEX));
  digitalWrite(NODE_nC, bitcheck(pinState, NODE_nC_INDEX));
  digitalWrite(NODE_nD, bitcheck(pinState, NODE_nD_INDEX));

  currentPinState++;
  
  if(NUMBER_OF_PUN_STATES == currentPinState)
  {
    currentPinState = 0;
  }

  TC4->COUNT16.INTFLAG.bit.MC0 = 1; // clears the interrupt
}

void setup() {
  // Digital output
  pinMode(NODE_A, OUTPUT);
  pinMode(NODE_B, OUTPUT);
  pinMode(NODE_C, OUTPUT);
  pinMode(NODE_D, OUTPUT);
  pinMode(NODE_nA, OUTPUT);
  pinMode(NODE_nB, OUTPUT);
  pinMode(NODE_nC, OUTPUT);
  pinMode(NODE_nD, OUTPUT);

  digitalWrite(NODE_A, HIGH);
  digitalWrite(NODE_B, HIGH);
  digitalWrite(NODE_C, HIGH);
  digitalWrite(NODE_D, HIGH);
  digitalWrite(NODE_nA, HIGH);
  digitalWrite(NODE_nB, HIGH);
  digitalWrite(NODE_nC, HIGH);
  digitalWrite(NODE_nD, HIGH);
 
  // Analog init
  // you can read the sensor output by analogRead(SENSOR_PIN)

  // setup timer
  fast_samd21_tc4_configure(20); // starts the timer/trigger with 0.1 s

  Serial.begin(115200);
}

void loop() {

}

