#include <avr/interrupt.h>
#include <avr/sleep.h>

#define LED_PIN PB4
#define LED_PIN_PORT DDB4

#define BUTTON_PIN PB0
//#define TIMER_PRESCALE (1 << WDP0) // 32ms
//#define TIMER_PRESCALE_MILLIS 32
#define TIMER_PRESCALE (1 << WDP1) // 64ms
#define TIMER_PRESCALE_MILLIS 64
//#define TIMER_PRESCALE (1 << WDP2) | (1 << WDP0); // 1/2s
//#define TIMER_PRESCALE_MILLIS 500
#define BTN_DEBOUNCE_DELAY_MILLIS 40
#define BTN_DOUBLE_CLICK_DELAY_MILLIS 150

typedef enum { SINGLE, DOUBLE } BUTTON_CLICK_TYPE;

typedef struct {
    int pinId;
    long lastReleaseTime;
    long lastDebounceTime;
    int state;
    BUTTON_CLICK_TYPE clickType;
    void (*handler)(int, int, BUTTON_CLICK_TYPE);
} BUTTON_ITEM;

long uptime = 0;

BUTTON_ITEM* handledButtons;

long millis(void);
int debouncedState(int);
void buttonHandler(int,int,BUTTON_CLICK_TYPE);

long millis() {
    return uptime;
}

BUTTON_ITEM* debouncedButton(int pin) {
    BUTTON_ITEM* btn = &handledButtons[pin];

    int currentState = (PINB & (1 << btn->pinId));

    if (currentState != btn->state) {
        btn->lastDebounceTime = millis();
    } 

    if (!currentState) {
        btn->lastReleaseTime = millis();
    }

    // if ((millis() - btn->lastDebounceTime) > BTN_DEBOUNCE_DELAY_MILLIS) {
        btn->state = currentState;
        if ((millis() - btn->lastReleaseTime) <= BTN_DOUBLE_CLICK_DELAY_MILLIS) {
            btn->clickType = DOUBLE;
        } else {
            btn->clickType = SINGLE;
        }
    // }

    return btn;
}

void addButton(int pin, void (*handler)(int, int, BUTTON_CLICK_TYPE)) {
    // FIXME: need to check for range
    handledButtons[pin].pinId = pin;
    handledButtons[pin].lastDebounceTime = 0;
    handledButtons[pin].state = 0;
    handledButtons[pin].handler = handler;
    handledButtons[pin].clickType = SINGLE;
}

void buttonsInit(int numPins) {
    int size = sizeof(BUTTON_ITEM)*numPins;
    handledButtons = (BUTTON_ITEM*)malloc(size);
    memset(handledButtons, 0, size);
}

ISR(WDT_vect) {
    // PORTB ^= (1 << LED_PIN);
    uptime += TIMER_PRESCALE_MILLIS;
}

ISR (PCINT0_vect) {
    // TODO: need to figure out which button was pressed
    BUTTON_ITEM* btn = debouncedButton(BUTTON_PIN);
    btn->handler(btn->pinId, btn->state, btn->clickType);
}

void buttonHandler(int btnId, int state, BUTTON_CLICK_TYPE clickCount) {
    switch (btnId) {
    case BUTTON_PIN:
        // if (state) {
        if (state && clickCount == DOUBLE) {
            PORTB ^= (1 << LED_PIN);
        }
       break; 
    }
}

void init_pins() {
    DDRB = 1 << LED_PIN_PORT; // LED PIN

    buttonsInit(1);
    addButton(BUTTON_PIN, &buttonHandler);
}

void init_interrupts() {
    PCMSK |= (1 << PCINT0); // Enable external interrupts PCINT0
    MCUCR  = (1 << ISC00); // (1 << ISC01);
    GIMSK |= (1 << PCIE); // Pin Change Interrupt Enable
 
    WDTCR |= TIMER_PRESCALE; // 64ms
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
