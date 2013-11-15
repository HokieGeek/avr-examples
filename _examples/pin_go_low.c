//	Author: Dingo_aus
//	Original Website: www.users.on.net/~symes
//	Document subject to copyright, permission must be granted by author before copying


#include <avr/io.h>	//included so we can use the text label for ports, pins etc.
#include <wdt.h>	//included so we can call the function wdt_disable();

int main(void)	//All C programs need a function called main - when run, this is called before anythign else
{
wdt_disable();	//call this function to stop the watch-dog timer firing

outp(0xFF, DDRD);	//set all PORTD pins to output by writing 0xFF (11111111) to the DDRB register

PORTD = 0xFF;		//make all pins high (PORTD = 11111111)
PORTD &= ~(1 << 5);//make the sixth pin go low. See website for explanation
	
}
