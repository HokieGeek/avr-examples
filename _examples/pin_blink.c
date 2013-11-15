#include <avr/io.h>
#include <avr/wdt.h>

int main(void)	
{
		PORTB = 0x00;
		PORTD = 0x00;
		//PORTB = 0x33; 	// 00110011
		//PORTB |= (1 << 5);// make the physical third pin on port B go high (known as pin 2)

	//wdt_disable();	//call this function to stop the watch-dog timer firing

	//outp(0xFF, DDRD);	//set all PORTD pins to output by writing 0xFF (11111111) to the DDRB register
	//DDRD = 0xFF;	//set all PORTD pins to output by writing 0xFF (11111111) to the DDRB register

	//while (1) {
		//PORTD = 0xFF;		//make all pins high (PORTD = 11111111)
		//PORTD &= ~(1 << 5); //make the sixth pin go low. See website for explanation

		/*
		delay(1000);

		PORTB = 0x33; 	// 00110011
		PORTB |= (1 << 5);// make the physical third pin on port B go high (known as pin 2)
		*/
	//}
}

/*void delay(int pause)  //time in mS
{
  for (;pause;pause--) {
#asm
  push    2
  mov     R2,#0D7h    // fine tune to crystal speed
  djnz    R2,$
  mov     R2,#0D7h
  djnz    R2,$
  pop     2
#endasm
  }
}*/

