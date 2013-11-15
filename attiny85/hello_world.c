#define F_CPU 16000000UL // 16 MHZ

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

//Global variables and definition

/*
#define PULSE_WIDTH 0x40
#define TIMER1_TOP 255   
#define TIMER1_PWM_INIT _BV(PWM1A) | _BV(COM1A1)
#define TIMER1_CLOCKSOURCE _BV(CS12)

enum { UP, DOWN };

ISR (TIM0_OVF_vect) {
    static uint8_t pwm;
    static uint8_t direction;

    switch (direction) {
        case UP: if (++pwm == TIMER1_TOP) direction = DOWN; break;
        case DOWN: if (--pwm == 0) direction = UP; break;
    }

    OCR1A = pwm; 
	PORTB |= (1 << PB4);

	if ((PINB & (1 << PB3))) {
	    PORTB &= ~(1 << PB3);
    } else {
	    PORTB |= (1 << PB3);
    }
    _delay_ms(500);
}

void pwm_start(int pin) {
    // Timer 1 is 8-bit PWM
    //TCCR1 = TIMER1_PWM_INIT;
    //TCCR1 |= TIMER1_CLOCKSOURCE;
    CLKPR = (1 << CLKPCE);        // enable clock prescaler update
    CLKPR = 0;                    // set clock to maximum (= crystal)

    // Set PWM value to 0.
    OCR1A = 42;

    // Enable OC1 as output.
    DDRB |= (1 << pin);

    // Enable timer 1 overflow interrupt.
    //TIMSK = _BV (TOIE1);

    TIFR = (1 << TOV0);           // clear interrupt flag
    TIMSK = (1 << TOIE0);         // enable overflow interrupt
    TCCR0B = (1 << CS00);         // start timer, no presc

	PORTB |= (1 << PB3);
    _delay_ms(500);
	PORTB &= ~(1 << PB3);

    sei ();
}

int main(void) {
    int digitalOutputPin = PB3;
    int digitalInputPin = PB0;
    int analogOutputPin = PB4;
    int analogInputPin = PB2;

    DDRB |= (1 << digitalOutputPin);

    //pwm_start(analogOutputPin);

    for (;;) {
	    if ((PINB & (1 << digitalInputPin))) {
		    PORTB |= (1 << digitalOutputPin);
        } else {
		    PORTB &= ~(1 << digitalOutputPin);
        }
    }
}
*/

void blinkLED() {
  DDRB |= (1 << PB3);
  int blink = 10;
  // for (blink = 0; blink <= 10; blink++) {
  for (; blink >= 0; blink--) {
	    PORTB &= ~(1 << PB3);
        delay_ms(20);
	    PORTB |= (1 << PB3);
        delay_ms(90);
  }
  PORTB &= ~(1 << PB3);
}
/*
*/

#define RED PB1
unsigned char compare;
volatile unsigned char compbuff;

void delay_ms(uint16_t ms) {
  while (ms) {
    _delay_ms(1);
    ms--;
  }
}

void init(void) {
  // set the direction of the ports
  DDRB |= (1 << RED);
  
  CLKPR = (1 << CLKPCE);        // enable clock prescaler update
  CLKPR = 0;                    // set clock to maximum (= crystal)

  OCR1A = 250;

  // initialise all channels
  compare = 0x00;           // set default PWM values
  compbuff = 0x00;          // set default PWM values

  TIFR = (1 << TOV0);           // clear interrupt flag
  TIMSK = (1 << TOIE0);         // enable overflow interrupt
  //TCCR0B = (1 << CS00);         // start timer, no prescale

  TCCR0A |= _BV(COM0A1);

  // Set timer to count with F_CPU / 8
  TCCR0B = _BV(CS10) | _BV(CS01) | _BV(CS00);

  // Use Fast PWM, OCRA TOP
  TCCR0A = _BV(WGM00) | _BV(WGM01);

  sei();
}

#define PORTB_MASK  (1 << RED)
ISR (TIM0_OVF_vect) {
  static unsigned char pinlevelB = PORTB_MASK;
  static unsigned char softcount = 0xFF;

  PORTB = pinlevelB;            // update outputs
  
  if(++softcount == 0){         // increment modulo 256 counter and update
                                // the compare values only when counter = 0.
    compare = compbuff;   // verbose code for speed

    pinlevelB = PORTB_MASK;     // set all port pins high
  }
  // clear port pin on compare match (executed on next interrupt)
  if(compare == softcount) pinlevelB &= ~(1 << RED);
}

int r_val = 0x00;
int main(void) {
  DDRB |= (1 << PB3);

  init();
  
  int r_dir = 1;
  
  for(;;) {
	if ((PINB & (1 << PB5))) {
	    PORTB |= (1 << PB3);
    } else {
	    PORTB &= ~(1 << PB3);
    }

    if (r_val > 254 - 1) { r_dir = -1; }
    if (r_val < 1 + 1) { r_dir = 1; }
    
    r_val += r_dir;

    compbuff = r_val;

    delay_ms(50);
  }
}

