#define F_CPU 8000000UL // 8 MHZ

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

const int ledPin = PB4;
volatile int timer_overflow_count = 0;

void blinkLED() {
    if ((PINB & (1 << ledPin))) {
        PORTB &= ~(1 << ledPin);
    } else {
        PORTB |= (1 << ledPin);
    }
}

ISR (TIM0_OVF_vect) {
    // blinkLED();
    // PORTB = ~PORTB;

    // a timer overflow occurs 4.6 times per second
    if (++timer_overflow_count > 5) {   
      // Toggle Port B pin 4 output state
      PORTB ^= 1<<PB4;
      timer_overflow_count = 0;
   }
}

void initTimer() {
    TCCR0B |= (1<<CS02) | (1<<CS00);
    // enable timer overflow interrupt
    TIMSK |=1<<TOIE0;

    // TCCR0A = (1<<CS02)|(1<<CS00); //Timer clock = system clock / 1024
    // TIFR = 1<<TOV0; //Clear TOV0 / clear pending interrupts
    // TIMSK = 1<<TOIE0; //Enable Timer0 Overflow Interrupt

    // DDRB = 0xFF; //Set Port B as outpu
    /*
    DDRB |= _BV(DDB0);

    // TIFR = _BV(TOV0);           // clear interrupt flag
    // TIMSK = _BV(TOIE0);         // enable overflow interrupt

    // Toggle on match
    TCCR0A |= _BV(COM0A1);

    // Set timer to count with F_CPU / 8
    TCCR0B |= _BV(CS10);
    TCCR0B |= _BV(CS01) | _BV(CS00);

    // Use Fast PWM, OCRA TOP
    TCCR0A |= _BV(WGM00);
    TCCR0A |= _BV(WGM01);

    OCR0A = 0x00;
    */

    sei();
}

int main() {
    //DDRB |= (1 << ledPin);
    DDRB = 1 << DDB4;

    initTimer();

    /*
    for (;;) {
        blinkLED();

        _delay_ms(500);
    }
    */

    return 1;
}
