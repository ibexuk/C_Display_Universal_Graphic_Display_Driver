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


//Address is 1 addresses per pixel starting from zero

//Coordinates are always 0,0 in top left corner.  Our screen in landscape:
//0,0		319,0
//0,239		319,239



/*
###############################################
##### TO USE THIS DRIVER IN A NEW PROJECT #####
###############################################
- Check this header file and the display.h header file for defines to setup and any usage notes
- Configure the control pins as outputs in your applications initialisation.


//NOTES
//- This display has a power down sequence requirment.  Call display_power_down() to power down Vcc before other power is removed.
//- To change the contrast the display_set_contrast() function may be called.
*/




/* ADAPTED DISPLAY TEST FUNCTION TO REPLACE FUNCITON IN DISPLAY.C FOR THIS DISPLAY:-
//**********************************
//**********************************
//********** DISPLAY TEST **********
//**********************************
//**********************************
//This function is a handy way of checking a new screen is setup correctly.  It draws a 1 pixel wide border round the edge of the screen and the following color fade bars:
//	Red
//	Green
//	Blue
//	Turquoise
//	Violet
//	Yellow
//	White
void display_test (void)
{
	DWORD_VAL colour;
	uint16_t x_coord = 0;
	uint16_t count;



	display_clear_screen(DISPLAY_COLOUR_BLACK);

	//---------------------------------------
	//----- DRAW BOX AROUND SCREEN EDGE -----
	//---------------------------------------

	//Single pixel wide line down left edge
	display_write_block(0, 0, 0, (DISPLAY_HEIGHT_PIXELS - 1), DISPLAY_COLOUR_WHITE);
	
	//Single pixel wide line down right edge
	display_write_block((DISPLAY_WIDTH_PIXELS - 1), 0, (DISPLAY_WIDTH_PIXELS - 1), (DISPLAY_HEIGHT_PIXELS - 1), DISPLAY_COLOUR_WHITE);
	
	//Single pixel wide line across top edge
	display_write_block(0, 0, (DISPLAY_WIDTH_PIXELS - 1), 0, DISPLAY_COLOUR_WHITE);
	
	//Single pixel wide line across bottom edge
	display_write_block(0, (DISPLAY_HEIGHT_PIXELS - 1), (DISPLAY_WIDTH_PIXELS - 1), (DISPLAY_HEIGHT_PIXELS - 1), DISPLAY_COLOUR_WHITE);


	//---------------------------------
	//----- DRAW COLOUR FADE BARS -----
	//---------------------------------
	DISPLAY_CS(0);

	//RED COLOUR FADE
	colour.Val = 0;
	x_coord = 4;
	for (count = 0; count < 128; count++)
	{
		display_write_pixel(x_coord, 4, colour.Val);
		display_write_pixel(x_coord, 5, colour.Val);
		display_write_pixel(x_coord, 6, colour.Val);
		display_write_pixel(x_coord, 7, colour.Val);
		
		colour.v[2] += 2;
		x_coord++;
	}
	
	//GREEN COLOUR FADE
	colour.Val = 0;
	x_coord = 4;
	for (count = 0; count < 128; count++)
	{
		display_write_pixel(x_coord, 9, colour.Val);
		display_write_pixel(x_coord, 10, colour.Val);
		display_write_pixel(x_coord, 11, colour.Val);
		display_write_pixel(x_coord, 12, colour.Val);
		
		colour.v[1] += 2;
		x_coord++;
	}
	
	//BLUE COLOUR FADE
	colour.Val = 0;
	x_coord = 4;
	for (count = 0; count < 128; count++)
	{
		display_write_pixel(x_coord, 14, colour.Val);
		display_write_pixel(x_coord, 15, colour.Val);
		display_write_pixel(x_coord, 16, colour.Val);
		display_write_pixel(x_coord, 17, colour.Val);
		
		colour.v[0] += 2;
		x_coord++;
	}
	
	
	//display_foreground_colour = DISPLAY_COLOUR_WHITE;
	//display_write_pixel(address, 239, display_foreground_colour);
	
	DISPLAY_CS(1);
}
*/


//*****************************
//*****************************
//********** DEFINES **********
//*****************************
//*****************************
#ifndef DISPLAY_MODEL_C_INIT				//(Include this section only once for each source file that includes this header file)
#define	DISPLAY_MODEL_C_INIT


//----------------------------------------------
//----- DEFINE TARGET COMPILER & PROCESSOR -----
//----------------------------------------------
//(ONLY 1 SHOULD BE INCLUDED, COMMENT OUT OTHERS)
#define	DISPLAY_USING_MICROCHIP_C30_COMPILER
//<< add other compiler types here


