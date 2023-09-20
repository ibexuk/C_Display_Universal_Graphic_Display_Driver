/*
IBEX UK LTD http://www.ibexuk.com
Electronic Product Design Specialists
RELEASED SOFTWARE

The MIT License (MIT)

Copyright (c) 2013, IBEX UK Ltd, http://ibexuk.com

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
//Project Name:		UNIVERSAL GRAPHIC DISPLAY DRIVER
//SCREEN & DRIVER MODEL C CODE FILE



//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//---------- THIS FILE CONTAINS SCREEN & DRIVER MODEL SPECIFIC FUNCTIONS FOR	----------
//---------- SCREEN:		Midas MCCOG240160B6W-FPTLW							----------
//---------- Resolution:	240 x 160 pixel										----------
//---------- Driver IC:		UC1698U (internal)									----------
//---------- Colour:		1bit (monochrome)									----------
//---------- Interface:		8bit Parallel										----------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------




//----- INCLUDE FILES FOR THIS SOURCE CODE FILE -----
#include "main.h"					//Global data type definitions (see https://github.com/ibexuk/C_Generic_Header_File )
#define	DISPLAY_MODEL_C				//(Define used for following header file to flag that it is the header file for this source file)
#include "display-model.h"			//(Include header file for this source file)
#include "display.h"

//----- OTHER PROJECT FILES REQUIRED BY THIS SOURCE CODE FILE -----


//----- COMPILER LIBRARY FILES REQUIRED BY THIS SOURCE CODE FILE -----





//***********************************
//***********************************
//********** DISPLAY DELAY **********
//***********************************
//***********************************
//(The accuracy of this function is not important as long as calling with a value of 1 means the delay will
//be at least 1mS - it is only used for the initialise function)
void display_delay_ms (uint16_t delay_ms)
{
#ifdef DISPLAY_USING_MICROCHIP_XC32_COMPILER
	CORETIMER_DelayMs(delay_ms);
#else
	uint32_t count;

	while (delay_ms)
	{
		count = (INSTRUCTION_CLOCK_FREQUENCY / 1000) / 3;	//Divide by 3 as this loop will take a minimum of 3 steps
															//(may be more depending on compiler but we don't care)
		while (count)
		{
			count--;
		}
		delay_ms--;
	}
#endif
}



//****************************************
//****************************************
//********** DISPLAY INITIALISE **********
//****************************************
//****************************************
void display_model_initialise(void)
{

	//----- PAUSE IF NECESSARY FOR DISPLAY TO POWERUP -----
	display_delay_ms(10);

	//----- SETUP CONTROL PINS -----
	DISPLAY_CS(1);
	DISPLAY_RESET(0);
	DISPLAY_RD(1);
	DISPLAY_WR(1);
	DISPLAY_DATA_TO_OUTPUTS();


	//----- DO THE RESET PIN -----
	DISPLAY_RESET(0);							//Set low to reset

	DISPLAY_CS(0);
	display_delay_ms(5);

	DISPLAY_RESET(1);							//Set high to run

	//display_delay_ms(10);
	display_delay_ms(150);						//150mS specified in UC1611S datasheet examples

	//----- PAUSE #mS BEFORE ENABLING V0 (The negative driver voltage) -----
	//(Not necessary for this screen)

	//----- DO THE CONFIGURATION COMMAND SEQUENCE -----

    display_write_command(0x24);		//Set temp Comp
	
    display_write_command(0x2b);		//Set panel loading
	
    display_write_command(0x2f);		//Set pump control
	
    display_write_command(0x81);		//Set potentiometer
    display_write_command(DISPLAY_DEFAULT_CONTRAST);			//Value
	
    display_write_command(0xa3);		//Set Line Rate
	
    display_write_command(0xc0);		//Set LCD Map control
	
    display_write_command(0x02);		//Inversion
	
    display_write_command(0xd3);		//Set display pattern
	
    display_write_command(0xe9);		//LCD Bias Ratio
	
    display_write_command(0x89);		//RAM Address Control

    display_write_command(0xf1);		//Set Com End
    display_write_command(127);			//Set to 127
	
    display_write_command(0xa6 | 0);	//Set inverse display
	
    display_write_command(0xa8);		//set Display Enable (B&W)
    display_write_command(0xd1);		//Set display pattern (1 bit for 1 pixel)
    display_write_command(0xA9);		//Set display enable

}




//**********************************
//**********************************
//********** SET CONTRAST **********
//**********************************
//**********************************
void display_model_set_contrast(uint8_t contrast_value)
{
    display_write_command(0x81);			//Set potentiometer
    display_write_command(contrast_value);	//0-255, although usable range is much more limited. 160 typ
}





//*********************************
//*********************************
//********** WRITE BLOCK **********
//*********************************
//*********************************
//Writes a rectangular block of pixels
//This function simply writes individual pixels.  If your LCD controller has built in block drawing capabilities you could update this function
//to use them for improved speed.
void display_write_block (uint16_t x_start_coord, uint16_t y_start_coord, uint16_t x_end_coord, uint16_t y_end_coord, uint32_t colour)
{
	uint16_t x_coord;
	uint16_t y_coord;

	DISPLAY_CS(0);
	
	for (y_coord = y_start_coord; y_coord <= y_end_coord; y_coord++)		//Most natural visually is for the drawing to occur as a sweep down the screen.
	{
		for (x_coord = x_start_coord; x_coord <= x_end_coord; x_coord++)
		{
			display_write_pixel(x_coord, y_coord, colour);
		}
	}
	
	DISPLAY_CS(1);
}





//*********************************
//*********************************
//********** WRITE PIXEL **********
//*********************************
//*********************************
//Writes a single pixel at the specified address
//Assumes DISPLAY_CS is already active
//colour	| null | red | green | blue |
//This function converts the colour to the display colour data format
void display_write_pixel (uint16_t x_coord, uint16_t y_coord, uint32_t colour)
{
	uint32_t address = 1;
	uint16_t w_temp;
	uint8_t *p_local_ram_buffer;
	uint8_t display_working_byte_value;
	uint8_t bitmap_mask;
	
	//The driver always works with coordinate 0,0 in top left corner.
	//This function converts the required coordinates to the display address and then writes the pixel.
	//Adjust this function as requried for new screen models and orientations.
	//
	//UC1698U with monochrome LCD MCCOG240160B6W
	//	2 bytes per 3 pixels [R4,R3,R2,R1,R0,G5,G4,G3 | G2,G1,G0,B4,B3,B2,B1,B0]
	//	R, G and B are each a seperate pixel and both bytes need to be written to update the pixels.
	//	Even though we're monochrome the driver is greyscale based and you just have to put a >50%ish value to turn the pixel on.
	//	Byte1:Byte0 control the top left 3 pixels
	//	Byte3:Byte2 control the top left 3 pixels 1 row down
	//	Pixel displayed ordering is G|B|R
	//As we're lots of memory we have a local ram buffer (could read from the screen but not implemented that)



	//ENSURE X AND Y COORDINATES ARE IN RANGE
	if (x_coord >= DISPLAY_WIDTH_PIXELS)
		return;
	if (y_coord >= DISPLAY_HEIGHT_PIXELS)
		return;


	//------------------------------------------------------
	//----- UPDATE THE LOCAL RAM BUFFER WITH THE PIXEL -----
	//------------------------------------------------------

	//----- READ THE CURRENT DISPLAY BYTE VALUE -----
	p_local_ram_buffer = &display_copy_buffer[0];							//We use a pointer to allow for compilers that can't access large arrays directly
	#if (DISPLAY_HEIGHT_PIXELS & 0x0007)
		p_local_ram_buffer += x_coord * ((DISPLAY_HEIGHT_PIXELS >> 3) + 1);		//DISPLAY_WIDTH_PIXELS is not a multiple of 8 so need to add 1 for remainder
	#else
		p_local_ram_buffer += x_coord * (DISPLAY_HEIGHT_PIXELS >> 3);			//DISPLAY_WIDTH_PIXELS is a multiple of 8
	#endif
	p_local_ram_buffer += (y_coord >> 3);										//8 pixels per byte

	display_working_byte_value = *p_local_ram_buffer;


	//----- APPLY THE MASK -----
	//Get a mask for the pixel within the byte
	bitmap_mask = 0x01;
	w_temp = y_coord & 0x07;
	while (w_temp--)
		bitmap_mask <<= 1;


	display_working_byte_value &= ~bitmap_mask;
	if (colour)
		display_working_byte_value |= (0xff & bitmap_mask);

	//Store it back to the buffer
	*p_local_ram_buffer = display_working_byte_value;


}

//***************************************************************
//***************************************************************
//********** WRITE THE LOCAL RAM BUFFER TO THE DISPLAY **********
//***************************************************************
//***************************************************************
//This must be called to actually update the display after changing anything as because there are 3 pixels in each 2 byte word sent to the display it doesn't make sense to
//do this as each pixel is written because you'd end up with tripple the wanted number of writes and each with the address also needing to be written time!
void display_output_buffer (void)
{
	uint32_t x_coord = 0;			//0-255
	uint32_t y_coord = 0;			//0-127
	uint8_t data;

	//SET ADDRESS TO START
	display_write_command(0x70);	// (9)"Set Row Address MBS" --> 0
    display_write_command(0x60);	// (9)"Set Row Address LBS"	-->	0

    display_write_command(0x10);	// (4)"Set Column Address MBS" --> 0
    display_write_command(0x00);	// (4)"Set Column Address LBS" --> 0
	
	
	DISPLAY_CS(0);						//Commands can be sent to both controller IC's every time
	DISPLAY_CONTROL_DATA(1);			//CD high for a data


//Version for ribbon cable at top orientation  (ideal viewing angle):
	x_coord = 0;			//0-255
	y_coord = 0;			//0-127
	while (1)
	{
		//1 byte per 8 pixels [M7,M6,M5,M4,M3,M2,M1,M0]
		data = 0;
 
 
		if (display_get_pixel_value(x_coord, y_coord++))
			data |= 0x01;
		if (display_get_pixel_value(x_coord, y_coord++))
			data |= 0x02;
		if (display_get_pixel_value(x_coord, y_coord++))
			data |= 0x04;
		if (display_get_pixel_value(x_coord, y_coord++))
			data |= 0x08;
		if (display_get_pixel_value(x_coord, y_coord++))
			data |= 0x10;
		if (display_get_pixel_value(x_coord, y_coord++))
			data |= 0x20;
		if (display_get_pixel_value(x_coord, y_coord++))
			data |= 0x40;
		if (display_get_pixel_value(x_coord, y_coord++))
			data |= 0x80;

		//Send byte
		DISPLAY_WRITE_DATA(data);

		x_coord++;
		y_coord -= 8;
		if (x_coord >= DISPLAY_WIDTH_PIXELS)
		{
			x_coord = 0;
			y_coord += 8;
			if (y_coord >= DISPLAY_HEIGHT_PIXELS)
				break;
		}
	}
  
 
//Version for ribbon cable at bottom orientation (not ideal viewing angle):
/*
	x_coord = 255;			//0-255
	y_coord = 127;			//0-127
	while (1)
	{
		//1 byte per 8 pixels [M7,M6,M5,M4,M3,M2,M1,M0]
		data = 0;
 
 
		if (display_get_pixel_value(x_coord, y_coord--))
			data |= 0x01;
		if (display_get_pixel_value(x_coord, y_coord--))
			data |= 0x02;
		if (display_get_pixel_value(x_coord, y_coord--))
			data |= 0x04;
		if (display_get_pixel_value(x_coord, y_coord--))
			data |= 0x08;
		if (display_get_pixel_value(x_coord, y_coord--))
			data |= 0x10;
		if (display_get_pixel_value(x_coord, y_coord--))
			data |= 0x20;
		if (display_get_pixel_value(x_coord, y_coord--))
			data |= 0x40;
		if (display_get_pixel_value(x_coord, y_coord--))
			data |= 0x80;

		//Send byte
		DISPLAY_WRITE_DATA(data);

		x_coord--;
		y_coord += 8;
		if (x_coord >= DISPLAY_WIDTH_PIXELS)
		{
			x_coord = 255;
			y_coord -= 8;
			if (y_coord >= DISPLAY_HEIGHT_PIXELS)
				break;
		}
		
	}
*/


	DISPLAY_CS(1);
}


