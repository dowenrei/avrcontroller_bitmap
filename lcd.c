/*  Author: Steve Gunn
 * 	Editied by : Ng Pei Herng
 * Licence: This work is licensed under the Creative Commons Attribution License.
 *           View this license at http://creativecommons.org/about/licenses/
 */

#include "avrlcd.h"
#include "ili934x.h"
#include "font.h"
#include "lcd.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>


lcd display = {LCDWIDTH, LCDHEIGHT, North, 0, 0, WHITE, BLACK};


void init_lcd()
{
	/* Disable JTAG in software, so that it does not interfere with Port C  */
	/* It will be re-enabled after a power cycle if the JTAGEN fuse is set. */
	MCUCR |= (1<<JTD);
	MCUCR |= (1<<JTD);
	
	/* Configure ports */
	CTRL_DDR = 0x7F;
	DATA_DDR = 0xFF;
	
	init_display_controller();
}

void set_orientation(orientation o)
{
	display.orient = o;
	write_cmd(MEMORY_ACCESS_CONTROL);
	if (o==North) { 
		display.width = LCDWIDTH;
		display.height = LCDHEIGHT;
		write_data(0x48);
	}
	else if (o==West) {
		display.width = LCDHEIGHT;
		display.height = LCDWIDTH;
		write_data(0xE8);
	}
	else if (o==South) {
		display.width = LCDWIDTH;
		display.height = LCDHEIGHT;
		write_data(0x88);
	}
	else if (o==East) {
		display.width = LCDHEIGHT;
		display.height = LCDWIDTH;
		write_data(0x28);
	}
	write_cmd(COLUMN_ADDRESS_SET);
	write_data16(0);
	write_data16(display.width-1);
	write_cmd(PAGE_ADDRESS_SET);
	write_data16(0);
	write_data16(display.height-1);
}

void fill_rectangle(rectangle r, uint16_t col)
{
	uint16_t x, y;
	write_cmd(COLUMN_ADDRESS_SET);
	write_data16(r.left);
	write_data16(r.right);
	write_cmd(PAGE_ADDRESS_SET);
	write_data16(r.top);
	write_data16(r.bottom);
	write_cmd(MEMORY_WRITE);
	for(x=r.left; x<=r.right; x++)
		for(y=r.top; y<=r.bottom; y++)
			write_data16(col);
}

void fill_rectangle_indexed(rectangle r, uint16_t* col)
{
	uint16_t x, y;
	write_cmd(COLUMN_ADDRESS_SET);
	write_data16(r.left);
	write_data16(r.right);
	write_cmd(PAGE_ADDRESS_SET);
	write_data16(r.top);
	write_data16(r.bottom);
	write_cmd(MEMORY_WRITE);
	for(x=r.left; x<=r.right; x++)
		for(y=r.top; y<=r.bottom; y++)
			write_data16(*col++);
}

void clear_screen()
{
	display.x = 0;
	display.y = 0;
	rectangle r = {0, display.width-1, 0, display.height-1};
	fill_rectangle(r, display.background);
}

void display_char(char c)
{
	uint16_t x, y;
	PGM_P fdata; //pointer to a string in program space
	uint8_t bits, mask;
	uint16_t sc=display.x, ec=display.x + 4, sp=display.y, ep=display.y + 7; //start page and end page
	if (c < 32 || c > 126) return;	//ASCII range of the bitmap
	fdata = (c - ' ')*5 + font5x7; //assign bitmap address for the character to fdata
	write_cmd(PAGE_ADDRESS_SET);	//set page address
	write_data16(sp);
	write_data16(ep);
	for(x=sc; x<=ec; x++) {		//write data from start col to end col on the lcd screen
		write_cmd(COLUMN_ADDRESS_SET);		//set col address
		write_data16(x); //start col and end col are set to be the same
		write_data16(x);
		write_cmd(MEMORY_WRITE);	   //write command
		bits = pgm_read_byte(fdata++); //read byte data from prgm memory in flash
		for(y=sp, mask=0x01; y<=ep; y++, mask<<=1) // mask with bits to write data bit by bit for 8 bits
			write_data16((bits & mask) ? display.foreground : display.background);	//if bit is one set pixel colour to foreground else background
	}
	write_cmd(COLUMN_ADDRESS_SET);
	write_data16(x);
	write_data16(x);
	write_cmd(MEMORY_WRITE);
	for(y=sp; y<=ep; y++)
		write_data16(display.background);

	display.x += 6;
	if (display.x >= display.width) { display.x=0; display.y+=24; }
}

//displaying a string
void display_string(char *str)
{
	uint8_t i;
	for(i=0; str[i]; i++)
		display_char(str[i]);
}

// wrd1e16

void draw_pikachu(){

	PGM_VOID_P f_da;
	uint8_t bits_high; uint8_t bits_low; uint16_t color;
	f_da = pikachu;
	// Pixel size 50 px* 50px
	uint8_t size = 50;
	for(uint16_t y=0; y<size; y++){
		for(uint16_t x=0; x<size; x++){

			bits_low = pgm_read_byte((f_da+(x*2+1)+size*2*y));
			bits_high = pgm_read_byte((f_da+x*2+size*2*y));
			write_cmd(PAGE_ADDRESS_SET);
			write_data16(y);
			write_data16(y);
			write_cmd(COLUMN_ADDRESS_SET);
			write_data16(x+100);
			write_data16(x+100);
			write_cmd(MEMORY_WRITE);
			color = bits_high << 8 | bits_low;
			write_data16(color);
}}}

void draw_bulbasaur(){

	PGM_VOID_P f_da;
	uint8_t bits_high; uint8_t bits_low; uint16_t color;
	f_da = bulbasaur;
	// Pixel size 50 px* 50px
	uint8_t size = 50;
	for(uint16_t y=0; y<size; y++){
		for(uint16_t x=0; x<size; x++){

			bits_low = pgm_read_byte((f_da+(x*2+1)+size*2*y));
			bits_high = pgm_read_byte((f_da+x*2+size*2*y));
			write_cmd(PAGE_ADDRESS_SET);
			//draw position
			write_data16(y+100);
			write_data16(y+100);
			write_cmd(COLUMN_ADDRESS_SET);
			write_data16(x+100);
			write_data16(x+100);
			write_cmd(MEMORY_WRITE);
			color = bits_high << 8 | bits_low;
			write_data16(color);
}}}


//writing a new line
void newline(){
	display.x = 0;
	if(display.y >= (display.height)-24) {display.y=0;}
	else{
	display.y = (display.y)+24;}
}
//setting cursor to beginning of the page
void cursor_to_begin(){
	display.x = 0;
	display.y = 0;
}
//setting placement of cursor
void cursor_to(uint16_t x, uint16_t y){
	display.x = x;
	display.y = y;
}

