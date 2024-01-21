#include <Arduino.h>
#include <HD44780.hpp>
#include "uart.hpp"
#include "libADC.hpp"

int points = 1;
int levels = 1;


void gameStarting(){

  for(int i=0; i<3; i++){
    LCD_Clear();
    LCD_GoTo(0, 0);
    LCD_WriteText("Starting game");
    LCD_GoTo(13, 0);

    for(int i=0; i<2; i++){
      LCD_GoTo(13+i, 0);
      LCD_WriteText(".");
      _delay_ms(500);
    }
  }
}

void updatePoints(int points){

  LCD_GoTo(7, 0);
  char string[50];
  sprintf(string, "%d", points);
  LCD_WriteText(string);

}

void updateLevel(int level){

  LCD_GoTo(15, 0);
  char string[50];
  sprintf(string, "%d", level);
  LCD_WriteText(string);

}

void levelsCaligraphy( char word[], char response){

  LCD_GoTo(0, 1);
  LCD_WriteText(word);

  uint8_t start[] = "Introduce the correct letter\n\r";
  uart_send_string((uint8_t*)start);

  int answered = 1;
  uint8_t data = 0;

  while(answered){

    if (uart_read_count() > 0) {

      data = uart_read();
      uart_send_byte(data);

      if(data==response){

        LCD_GoTo(0, 1);
        LCD_WriteText("CORRECT          ");
        updatePoints(points++);
        updateLevel(levels++);
        answered=0;

      }else{

        LCD_GoTo(0, 1);
        LCD_WriteText("INCORRECT          ");
        updateLevel(levels++);
        answered=0;

      }
    }

  }

}

void caligraphyGame() {
  gameStarting();

  int level = 1;
  int points = 0;
  char string[50]; 
  char string2[50];

  LCD_Clear();
  LCD_GoTo(0, 0);
  LCD_WriteText("Fill the gaps");
  _delay_ms(2000);


  LCD_Clear();
  LCD_GoTo(9, 0);
  sprintf(string, "Level %d", level);
  LCD_WriteText(string);

  LCD_GoTo(0, 0);
  sprintf(string2, "Points %d", points);
  LCD_WriteText(string2);


  levelsCaligraphy("VOCA_ULARY", 'b');
  _delay_ms(500);
  levelsCaligraphy("SA_E      ", 'v');
  _delay_ms(500);
  levelsCaligraphy("IMPRO_E      ", 'v');
  _delay_ms(500);
  levelsCaligraphy("S_MPHONY      ", 'y');

  _delay_ms(1000);
  //FINALIZING GAME -- 

  LCD_Clear();
  LCD_GoTo(0, 0);
  LCD_WriteText("CONGRATULATIONS");
  LCD_GoTo(0, 1);
  LCD_WriteText("GAME FINISHED");
  


}

void levelsDisordered(char orderedWord[], char response[]){

  LCD_GoTo(0, 1);
  LCD_WriteText(orderedWord);

  uint8_t start[] = "Order the word\n\r";
  uart_send_string((uint8_t*)start);

  //SEND THE STRING PRESSING ENTER IN THE KEAYBOARD

  int answered = 1;
  uint8_t data = 0;
  char receivedString[50]={};
  int index=0;

  while(answered){

    if (uart_read_count() > 0) {

      data = uart_read();
      uart_send_byte(data);

      if(data=='\r'){
        answered=0;
      }else{
        receivedString[index++]=data;
      }


    }
  }

  receivedString[index] = '\0';


  if(strcmp(receivedString, response) == 0){

    LCD_GoTo(0, 1);
    LCD_WriteText("CORRECT!");
    updatePoints(points++);
    updateLevel(levels++);

  }else{

    LCD_GoTo(0, 1);
    LCD_WriteText("INCORRECT");
    updateLevel(levels++);

  }

  _delay_ms(1000);

}


void disorderedWords(){
  gameStarting();

  levels = 1;
  points = 1;
  char string[50]; 
  char string2[50];


  LCD_Clear();
  LCD_GoTo(0, 0);
  LCD_WriteText("Order the word");
  _delay_ms(2000);

  LCD_Clear();
  LCD_GoTo(9, 0);
  sprintf(string, "Level %d", levels);
  LCD_WriteText(string);

  LCD_GoTo(0, 0);
  sprintf(string2, "Points %d", points-1);
  LCD_WriteText(string2);

  //now plot the disordered word

  levelsDisordered("BEUSECA       ", "because");
  levelsDisordered("OGRANES       ", "oranges");
  levelsDisordered("RTEE      ", "tree");
  levelsDisordered("IKWI      ", "kiwi");

  //FINALIZING GAME -- 

  LCD_Clear();
  LCD_GoTo(0, 0);
  LCD_WriteText("CONGRATULATIONS");
  LCD_GoTo(0, 1);
  LCD_WriteText("GAME FINISHED");


  

}



int main(){

  unsigned char menuPosition=2;

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
    LCD_WriteText("Initializing");
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
    //RIGHT -- ENTER
    if ((raw >= 0 ) && (raw < 100)){

		  switch (menuPosition){
			case 1: disorderedWords(); break;
			case 2: caligraphyGame(); break;

		  }
		}


  }


      
        




  



} 