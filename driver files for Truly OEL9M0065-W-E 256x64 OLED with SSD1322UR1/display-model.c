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
//---------- SCREEN:		Truly OEL9M0065-W-E									----------
//---------- Resolution:	256 x 64 pixel										----------
//---------- Driver IC:		Solomon Systec SSD1322UR1 (internal)				----------
//---------- Colour:		4bit grayscale										----------
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
}




//****************************************
//****************************************
//********** DISPLAY INITIALISE **********
//****************************************
//****************************************
void display_model_initialise(void)
{

	
	//##### DEBUG PIN TEST #####
	//Comment out for normal operation
	//Can be useful to quickly check each pin for shorts etc.  Each pin is pulsed one by one in a continuous loop so it can be probed with a scope.
	/*
	uint8_t b_temp;
	DISPLAY_RESET(0);
	DISPLAY_CS(1);
	while (1)
	{
		ENABLE_INT;
		ClearWDT();
		DISABLE_INT;

		DISPLAY_CS(0);
		DISPLAY_CS(1);

		DISPLAY_REGISTER_SELECT(0);
		DISPLAY_REGISTER_SELECT(1);

		DISPLAY_RD(1);
		DISPLAY_RD(0);
		
		DISPLAY_WR(1);
		DISPLAY_WR(0);
		
		DISPLAY_RESET(1);
		DISPLAY_RESET(0);
		
		//DISPLAY_VCC(1);
		//DISPLAY_VCC(0);

		
		DISPLAY_DATA_TO_OUTPUTS();
		for (b_temp = 0x01; b_temp; b_temp <<= 1)
		{
			DISPLAY_WRITE_DATA(b_temp);
		}
		
		DISPLAY_WRITE_DATA(b_temp);
	}
	*/
	//#########################



	//----- PAUSE IF NECESSARY FOR DISPLAY TO POWERUP -----
	display_delay_ms(10);
	
	//----- RESET DISPLAY -----
	DISPLAY_RESET(0);
	DISPLAY_CS(1);
	DISPLAY_RD(1);
	DISPLAY_WR(1);
    display_delay_ms(1);					//min 100uS
    DISPLAY_RESET(1);						//min 100uS
    display_delay_ms(1);
    
    DISPLAY_VCC(1);
    display_delay_ms(5);

	DISPLAY_CS(0);
	DISPLAY_DATA_TO_OUTPUTS();

	display_write_command(0xFD);			//Command lock setting
	display_write_data(0x12);
	
	display_write_command(0xAE);			//Display OFF
	
	display_write_command(0x15);			//Column address setting
	display_write_data(0x1C);
	display_write_data(0x5B);
	
	display_write_command(0x75);			//Row address setting
	display_write_data(0x00);
	display_write_data(0x3F);
	
	display_write_command(0xA0);			//Re-map dual com mode
	display_write_data(0x14);
	display_write_data(0x11);				//Enable dual com mode
	
	display_write_command(0xA1);			//Display start line
	display_write_data(0x00);
	
	display_write_command(0xA2);			//Display offset
	display_write_data(0x00);
	
	display_write_command(0xA6);			//Normal display
	
	//display_write_command(0xA8);			//Partial display setting
	
	//display_write_data(0x00);				//Start row
	
	//display_write_data(0x3F);				//End row
	
	//display_write_command(0xA9);			//Exit partial display mode
	
	display_write_command(0xAB);			//Function selection
	
	display_write_data(0x01);				//Enable internal vdd
	
	display_write_command(0xB1);			//Phase length setting
	
	display_write_data(0x74);				//Phase1 = 9dclk, phase2 = 7dclk
	
	display_write_command(0xB3);			//Front clock divider & oscillator freq
	display_write_data(0x51);
	
	display_write_command(0xB4);			//Display enhancement A
	
	display_write_data(0xA2);				//INTERNAL VSL
	
	display_write_data(0xB5);				//Normal or 11111101 to enhance low GS
	
	//display_write_command(0xB5);			//GPIO setting
	//display_write_data(0x0a);
	
	display_write_command(0xB6);			//Second precharge period setting
	display_write_data(0x08);
	
	display_write_command(0xBB);			//Set precharge voltage
	display_write_data(0x17);
	
	display_write_command(0xBE);			//Set VCOMH voltage
	
	display_write_data(0x04);				//0.80 * VCC
	
	display_write_command(0xC1);			//Contrast set
	display_write_data(DEFAULT_CONTRAST);
		
	display_write_command(0xC7);			//Master current set
	display_write_data(0x0F);
	
	display_write_command(0xCA);			//Mux set
	display_write_data(0x3F);
	
	display_write_command(0xD1);			//Display enhancement B
	display_write_data(0xA2);
	display_write_data(0x20);
	
	display_write_command(0x00);			//Enable gray scale setting
	
	display_write_command(0xB8);			//Gray scale setting
	
	display_write_data(0x00);				//GS1=0
	display_write_data(0x0C);				//GS2=8
	display_write_data(0x18);				//GS3=8
	display_write_data(0x24);				//GS4=8
	display_write_data(0x30);				//GS5=9
	display_write_data(0x3C);				//GS6=9
	display_write_data(0x48);				//GS7=9
	display_write_data(0x54);				//GS8=10
	display_write_data(0x60);				//GS9=10
	display_write_data(0x6C);				//GS10=10
	display_write_data(0x78);				//GS11=10
	display_write_data(0x84);				//GS12=10
	display_write_data(0x90);				//GS13=11
	display_write_data(0x9C);				//GS14=11
	display_write_data(0xA8);				//GS15=12

	display_write_command(0xAF);			//Display ON
	//SEG/COM will be ON after 200ms(tAF).



	DISPLAY_CS(1);
}



