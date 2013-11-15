#define F_CPU 8000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#define LED_PIN PB4
#define LED_PIN_PORT DDB4

#define TIMER_PRESCALE (1 << WDP2) | (1 << WDP0); // 1/2s
#define TIMER_PRESCALE_MILLIS 500

// http://www.adafruit.com/datasheets/sn74hc595.pdf

/*
PB1 to Data Pin (pin14)
PB2 to Latch Clock (pin12)
PB3 to Shift Clock (pin11)
PB4 to Reset (pin10)
*/

#define SREG_PIN_DATA PB1
#define SREG_PIN_LATCHCLOCK PB2
#define SREG_PIN_SHIFTCLOCK PB3
#define SREG_PIN_RESET PB4

#define OPTIONS_RANGE_START 0b01000000
#define OPTIONS_RANGE_END 0b00000100

uint8_t currentVoltage = 0b10000000;

ISR(WDT_vect) {
    nextVoltage();
}

void toggleShiftClock(void) {
    PORTB |= _BV(SREG_PIN_SHIFTCLOCK);
    PORTB &= ~ _BV(SREG_PIN_SHIFTCLOCK);
}
 	
void toggleLatchClock(void) {
    PORTB |= _BV(SREG_PIN_LATCHCLOCK);
    _delay_ms(3);
    PORTB &= ~ _BV(SREG_PIN_LATCHCLOCK);
    _delay_ms(3);
}

void resetRegister(void) {
    PORTB &= ~ _BV(SREG_PIN_RESET);
    _delay_ms(10);
    PORTB |= _BV(SREG_PIN_RESET);
    _delay_ms(10);
}

void shiftBytes(uint8_t data) {
    for (uint8_t i = 0; i < 8; i++) {
        if ((data & (1 << i))) {
            PORTB |= (1 << SREG_PIN_DATA);
        } else {
            PORTB &= (~(1 << SREG_PIN_DATA));
        }
	    toggleShiftClock();
    }
	toggleLatchClock();
}

void nextVoltage() {
    currentVoltage = currentVoltage >> 1;
    if (currentVoltage & OPTIONS_RANGE_END)
        currentVoltage = OPTIONS_RANGE_START;
    shiftBytes(currentVoltage);
}

void initShiftRegister() {
    DDRB = 0xFF;  // Set the Control DDR (i.e. DDRB) to be all outputs
    // DDRB |= ((1<<SREG_PIN_SHIFTCHCLOCK)|(1<<SREG_PIN_LATCHCLOCK)|(1<<SREG_PIN_DATA)|(1<<SREG_PIN_RESET));

    resetRegister();  // Toggle the Reset Pin on the 595 to clear out SR
    // PORTB &= ~_BV(SREG_PIN_OUTPUTENABLE);

    // uint8_t d = 0b10000000;
    // currentVoltage = 0b01000000;
}

void init_pins() {
    // DDRB = (1 << LED_PIN_PORT); // LED PIN

    // ShiftRegister shiftReg(PB0, PB1, PB2, PB3, PB4);
    initShiftRegister();
}

void init_interrupts() {
    WDTCR |= TIMER_PRESCALE;
    WDTCR |= (1 << WDIE); // Enable watchdog timer interrupts

    sei(); // Enable global interrupts 
}

int main(void) {
    init_pins();
    init_interrupts();

    set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Use the Power Down sleep mode

    for (;;) {
        sleep_mode();   // go to sleep and wait for interrupt...
    }
}
