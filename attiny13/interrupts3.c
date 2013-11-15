#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#define LED_PIN PB4
 
ISR(WDT_vect) {
   PORTB |= (1<<LED_PIN);
   _delay_ms(300);
   PORTB &= ~(1<<LED_PIN);
}

int main(void) {
   // Set up Port B pin 4 mode to output
   DDRB = 1<<DDB4;
 
   // temporarily prescale timer to 4s so we can measure current
   WDTCR |= (1<<WDP3); // (1<<WDP2) | (1<<WDP0);
   WDTCR |= (1<<WDTIE); // Enable watchdog timer interrupts
 
   sei(); // Enable global interrupts 
 
   set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Use the Power Down sleep mode
   for (;;) {
      sleep_mode();   // go to sleep and wait for interrupt...
   }
}
