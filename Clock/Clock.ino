#define F_CPU 1000000UL
#define SPEED_UP    1
#define SLOW_DOWN   2
#define CLOCK_PIN   3
#define CLOCK_CAL   0

#include <avr/sleep.h>
#include <avr/interrupt.h>

byte button_interrupted = 0;

void setup() {
  OSCCAL += CLOCK_CAL;  // configure manually
  
  for(byte i=0; i<=4; i++) 
    pinMode(i, INPUT);  // lowest power
  
  pinMode(CLOCK_PIN, OUTPUT);

  // configure button interrupts (speedup/slowdown buttons)
  GIMSK |= (1 << PCIE);
  PCMSK |= (1 << PCINT1) | (1 << PCINT2);

  // configure watchdog for ~8s interrupt
  WDTCR |= (1 << WDIE) | (1 << WDP3) | (1 << WDP0);

  // configure sleep mode
  ADCSRA &= ~_BV(ADEN);  // ADC off
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_bod_disable();

  // enable interrupts
  sei();
}

void loop() {
  digitalWrite(CLOCK_PIN, HIGH);
  delay(1000);  // power clock for 1 second
  digitalWrite(CLOCK_PIN, LOW);
  
  delay(3000);  // sleep for 3 seconds (tuneable with OSCCAL)
  sleep56();    // sleep for 56 seconds (not tuneable)
}

void sleep56() {
  for(byte i=0; i<7; i++) {
    sleep_cpu();  // sleep for 8 seconds
    WDTCR |= (1 << WDIE);  // reenable watchdog interrupt
    
    if(button_interrupted){
      button_interrupted = 0;
      return;
    }
  }
}

ISR(WDT_vect) {
  // blank just so it doesn't reset on watchdog time-out
}


ISR(PCINT0_vect) {
  if(digitalRead(SPEED_UP) == LOW)
    OSCCAL += 1;
  else if(digitalRead(SLOW_DOWN) == LOW)
    OSCCAL -= 1;
  
  delay(100);  // debounce
  button_interrupted = 1;  // skip to next minute cycle
}