/*
int main()
{
    // Turn on LED output pin
    DDRB   |= _BV(PB4);

    // Toggle on match
    //TCCR0A |= _BV(COM0A0);
    TCCR0A |= _BV(COM0A1);

    // Set timer to count with F_CPU / 8
    TCCR0B |= _BV(CS10);
    TCCR0B |= _BV(CS01) | _BV(CS00);

    // Use Fast PWM, OCRA TOP
    TCCR0A |= _BV(WGM00);
    TCCR0A |= _BV(WGM01);
    //TCCR0B |= _BV(WGM02);
    OCR0A = 0x00;

    uint8_t dir = 1;
    uint16_t div = 500;
    uint16_t stall = 0;
    uint16_t stall_time = 500;
    for(;;) {
        // We only want to update every div counts
        if(TCNT0 % div != 0) continue;
        // Stall at the top and bottom, and change direction
        if(OCR0A == 255 || OCR0A == 0) {
            // Switch directon at top
            if(OCR0A == 255) dir = -1;
            else 
            {   // Disable LED at bottom
                // And switch direction
                DDRB  &= ~(_BV(PB4));
                dir = 1; 
            }
            while(stall < stall_time) 
            { 
                ++stall;
                _delay_ms(10); 
            }
            // Turn output back on if necessary
            if(OCR0A == 0) DDRB  |= _BV(PB4);
            stall = 0;
        }
        // General case - increment direction either way
        OCR0A += dir;
    }
    return 0;
}
*/

/*
#define CHMAX 3 // maximum number of PWM channels
#define PWMDEFAULT 0x00 // default PWM value at start up for all channels

#define RED_CLEAR (pinlevelB &= ~(1 << RED)) // map RED to PB0
#define GREEN_CLEAR (pinlevelB &= ~(1 << GREEN)) // map GREEN to PB1
#define BLUE_CLEAR (pinlevelB &= ~(1 << BLUE)) // map BLUE to PB2

//! Set bits corresponding to pin usage above
#define PORTB_MASK  (1 << PB0)|(1 << PB1)|(1 << PB2)

#define set(x) |= (1<<x) 
#define clr(x) &=~(1<<x) 
#define inv(x) ^=(1<<x)

#define RED PB0
#define GREEN PB1
#define BLUE PB2
#define LED_PORT PORTB
#define LED_DDR DDRB

void delay_ms(uint16_t ms);
void init();

unsigned char compare[CHMAX];
volatile unsigned char compbuff[CHMAX];

int r_val = 0x00;
int g_val = 0x55;
int b_val = 0xAA;
float dim = 1;

int main() {
  blinkLED();
  
  init();
  
  int r_dir = 1;
  int g_dir = 2;
  int b_dir = 4;
  
  for(;;) {
    if (r_val > 254 - 1) {
      r_dir = -1;
    }
    if (r_val < 1 + 1) {
      r_dir = 1;
    }

    if (g_val > 254 - 3) {
      g_dir = -2;
    }
    if (g_val < 1 + 3) {
      g_dir = 2;
    }

    if (b_val > 254 - 4) {
      b_dir = -4;
    }
    if (b_val < 1 + 4) {
      b_dir = 4;
    }
    
    r_val += r_dir;
    g_val += g_dir;
    b_val += b_dir;

    compbuff[0] = r_val;
    compbuff[1] = g_val;
    compbuff[2] = b_val;

    delay_ms(50);
  }
}


void delay_ms(uint16_t ms) {
  while (ms) {
    _delay_ms(1);
    ms--;
  }
}

void init(void) {
  // set the direction of the ports
  LED_DDR set(RED);
  LED_DDR set(GREEN);
  LED_DDR set(BLUE);

  unsigned char i, pwm;

  CLKPR = (1 << CLKPCE);        // enable clock prescaler update
  CLKPR = 0;                    // set clock to maximum (= crystal)

  pwm = PWMDEFAULT;

  // initialise all channels
  for(i=0 ; i<CHMAX ; i++) {
    compare[i] = pwm;           // set default PWM values
    compbuff[i] = pwm;          // set default PWM values
  }

  TIFR = (1 << TOV0);           // clear interrupt flag
  TIMSK = (1 << TOIE0);         // enable overflow interrupt
  TCCR0B = (1 << CS00);         // start timer, no prescale

  sei();
}


ISR (TIM0_OVF_vect) {
  static unsigned char pinlevelB=PORTB_MASK;
  static unsigned char softcount=0xFF;

  PORTB = pinlevelB;            // update outputs
  
  if(++softcount == 0){         // increment modulo 256 counter and update
                                // the compare values only when counter = 0.
    compare[0] = compbuff[0];   // verbose code for speed
    compare[1] = compbuff[1];
    compare[2] = compbuff[2];

    pinlevelB = PORTB_MASK;     // set all port pins high
  }
  // clear port pin on compare match (executed on next interrupt)
  if(compare[0] == softcount) RED_CLEAR;
  if(compare[1] == softcount) GREEN_CLEAR;
  if(compare[2] == softcount) BLUE_CLEAR;
}
*/
