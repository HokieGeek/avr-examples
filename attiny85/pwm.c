#define F_CPU 8000000UL

#include <avr/cpufunc.h>
#include <avr/interrupt.h>
#include <avr/io.h>
//#include <util/delay.h>

uint8_t dir = 1;
/*
uint16_t div = 500;
uint16_t stall = 0;
uint16_t stall_time = 500;
*/

// Interrupt Service Routines Macro
ISR (TIM0_OVF_vect) {
//ISR(TIMER0_COMPA_vect)
    // PORTB = ~PORTB;
    // Stall at the top and bottom, and change direction
    if(OCR0A == 255 || OCR0A == 0) {
        // Switch directon at top
        if(OCR0A == 255) dir = -1;
        else dir = 1;
    }

    OCR0A += dir;
}

void init(void) {
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

    sei();
}

int main(void) {
    //DDRB |= (1 << PB3);
    //DDRB |= (1 << PB4);

    init();

    while(1) {
        _NOP();
        /*
        if ((PINB & (1 << PB2))) {
            PORTB |= (1 << PB3);
        } else {
            PORTB &= ~(1 << PB3);
        }*/

    /*
        if ((PINB & (1 << PB2))) PORTB |= (1 << PB4);
        else PORTB &= ~(1 << PB4);
    */
    }
}

/*
int main(void) {
    // Turn on LED output pin
    DDRB   |= _BV(PB1);

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
                DDRB  &= ~(_BV(PB1));
                dir = 1; 
            }
            while(stall < stall_time) 
            { 
                ++stall;
                _delay_ms(10); 
            }
            // Turn output back on if necessary
            if(OCR0A == 0) DDRB  |= _BV(PB1);
            stall = 0;
        }
        // General case - increment direction either way
        OCR0A += dir;
    }
}
*/
