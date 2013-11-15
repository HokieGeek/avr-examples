#include <avr/interrupt.h>
#include <avr/sleep.h>

#define LED_PIN PB4

#define BUTTON_PIN PB0
#define TIMER_PRESCALE (1 << WDP0) // 32ms
#define TIMER_PRESCALE_MILLIS 32
//#define TIMER_PRESCALE (1 << WDP1) // 64ms
//#define TIMER_PRESCALE_MILLIS 64
//#define TIMER_PRESCALE (1 << WDP2) | (1 << WDP0); // 1/2s
//#define TIMER_PRESCALE_MILLIS 500
#define DEBOUNCE_DELAY_MILLIS 60

long uptime = 0;

long millis() {
    return uptime;
}

typedef struct {
    int pinId;
    long lastDebounceTime;
    int state;
} DEBOUNCE_ITEM;

DEBOUNCE_ITEM* debouncedButtons;

int debouncedState(int pin) {
    DEBOUNCE_ITEM* item = debouncedButtons; // FIXME

    int currentState = (PINB & (1 << item->pinId));

    if (currentState != item->state) {
        //item->lastDebounceTime = millis();
    }

    //if ((millis() - item->lastDebounceTime) > DEBOUNCE_DELAY_MILLIS) {
        item->state = currentState;
    //}

    return item->state;
}

void debounceButton(int pin) {
    debouncedButtons = (DEBOUNCE_ITEM*)malloc(sizeof(DEBOUNCE_ITEM));
    debouncedButtons->pinId = pin;
    debouncedButtons->lastDebounceTime = 0;
    debouncedButtons->state = 0;
}

ISR(WDT_vect) {
    uptime += TIMER_PRESCALE_MILLIS;
}

ISR (PCINT0_vect) {
    // if (PINB & (1 << BUTTON_PIN)) {
    if (debouncedState(BUTTON_PIN)) {
        PORTB ^= (1 << LED_PIN);
    }
}

void init_pins() {
    DDRB = 1 << DDB4; // LED PIN

    debounceButton(BUTTON_PIN);
}

void init_interrupts() {
    PCMSK |= (1 << PCINT0); // Enable external interrupts PCINT0
    MCUCR  = (1 << ISC00); // | (1 << ISC00);
    GIMSK |= (1 << PCIE); // Pin Change Interrupt Enable
 
    WDTCR |= TIMER_PRESCALE; // 64ms
    WDTCR |= (1 << WDTIE); // Enable watchdog timer interrupts

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
