#include <Arduino.h>
#include <HD44780.hpp>
#include "uart.hpp"
#include "libADC.hpp"

int main(){

  unsigned char menuPosition=2;
  uint8_t menu1[] = "CALIGRAPHY GAME\n\r";
  uint8_t menu2[] = "DISORDERED WORDS\n\r";
  uint8_t refreshLCD=1;
  uint16_t raw;
  char buf[7];

  uint8_t data = 0;
  const uint8_t start[] = "Starting the game\n\r";
  
  uart_init(9600, 0);
  sei();
  uart_send_string((uint8_t*)start);
	ADC_Init();
  LCD_Initalize();


  for(int i=0; i<3; i++){

    LCD_Clear();
    LCD_GoTo(0, 0);
    LCD_WriteText("Starting game");
    LCD_GoTo(13, 0);

    for(int i=0; i<3; i++){
      LCD_GoTo(13+i, 0);
      LCD_WriteText(".");
      _delay_ms(500);
    }

  }
  
  LCD_Clear();
  LCD_GoTo(0,0);
  LCD_WriteText("CHOOSE A GAME");

  while (1) {

    ADCSRA |= (1<<ADSC);
		while(ADCSRA & (1<<ADSC));
		raw = ADC;

    // UP
		if ((100 < raw) && (raw < 250) ) {
			menuPosition=2;
			refreshLCD=1;
		}

    // DOWN
		if ((250 < raw) && (raw < 350)) {
			menuPosition=1;
			refreshLCD=1;
		}

    if (refreshLCD){
			LCD_GoTo(0,1);
			//LCD_WriteCommand(HD44780_CLEAR);
			_delay_ms(100);

      if(menuPosition==2){

			  LCD_WriteText("CALIGRAPHY GAME");
			  _delay_ms(100);
      }

      
      if(menuPosition==1){

			  LCD_WriteText("DISORDERED WORDS");
			  _delay_ms(100);
        
      }

			refreshLCD=0;

		}


  }


      
        




  



}