//********************************
//********************************
//********** POWER DOWN **********
//********************************
//********************************
void display_power_down (void)
{
	DISPLAY_CS(0);
	DISPLAY_DATA_TO_OUTPUTS();
	display_write_command(0xae);		//Display off
	DISPLAY_CS(1);
	
	DISPLAY_VCC(0);
    display_delay_ms(150);				//Typical tOFF=100ms
}



//**********************************
//**********************************
//********** SET CONTRAST **********
//**********************************
//**********************************
//0x00 - 0xff
void display_set_contrast (uint8_t contrast)
{
	DISPLAY_CS(0);
	display_write_command(0xC1);			//Contrast set
	display_write_data(contrast);
	DISPLAY_CS(1);
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
	DISPLAY_DATA_TO_OUTPUTS();
	
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
	uint8_t x_coord_use;
	uint8_t y_coord_use;
	DWORD_VAL input_colour;
	uint8_t output_colour;
	uint8_t data0;
	uint8_t data1;
	
	//The driver always works with coordinate 0,0 in top left corner.
	//This function converts the required coordinates to the display address and then writes the pixel.
	//Adjust this function as requried for new screen models and orientations.
	//
	//SSD1926 with QVGA LCD
	//Colour is 16 bit:- 5Red|6Green|5Blue
	//Address 0 = top left corner.
	//2 address locations per pixel



	if (x_coord >= DISPLAY_WIDTH_PIXELS)
		return;
	if (y_coord >= DISPLAY_HEIGHT_PIXELS)
		return;
		



	//Colour is 4 bit grayscale
	input_colour.Val = colour;
	
	input_colour.v[0] >>= 4;
	input_colour.v[1] >>= 4;
	input_colour.v[2] >>= 4;
	
	output_colour = input_colour.v[0];			//Blue
	
	if (output_colour < input_colour.v[1])
		output_colour = input_colour.v[1];		//Green
		
	if (output_colour < input_colour.v[2])
		output_colour = input_colour.v[2];		//Red


	//Therre are 4 Pixels in each column, 2 Pixels per byte read
	

	//----- DO X COORD -----
	x_coord_use = x_coord >> 2;			//Adjust for correct orientation if necessary.  4 pixels per column.
	
	//----- DO Y COORD -----
	y_coord_use = y_coord;				//Adjust for correct orientation if necessary



	//----------------
	//----- READ -----
	//----------------

	//----- SET ADDRESS -----
	display_write_command(0x15);			//Set column address
	display_write_data(x_coord_use + 0x1c);

	display_write_command(0x75);			//Set row address
	display_write_data(y_coord_use);


	//----- READ PIXELS -----
	display_write_command(0x5d);		//Read ram
	
	DISPLAY_REGISTER_SELECT(1);			//Data
	DISPLAY_DATA_TO_INPUTS();

	DISPLAY_RD(0);						//Dummy read
	DISPLAY_RW_ACCESS_DELAY();
	DISPLAY_RD(1);

	DISPLAY_RD(0);						//Get pixels 0, 1
	DISPLAY_RW_ACCESS_DELAY();
	DISPLAY_READ_DATA(data0);
	DISPLAY_RD(1);

	DISPLAY_RD(0);						//Get pixels 2, 3
	DISPLAY_RW_ACCESS_DELAY();
	DISPLAY_READ_DATA(data1);
	DISPLAY_RD(1);
	
	DISPLAY_DATA_TO_OUTPUTS();
	
	
	//----- MODIFY PIXEL -----
	switch (x_coord & 0x03)
	{
	case 0:
		data0 &= 0x0f;
		data0 |= output_colour << 4;
		break;
	case 1:
		data0 &= 0xf0;
		data0 |= output_colour;
		break;
	case 2:
		data1 &= 0x0f;
		data1 |= output_colour << 4;
		break;
	case 3:
		data1 &= 0xf0;
		data1 |= output_colour;
		break;
	}
	

	//----- SET ADDRESS -----
	display_write_command(0x15);			//Set column address
	display_write_data(x_coord_use + 0x1c);

	display_write_command(0x75);			//Set row address
	display_write_data(y_coord_use);


	//----- WRITE PIXELS -----
	display_write_command(0x5c);		//Write ram
	display_write_data(data0);
	display_write_data(data1);


	

}






//************************************
//************************************
//*********** WRITE COMMAND **********
//************************************
//************************************
void display_write_command (uint8_t command)
{
	DISPLAY_REGISTER_SELECT(0);		//Command
	
	DISPLAY_WRITE_DATA(command);
	DISPLAY_RW_ACCESS_DELAY();
	DISPLAY_WR(0);
	DISPLAY_RW_ACCESS_DELAY();
	DISPLAY_WR(1);
}



//*********************************
//*********************************
//*********** WRITE DATA **********
//*********************************
//*********************************
void display_write_data (uint8_t data)
{
	DISPLAY_REGISTER_SELECT(1);		//Data
	
	DISPLAY_WRITE_DATA(data);
	DISPLAY_RW_ACCESS_DELAY();
	DISPLAY_WR(0);
	DISPLAY_RW_ACCESS_DELAY();
	DISPLAY_WR(1);
}