//*************************************
//*************************************
//********** GET PIXEL VALUE **********
//*************************************
//*************************************
uint8_t display_get_pixel_value (uint16_t x_coord, uint16_t y_coord)
{
	uint8_t *p_local_ram_buffer;
	uint8_t bitmap_mask;
	uint16_t w_temp;

	//----- READ THE CURRENT DISPLAY BYTE VALUE -----
	p_local_ram_buffer = &display_copy_buffer[0];								//We use a pointer to allow for compilers that can't access large arrays directly
	#if (DISPLAY_HEIGHT_PIXELS & 0x0007)
		p_local_ram_buffer += x_coord * ((DISPLAY_HEIGHT_PIXELS >> 3) + 1);		//DISPLAY_WIDTH_PIXELS is not a multiple of 8 so need to add 1 for remainder
	#else
		p_local_ram_buffer += x_coord * (DISPLAY_HEIGHT_PIXELS >> 3);			//DISPLAY_WIDTH_PIXELS is a multiple of 8
	#endif
	p_local_ram_buffer += (y_coord >> 3);										//8 pixels per byte


	//----- APPLY THE MASK -----
	//Get a mask for the pixel within the byte
	bitmap_mask = 0x01;
	w_temp = y_coord & 0x07;
	while (w_temp--)
		bitmap_mask <<= 1;

	if ((*p_local_ram_buffer) & bitmap_mask)
		return(1);
	else
		return(0);
}

//**********************************************
//**********************************************
//********** WRITE COMMAND TO DISPLAY **********
//**********************************************
//**********************************************
//Modify this routine to suit the screen in use
//This routine is usually that same as the instructions in the write part of display_write_bitmap_byte
//The only difference may be setting the A0 data / command line.  However to allow for displays that aren't
//so normal we use this write command routine.
void display_write_command (uint8_t data)
{
	//Setup control pins
	DISPLAY_CS(0);						//Commands can be sent to both controller IC's every time
	DISPLAY_CONTROL_DATA(0);			//CD low for a command

	//Send byte
	DISPLAY_WRITE_DATA(data);

	DISPLAY_CS(1);
}




