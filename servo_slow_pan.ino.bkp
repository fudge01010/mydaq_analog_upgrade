#define SERVO 1

int value;
int servoValue;
volatile bool refreshServo;

void spicyDelay(uint16_t milliseconds);

void setup() {
  
  analogReference(DEFAULT); //make sure we're using the 5v (VCC) reference for full range
  pinMode(SERVO, OUTPUT);

// Set up the interrupt that will refresh the servo for us automagically
  OCR0A = 0xAF;            // any number is OK
  TIMSK |= _BV(OCIE0A);    // Turn on the compare interrupt (below!)
}

int pos;
unsigned long currentMillis;
unsigned long previousMillis = 0; 
int i;

void loop() {

    for(pos = 600; pos < 2400; pos++)  // goes from 0 degrees to 180 degrees
    {                                   // in steps of 1 degree
        delay(2);
        if (refreshServo) {
          pulseMicros(pos);
        }
    }
    for(pos = 2400; pos > 600; pos--)  // goes from 180 degrees to 0 degrees
    {
        delay(2);
        if (refreshServo) {
          pulseMicros(pos);
        }
    }
}

void spicyDelayMicroseconds(uint16_t us)
{
  // for the 8 MHz internal clock on the ATmega168

  // for a one- or two-microsecond delay, simply return.  the overhead of
  // the function calls takes more than two microseconds.  can't just
  // subtract two, since us is unsigned; we'd overflow.
  if (--us == 0)
      return;
  if (--us == 0)
      return;

  // the following loop takes half of a microsecond (4 cycles)
  // per iteration, so execute it twice for each microsecond of
  // delay requested.
  us <<= 1;

  // partially compensate for the time taken by the preceeding commands.
  // we can't subtract any more than this or we'd overflow w/ small delays.
  us--;

    // busy wait
    __asm__ __volatile__ (
        "1: sbiw %0,1" "\n\t" // 2 cycles
        "brne 1b" : "=w" (us) : "0" (us) // 2 cycles
    );
}//end spicyDelayMicroseconds

void spicyDelay(uint16_t milliseconds)
{
    for(uint16_t i = 0; i < milliseconds; i++)
    {
        spicyDelayMicroseconds(1000);
    }
}//end spicyDelay

void pulseMicros(int timeIn) {
  refreshServo = false;
  //high pulse:
  digitalWrite(SERVO, HIGH);
  delayMicroseconds(timeIn);
  digitalWrite(SERVO, LOW);
//  delayMicroseconds(20000-timeIn);
}
//

volatile uint8_t counter = 0;
SIGNAL(TIMER0_COMPA_vect) {
  // this gets called every 2 milliseconds
  counter += 2;
  // every 20 milliseconds, refresh the servos!
  if (counter >= 20) {
      counter = 0;
      refreshServo = true;
  //    myServo2.refresh();
  }
}

