#include <avr/io.h>
#include <wdt.h>	//included so we can call the function wdt_disable();

int main(void)	//All C programs need a function called main - when run, this is called first
{
wdt_disable();	//call this function to stop the watch-dog timer firing

outp(0xFF, DDRB);	//set all of PORTB to output by writing 0xFF (11111111) to the DDRB register

	while (1)	//loop forever
	{
		
		PORTB = 0x33; 	// 00110011
		PORTB |=(1<<2);// make the physical third pin on port B go high (known as pin 2)

	}
}
