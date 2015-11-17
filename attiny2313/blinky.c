// #define F_CPU 8000000L
#include <avr/io.h>
#include <util/delay.h>

int main() {
    const int ledPin1 = PB0;
    const int ledPin2 = PD5;

    DDRB |= (1<<ledPin1)|(1<<ledPin2);

    PORTB &= ~(1 << ledPin1);
    PORTD |= (1 << ledPin2);

    while(1) {
        if ((PINB & (1 << ledPin1))) {
            PORTB &= ~(1 << ledPin1);
        } else {
            PORTB |= (1 << ledPin1);
        }
        if ((PIND & (1 << ledPin2))) {
            PORTD &= ~(1 << ledPin2);
        } else {
            PORTD |= (1 << ledPin2);
        }
        _delay_ms(500);
    }
}
