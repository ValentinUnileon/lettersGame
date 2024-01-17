#include <Arduino.h>
#include <HD44780.hpp>
#include "uart.hpp"

int main(){

  uint8_t data = 0;
  const uint8_t start[] = "Starting the game\n\r";
  
  uart_init(9600, 0);
  sei();
  uart_send_string((uint8_t*)start);

  LCD_Initalize();


  for(int i=0; i<3; i++){

    LCD_Clear();
    LCD_GoTo(0, 0);
    LCD_WriteText("Starting game");
    LCD_GoTo(13, 0);

    for(int i=0; i<3; i++){
      LCD_GoTo(13+i, 0);
      LCD_WriteText(".");
      _delay_ms(1000);
    }

  }


    while (1) {

      if (uart_read_count() > 0) {

      }


    }


      
        




  



}