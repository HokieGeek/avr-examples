#include <avr/io.h>
#include <util/delay.h>

/***************************************
Configure Connections
****************************************/

#define HC595_PORT   PORTB
#define HC595_DDR    DDRB

#define HC595_DS_POS PB0      //Data pin (DS) pin location

#define HC595_SH_CP_POS PB1      //Shift Clock (SH_CP) pin location 
#define HC595_ST_CP_POS PB2      //Store Clock (ST_CP) pin location

/***************************************
Configure Connections ***ENDS***
****************************************/

//Sends a clock pulse on SH_CP line. Pulse the Shift Clock
void HC595Pulse() {
   HC595_PORT|=(1<<HC595_SH_CP_POS);//HIGH
   HC595_PORT&=(~(1<<HC595_SH_CP_POS));//LOW
}

//Sends a clock pulse on ST_CP line. Pulse the Store Clock
void HC595Latch() {
   HC595_PORT|=(1<<HC595_ST_CP_POS);//HIGH
   _delay_loop_1(1);

   HC595_PORT&=(~(1<<HC595_ST_CP_POS));//LOW
   _delay_loop_1(1);
}

void HC595Write(uint8_t data) {
   //Order is MSB first
   for(uint8_t i=0;i<8;i++) {
      //Output the data on DS line according to the value of MSB
      if(data & 0b10000000) {
         //MSB is 1 so output high
         HC595_PORT |= (1<<HC595_DS_POS);
      } else {
         //MSB is 0 so output high
         HC595_PORT &= (~(1<<HC595_DS_POS));
      }

      HC595Pulse();  //Pulse the Clock line
      data=data<<1;  //Now bring next bit at MSB position
   }

   //Now all 8 bits have been transferred to shift register
   //Move them to output latch at one
   HC595Latch();
}

void Wait() {
   for(uint8_t i=0;i<30;i++) {
      _delay_loop_2(0);
   }
}

void main() {
   uint8_t led_pattern[8]={
                        0b10000001,
                        0b11000011,
                        0b11100111,
                        0b11111111,
                        0b01111110,
                        0b00111100,
                        0b00011000,
                        0b00000000,
                     };

   //Initialize HC595 system
   //Make the Data(DS), Shift clock (SH_CP), Store Clock (ST_CP) lines output
   HC595_DDR|=((1<<HC595_SH_CP_POS)|(1<<HC595_ST_CP_POS)|(1<<HC595_DS_POS));

   while(1) {
      for(uint8_t i=0;i<8;i++) {
         HC595Write(led_pattern[i]);   //Write the data to HC595
         Wait();                 //Wait 
      }
   }
}
