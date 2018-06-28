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
    inp =  inp + constrain(analogRead(A3), 41, 962);//962
    delay(1);
    for (int j = 0; j<4;j++) {
      inp =  inp + constrain(analogRead(A3), 41, 962);
      inp = inp / 2;
      delay(1);
    }
//    inp = constrain(analogRead(A3), 41, 962);
    pos = map(inp, 41, 962, 600, 2400);
    delay(1);  
  }
  
  
}

void pulseMicros(int timeIn) {
  refreshServo = false;
  //high pulse:
  digitalWrite(SERVO, HIGH);
  delayMicroseconds(timeIn);
  digitalWrite(SERVO, LOW);
//  delayMicroseconds(20000-timeIn);
}

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

