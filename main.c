/*
 * Draw Image (Bitmap) on TFT Display
 * Author : Wen Rei
 */


#include <avr/io.h>
//#include <util/delay.h>
//#include <avr/interrupt.h>
#include "lcd.h"

int main(){


	init_lcd();
	clear_screen();
	draw_pikachu();
	draw_bulbasaur();

return(0);
}

