#define F_CPU 8000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#define TIMER_PRESCALE (1 << WDP2) | (1 << WDP1); // 1s
// #define TIMER_PRESCALE (1 << WDP2) | (1 << WDP1) | (1 << WDP0); // 2s

#define BUTTON_PIN PB0
#define LED_PIN PB4
#define LED_PIN_PORT DDB4

#define SAVE_ADDRESS 0b01000000
#define LED_ON 0b01000000
#define LED_OFF 0b00000100

void EEPROM_write(uint8_t address, uint8_t data) {
    while(EECR & (1<<EEPE)) {} // Wait for completion of previous write

    EECR = (0<<EEPM1)|(0<<EEPM0); // Set Programming mode

    // Set up address and data registers
    EEAR = address;
    EEDR = data;

    EECR |= (1<<EEMPE); // Write logical one to EEMPE
    EECR |= (1<<EEPE); // Start eeprom write by setting EEPE
}

uint8_t EEPROM_read(uint8_t address) {
    while(EECR & (1<<EEPE)) {} // Wait for completion of previous write

    EEAR = address; // Set up address register
    EECR |= (1<<EERE); // Start eeprom read by writing EERE

    return EEDR; // Return data from data register
}

ISR(WDT_vect) {
    // PORTB ^= (1 << LED_PIN);
    uint8_t currVal = EEPROM_read(SAVE_ADDRESS);
    if (currVal & LED_OFF) {
        PORTB |= (1 << LED_PIN);
        // EEPROM_write(SAVE_ADDRESS, LED_ON);
    } else {
        PORTB &= ~(1 << LED_PIN);
        // EEPROM_write(SAVE_ADDRESS, LED_OFF);
    }
}

ISR (PCINT0_vect) {
    uint8_t currVal = EEPROM_read(SAVE_ADDRESS);
    if (currVal & LED_OFF) {
        EEPROM_write(SAVE_ADDRESS, LED_ON);
    } else {
        EEPROM_write(SAVE_ADDRESS, LED_OFF);
    }
}

int main(void) {
    DDRB = (1<<LED_PIN_PORT); // LED PIN

    PCMSK |= (1 << PCINT0); // Enable external interrupts PCINT0
    MCUCR  = (1 << ISC00); // (1 << ISC01);
    GIMSK |= (1 << PCIE); // Pin Change Interrupt Enable

    WDTCR |= TIMER_PRESCALE;
    WDTCR |= (1<<WDIE); // Enable watchdog timer interrupts

    sei(); // Enable global interrupts 

    EEPROM_write(SAVE_ADDRESS, LED_OFF);

    set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Use the Power Down sleep mode

    for (;;) {
        sleep_mode();   // go to sleep and wait for interrupt...
    }
}
