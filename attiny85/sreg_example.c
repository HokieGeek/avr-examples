#include<avr/io.h>
#include<util/delay.h>

/*-------------CONNECTION BETWEEN 74HC595 AND ATTINY13-----------------*/
#define SREG_PIN_DATA PB0
#define SREG_PIN_OUTPUTENABLE PB1
#define SREG_PIN_LATCHCLOCK PB2
#define SREG_PIN_SHIFTCLOCK PB3
#define SREG_PIN_RESET PB4

#define OPTIONS_RANGE_START 0b01000000
#define OPTIONS_RANGE_END 0b00000100

uint8_t currentVoltage = 0b10000000;

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
    // DDRB |= ((1 << HC595_SH_CP_POS)|(1<<HC595_ST_CP_POS)|(1<<HC595_DS_POS));

    resetRegister();  // Toggle the Reset Pin on the 595 to clear out SR
    PORTB &= ~_BV(SREG_PIN_OUTPUTENABLE);

    // uint8_t d = 0b10000000;
    // currentVoltage = 0b01000000;
}

/*---------------------------MAIN FUNCTION--------------------------*/
void main(void) {
    initShiftRegister();
    while(1)  {
        nextVoltage();
        _delay_ms(1000);
    }
}
