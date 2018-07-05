#define SERVO 1

volatile bool refreshServo;

void setup() {
//  analogReference(DEFAULT); //make sure we're using the 5v (VCC) reference for full range
  pinMode(SERVO, OUTPUT);

// Set up the interrupt that will refresh the servo for us automagically
  OCR0A = 0xAF;            // any number is OK
  TIMSK |= _BV(OCIE0A);    // Turn on the compare interrupt (below!)

  for (int i = 0; i<100;i++){   //manually trugger a mid-range position to "test" servo, for approx 2sec
    pulseMicros(1500);
    delayMicroseconds(18500);  
  }
}

int pos;        // init vars
int inp;


void loop() {
  if (refreshServo) {       //if interrupt has fired and flag set, manually create servo pulse
    pulseMicros(pos);
  } else {                  //otherwise, read analog pin and determine servo position
    inp = 0;
    inp =  inp + constrain(analogRead(A3), 41, 962);
    /*
    The ATtiny has a 10-bit adc (1024bit). Giving us a resolution of 4.88mv/bit.
    Due to the silicon diode onboard the backpack, we lose ~0.4v at the bottom end of the range. This then goes through the
    voltage divider, meaning we lose ~0.2v minimum. 0.2v * 4.88mv = 41 bits.

    At the top of the range, we can see a maximum of ((10v - 0.4v)/2) = 983. To account for variation in each diode, DAQ output, etc, we
    constrain the top end to 962 bits (~9.78 volts).

    This is then read 5 times, and averaged out, giving the final analog value.
    */
    delay(1);
    for (int j = 0; j<4;j++) {
      inp =  inp + constrain(analogRead(A3), 41, 962);
      inp = inp / 2;
      delay(1);
    }
//    inp = constrain(analogRead(A3), 41, 962);

    //Finally, the read analog value is mapped to a microsecond range of 600-2400, corresponding to a 
    //range of approx 180degrees on the servos we use.
    pos = map(inp, 41, 962, 600, 2400);
    delay(1);  
  }
  
  
}


// this function is called when the interrupt has triggered, and sets the flag to pulse the servo.
void pulseMicros(int timeIn) {
  refreshServo = false;
  //high pulse:
  digitalWrite(SERVO, HIGH);
  delayMicroseconds(timeIn);
  digitalWrite(SERVO, LOW);
//  delayMicroseconds(20000-timeIn);
}

//this interrupt and it's associated counter var are called every 2ms. After 20ms (period between servo pulses), it sets the refresh flag.
volatile uint8_t counter = 0;
SIGNAL(TIMER0_COMPA_vect) {
  // this gets called every 2 milliseconds
  counter += 2;
  // every 20 milliseconds, refresh the servos!
  if (counter >= 20) {
      counter = 0;
      refreshServo = true;
  }
}

