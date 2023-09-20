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
//---------- SCREEN:		Local Memory (e.g. for writing to a LED matrix)		----------
//---------- Resolution:	# x # pixel											----------
//---------- Driver IC:		None												----------
//---------- Color:			32bit (Null|8Red|8Green|8Blue)						----------
//---------- Interface:		None												----------
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







//****************************************
//****************************************
//********** DISPLAY INITIALISE **********
//****************************************
//****************************************
void display_model_initialise(void)
{

}





//*********************************
//*********************************
//********** WRITE BLOCK **********
//*********************************
//*********************************
//Writes a rectangular block of pixels
//This function simply writes individual pixels.  If your LCD controller has built in block drawing capabilities you could update this function
//to use them for improved speed.
void display_write_block (uint16_t x_start_coord, uint16_t y_start_coord, uint16_t x_end_coord, uint16_t y_end_coord, uint32_t color)
{
	uint16_t x_coord;
	uint16_t y_coord;


	for (y_coord = y_start_coord; y_coord <= y_end_coord; y_coord++)		//Most natural visually is for the drawing to occur as a sweep down the screen.
	{
		for (x_coord = x_start_coord; x_coord <= x_end_coord; x_coord++)
		{
			//display_write_pixel(x_coord, y_coord, color);
			display_buffer[x_coord + (y_coord * DISPLAY_WIDTH_PIXELS)] = color;
		}
	}
}





//*********************************
//*********************************
//********** WRITE PIXEL **********
//*********************************
//*********************************
//Writes a single pixel at the specified address
//Assumes DISPLAY_CS is already active
//color		| null | red | green | blue |
//This function converts the colour to the display colour data format
void display_write_pixel (uint16_t x_coord, uint16_t y_coord, uint32_t color)
{
	
	//The driver always works with coordinate 0,0 in top left corner.
	//This function converts the required coordinates to the display address and then writes the pixel.
	//Adjust this function as requried for new screen models and orientations.
	//

	if (x_coord >= DISPLAY_WIDTH_PIXELS)
		return;
	if (y_coord >= DISPLAY_HEIGHT_PIXELS)
		return;

	//----- WRITE PIXEL -----
	display_buffer[x_coord + (y_coord * DISPLAY_WIDTH_PIXELS)] = color;
}
	














