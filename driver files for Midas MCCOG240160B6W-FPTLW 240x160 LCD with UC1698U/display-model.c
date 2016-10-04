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
void display_delay_ms (WORD delay_ms)
{
	DWORD count;

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

void display_write_data (BYTE data)
{
	mPORTEWrite(data);

	DISPLAY_CS(0);
	DISPLAY_CONTROL_DATA(1);

	DISPLAY_WR(0);

	DISPLAY_BUS_ACCESS_DELAY;

	DISPLAY_WR(1);

	

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

	display_delay_ms(10);

	//----- PAUSE #mS BEFORE ENABLING V0 (The negative driver voltage) -----
	//(Not necessary for this screen)

	//----- DO THE CONFIGURATION COMMAND SEQUENCE -----
	display_write_command(0xe2);				//System Reset
	display_delay_ms(1);
	display_delay_ms(1);

	display_write_command(0x28 | 3);			//0x28 = power control command (13nF<LCD<=22nF; Internal Vlcd(*10))
	display_delay_ms(1);

	display_write_command(0x24 | 1);			//0x24 = temperature control (-0.05%/C)
	display_delay_ms(1);

	display_write_command(0xc0 | 2);			//0x20 = LCD Mappiing Control (SEG1-->SEG384; COM160-->COM1)
	display_delay_ms(1);

	display_write_command(0x88 | 3);			//0x88 = RAM Address Control (up to down, automatically +1)
	display_delay_ms(100);

	display_write_command(0xD0 | 0);			//Color Pattern (B-G-R-B-G-R...)
	display_delay_ms(1);

	display_write_command(0xD4 | 2);			//Color Mode (64K color, R4--R0,G5--G0,B4--B0)
	display_delay_ms(1);

	display_write_command(0xD8 | 5);			//COM Scan Function (LRC: AEBCD--AEBCD;  Disable FRC;   Enable SEG PWM)
	display_delay_ms(1);

	display_write_command(0xA0 | 1);			//Line Rate (Frame frequency 30.5 KIPS)
	display_delay_ms(1);

	display_write_command(0xE8 | 3);			//LCD Bias Ratio (Bias 1/12)
	display_delay_ms(1);

	display_write_command(0x81);				//Set Vbias Potentiometer
	display_delay_ms(1);
	display_write_command(35);					//(0~255 for selection)
	display_delay_ms(1);

	display_write_command(0xF1);				//Set COM End
	display_delay_ms(1);
	display_write_command(159);
	display_delay_ms(1);

	display_write_command(0xC8);				//Set N-Line Inversion
	display_delay_ms(1);
	display_write_command(0x00);				//(Disable NIV)
	display_delay_ms(1);

	display_write_command(0x84);				//Set Partial Display Control (Disable Partical function)
	display_delay_ms(1);
	display_write_command(0xA6);				//Set Inverse Display (Normal display)
	display_delay_ms(1);
	display_write_command(0xAD);				//Set Display Enable (Disable Sleep; Disable Green enhance; Disable gray scale)
	display_delay_ms(1);

}




//**********************************
//**********************************
//********** SET CONTRAST **********
//**********************************
//**********************************
void display_model_set_contrast(BYTE contrast_value)
{
	display_write_command(0x81);				//Set Vbias Potentiometer
	display_write_command(35);					//(0~255 for selection)
}





//*********************************
//*********************************
//********** WRITE BLOCK **********
//*********************************
//*********************************
//Writes a rectangular block of pixels
//This function simply writes individual pixels.  If your LCD controller has built in block drawing capabilities you could update this function
//to use them for improved speed.
void display_write_block (WORD x_start_coord, WORD y_start_coord, WORD x_end_coord, WORD y_end_coord, DWORD colour)
{
	WORD x_coord;
	WORD y_coord;

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
void display_write_pixel (WORD x_coord, WORD y_coord, DWORD colour)
{
	DWORD address = 1;
	WORD w_temp;
	BYTE *p_local_ram_buffer;
	BYTE display_working_byte_value;
	BYTE bitmap_mask;
	
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
//This must be called to actually update the display after changing anything as because there are 3 pixels in each 2 byute word sent to the display it doesn't make sense to
//do this as each pixel is written because you'd end up with tripple the wanted number of writes and each with the address also needing to be written time!
void display_output_buffer (void)
{
	DWORD x_coord = 0;			//0-239
	DWORD y_coord = 0;			//0-159
	WORD data;

	//SET ADDRESS TO START
	display_write_command(0x70);	// (9)"Set Row Address MBS" --> 0
    display_write_command(0x60);	// (9)"Set Row Address LBS"	-->	0

    display_write_command(0x10);	// (4)"Set Column Address MBS" --> 0
    display_write_command(0x00);	// (4)"Set Column Address LBS" --> 0

	DISPLAY_CS(0);						//Commands can be sent to both controller IC's every time
	DISPLAY_CONTROL_DATA(1);			//CD high for a data

	while (1)
	{
		//2 bytes per 3 pixels [R4,R3,R2,R1,R0,G5,G4,G3 | G2,G1,G0,B4,B3,B2,B1,B0]
		data = 0;
		if (display_get_pixel_value(x_coord++, y_coord))
			data |= 0x07e0;
		if (display_get_pixel_value(x_coord++, y_coord))
			data |= 0x001f;
		if (display_get_pixel_value(x_coord++, y_coord))
			data |= 0xf800;


		//Send byte
		DISPLAY_WRITE_DATA((BYTE)(data & 0x00ff));
		DISPLAY_WRITE_DATA((BYTE)(data >> 8));



		x_coord -= 3;
		y_coord += 1;
		if (y_coord >= DISPLAY_HEIGHT_PIXELS)
		{
			y_coord = 0;
			x_coord += 3;
			if (x_coord >= DISPLAY_WIDTH_PIXELS)
				break;
		}
	}
	DISPLAY_CS(1);
}


//*************************************
//*************************************
//********** GET PIXEL VALUE **********
//*************************************
//*************************************
BYTE display_get_pixel_value (WORD x_coord, WORD y_coord)
{
	BYTE *p_local_ram_buffer;
	BYTE bitmap_mask;
	WORD w_temp;

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
//This routine is usually that same as the instrucitons in the write part of display_write_bitmap_byte
//The only difference may be setting the A0 data / command line.  However to allow for displays that aren't
//so normal we use this write command routine.
void display_write_command (BYTE data)
{
	//Setup control pins
	DISPLAY_CS(0);						//Commands can be sent to both controller IC's every time
	DISPLAY_CONTROL_DATA(0);			//CD low for a command

	//Send byte
	DISPLAY_WRITE_DATA(data);

	DISPLAY_CS(1);
}