//-------------------------
//----- DISPLAY SETUP -----
//-------------------------
#define	DISPLAY_WIDTH_PIXELS			256			//Width is the X coordinate
#define	DISPLAY_HEIGHT_PIXELS			64			//Height is the Y coordinate

#define	DEFAULT_CONTRAST				0x7f		//0x00 - 0xff


//------------------------
//----- USER OPTIONS -----
//------------------------
//#define	DISPLAY_BUS_INTERFACE_IS_BIG_ENDIAN		//Comment out if little endian selected using SSD1926 pin CNF4 (CNF4=H=BigEndian)


#ifdef DISPLAY_USING_MICROCHIP_C30_COMPILER
//########################################
//########################################
//##### USING MICROCHIP C30 COMPILER #####
//########################################
//########################################


//----------------------
//----- IO DEFINES -----
//----------------------
//BS1:0 = 10 = 8bit, 8080parallel

//CONTROL PINS:-
#define DISPLAY_CS(state)					(state ? (_LATE7 = 1) : (_LATE7 = 0))				//0 = Select Controller
#define DISPLAY_REGISTER_SELECT(state)		(state ? (_LATE6 = 1) : (_LATE6 = 0))				//0 = Command, 1 = Data (DC pin)
#define DISPLAY_RD(state)					(state ? (_LATG15 = 1) : (_LATG15 = 0))				//0 = Read (E pin in 6800 mode)
#define DISPLAY_WR(state)					(state ? (_LATE5 = 1) : (_LATE5 = 0))				//0 = Read (R/W pin in 6800 mode)
#define	DISPLAY_RESET(state)				(state ? (_LATC1 = 1) : (_LATC1 = 0))				//0 = Reset Controller
#define	DISPLAY_VCC(state)					(state ? (_LATC2 = 1) : (_LATC2 = 0))				//1 = 14V Vcc on


//PORTS:-
#define	DISPLAY_DATA_TO_OUTPUTS()			TRISD &= 0xff00
#define	DISPLAY_DATA_TO_INPUTS()			TRISD |= 0x00ff
#define	DISPLAY_WRITE_DATA(data)			LATD &= 0xff00; LATD |= data					//Write 8bit word to controller
#define	DISPLAY_READ_DATA(data)				data = PORTD & 0x00ff							//Read 16bit word from controller

#define DISPLAY_RW_ACCESS_DELAY()			Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop()


//###############################################
//###############################################
//##### END OF USING MICROCHIP C30 COMPILER #####
//###############################################
//###############################################
#endif		//#ifdef DISPLAY_USING_MICROCHIP_C30_COMPILER







#endif //#ifndef DISPLAY_MODEL_C_INIT




//*******************************
//*******************************
//********** FUNCTIONS **********
//*******************************
//*******************************
#ifdef DISPLAY_MODEL_C
//-----------------------------------
//----- INTERNAL ONLY FUNCTIONS -----
//-----------------------------------
void display_delay_ms (uint16_t delay_ms);
void display_write_command (uint8_t command);
void display_write_data (uint8_t data);


//-----------------------------------------
//----- INTERNAL & EXTERNAL FUNCTIONS -----
//-----------------------------------------
//(Also defined below as extern)
void display_model_initialise(void);
void display_power_down (void);
void display_set_contrast (uint8_t contrast);
void display_write_pixel (uint16_t x_coord, uint16_t y_coord, uint32_t colour);
void display_write_block (uint16_t x_start_coord, uint16_t y_start_coord, uint16_t x_end_coord, uint16_t y_end_coord, uint32_t colour);


#else
//------------------------------
//----- EXTERNAL FUNCTIONS -----
//------------------------------
extern void display_model_initialise(void);
extern void display_power_down (void);
extern void display_set_contrast (uint8_t contrast);
extern void display_write_pixel (uint16_t x_coord, uint16_t y_coord, uint32_t colour);
extern void display_write_block (uint16_t x_start_coord, uint16_t y_start_coord, uint16_t x_end_coord, uint16_t y_end_coord, uint32_t colour);


#endif




//****************************
//****************************
//********** MEMORY **********
//****************************
//****************************
#ifdef DISPLAY_MODEL_C
//--------------------------------------------
//----- INTERNAL ONLY MEMORY DEFINITIONS -----
//--------------------------------------------


//--------------------------------------------------
//----- INTERNAL & EXTERNAL MEMORY DEFINITIONS -----
//--------------------------------------------------
//(Also defined below as extern)


#else
//---------------------------------------
//----- EXTERNAL MEMORY DEFINITIONS -----
//---------------------------------------


#endif


