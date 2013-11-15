#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>

int main() {
    const int ledPin = PB4;

    DDRB |= (1 << ledPin);

    while(1) {
        if ((PINB & (1 << ledPin))) {
            PORTB &= ~(1 << ledPin);
        } else {
            PORTB |= (1 << ledPin);
        }
        _delay_ms(500);
    }
}
