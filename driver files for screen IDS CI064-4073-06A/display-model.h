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
//---------- SCREEN:		Intelligent Display Solutions: CI064-4073-06A		----------
//---------- Resolution:	256 x 128 pixel										----------
//---------- Driver IC:		UC1611S (internal)									----------
//---------- Colour:		1bit (monochrome)									----------
//---------- Interface:		8bit Parallel										----------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

//This screen is a problem because there are 8 pixels in each 1 byte word sent to the display it doesn't make sense to
//do this as each pixel is written because you'd end up with 8x the wanted number of writes and each with the address
//also needing to be written each time!
//Instead we use a local ram buffer and call this to output to the screen: display_output_buffer()


//The UC1611s driver chip is wired inside the display with BM1 connected to VDD, so 16-bit operation is not used. CS1=H hardwired internally.
//This driver is designed for D15, D13 and BM0 connected to 0V so that 8-bit 8080 mode is selected.


//Coordinates are always 0,0 in top left corner.  Our screen in landscape:
//0,0		255,0
//0,127		255,127



/*
###############################################
##### TO USE THIS DRIVER IN A NEW PROJECT #####
###############################################
- Check this header file and the display.h header file for defines to setup and any usage notes
- Configure the control pins as outputs in your applications initialisation.
*/



//*****************************
//*****************************
//********** DEFINES **********
//*****************************
//*****************************
#ifndef DISPLAY_MODEL_C_INIT				//(Include this section only once for each source file that includes this header file)
#define	DISPLAY_MODEL_C_INIT

#define DISPLAY_DEFAULT_CONTRAST			160


//----------------------------------------------
//----- DEFINE TARGET COMPILER & PROCESSOR -----
//----------------------------------------------
//(ONLY 1 SHOULD BE INCLUDED, COMMENT OUT OTHERS)
//#define	DISPLAY_USING_MICROCHIP_C32_COMPILER
#define	DISPLAY_USING_MICROCHIP_XC32_COMPILER
//<< add other compiler types here


//-------------------------
//----- DISPLAY SETUP -----
//-------------------------
#define	DISPLAY_WIDTH_PIXELS			256			//Width is the X coordinate
#define	DISPLAY_HEIGHT_PIXELS			128			//Height is the Y coordinate


//------------------------
//----- USER OPTIONS -----
//------------------------



#ifdef DISPLAY_USING_MICROCHIP_C32_COMPILER
//########################################
//########################################
//##### USING MICROCHIP C32 COMPILER #####
//########################################
//########################################

//----------------------
//----- IO DEFINES -----
//----------------------

#define	DISPLAY_BUS_ACCESS_DELAY	Nop(); Nop(); Nop(); Nop(); Nop()		//('Nop();' is a single cycle null instruction for the C32 compiler, include multiple times if required)

//CONTROL PINS:-
#define DISPLAY_CS(state)					(state ? mPORTBSetBits(BIT_14) : mPORTBClearBits(BIT_14))		//0 = Select Controller
#define DISPLAY_CONTROL_DATA(state)			(state ? mPORTBSetBits(BIT_15) : mPORTBClearBits(BIT_15))		//0 = Control, 1 = Data
#define	DISPLAY_RESET(state)				(state ? mPORTDSetBits(BIT_7) : mPORTDClearBits(BIT_7))			//0 = Reset Controller
#define DISPLAY_RD(state)					(state ? mPORTDSetBits(BIT_5) : mPORTDClearBits(BIT_5))
#define DISPLAY_WR(state)					(state ? mPORTDSetBits(BIT_4) : mPORTDClearBits(BIT_4))

//PORTS:-
#define	DISPLAY_DATA_TO_OUTPUTS()			mPORTESetPinsDigitalOut(0x00ff)
#define	DISPLAY_DATA_TO_INPUTS()			mPORTESetPinsDigitalIn(0x00ff)
#define	DISPLAY_WRITE_DATA(data)			mPORTEWrite(data); DISPLAY_WR(0); DISPLAY_BUS_ACCESS_DELAY; DISPLAY_WR(1)	//Write 8bit word to controller
#define	DISPLAY_READ_DATA_DO_DUMMY(data)	DISPLAY_RD(0); DISPLAY_BUS_ACCESS_DELAY; DISPLAY_RD(1)	//Write 8bit word to controller
#define	DISPLAY_READ_DATA(data)				DISPLAY_RD(0); DISPLAY_BUS_ACCESS_DELAY; data = (uint8_t)(mPORTERead() & 0x000000ff); DISPLAY_RD(1)	//Write 8bit word to controller


//###############################################
//###############################################
//##### END OF USING MICROCHIP C32 COMPILER #####
//###############################################
//###############################################
#endif		//#ifdef DISPLAY_USING_MICROCHIP_C32_COMPILER




#ifdef DISPLAY_USING_MICROCHIP_XC32_COMPILER
//#########################################
//#########################################
//##### USING MICROCHIP XC32 COMPILER #####
//#########################################
//#########################################

//----------------------
//----- IO DEFINES -----
//----------------------

#define	DISPLAY_BUS_ACCESS_DELAY	Nop(); Nop(); Nop(); Nop(); Nop()		//('Nop();' is a single cycle null instruction for the C32 compiler, include multiple times if required)

//CONTROL PINS:-
#define DISPLAY_CS(state)					LATJbits.LATJ8 = state				//CS0. 0 = select controller
#define DISPLAY_CONTROL_DATA(state)			LATJbits.LATJ9 = state				//CD. 0 = Command, 1=Data
#define	DISPLAY_RESET(state)				LATCbits.LATC1 = state				//0 = reset controller
#define DISPLAY_RD(state)					LATDbits.LATD9 = state				//WR1. 0 = read (in 8080 mode)
#define DISPLAY_WR(state)					LATAbits.LATA4 = state				//WR0. 0 = WRITE (in 8080 mode)

//PORTS:-
#define	DISPLAY_DATA_TO_OUTPUTS()			TRISECLR = (uint32_t)0x00ff
#define	DISPLAY_DATA_TO_INPUTS()			TRISESET = (uint32_t)0x00ff; 
#define	DISPLAY_WRITE_DATA(data)			LATESET = (uint32_t)data; LATECLR = (uint32_t)~data; DISPLAY_WR(0); DISPLAY_BUS_ACCESS_DELAY; DISPLAY_WR(1)	//Write 8bit word to controller
#define	DISPLAY_READ_DATA_DO_DUMMY(data)	DISPLAY_RD(0); DISPLAY_BUS_ACCESS_DELAY; DISPLAY_RD(1)	//Write 8bit word to controller
#define	DISPLAY_READ_DATA(data)				DISPLAY_RD(0); DISPLAY_BUS_ACCESS_DELAY; data = (uint8_t)(PORTE & 0x000000ff); DISPLAY_RD(1)	//Read 8bit word from controller


//-----------------------------------
//----- BUS ACCESS DELAY DEFINE -----
//-----------------------------------
//Depending on the speed of your processor and bus you may need to use a delay when reading and writing to and from the controller.
//Use this define to do this.
//Need min 75nS
#define	DISPLAY_BUS_ACCESS_DELAY	asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop")		//(nop is a single cycle null instruction, include multiple times if required)



//################################################
//################################################
//##### END OF USING MICROCHIP XC32 COMPILER #####
//################################################
//################################################
#endif		//#ifdef DISPLAY_USING_MICROCHIP_XC32_COMPILER






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
void display_write_command (uint8_t data);
uint8_t display_get_pixel_value (uint16_t x_coord, uint16_t y_coord);


//-----------------------------------------
//----- INTERNAL & EXTERNAL FUNCTIONS -----
//-----------------------------------------
//(Also defined below as extern)
void display_model_initialise(void);
void display_model_set_contrast(uint8_t contrast_value);
void display_write_pixel (uint16_t x_coord, uint16_t y_coord, uint32_t colour);
void display_write_block (uint16_t x_start_coord, uint16_t y_start_coord, uint16_t x_end_coord, uint16_t y_end_coord, uint32_t colour);
void display_output_buffer (void);


#else
//------------------------------
//----- EXTERNAL FUNCTIONS -----
//------------------------------
extern void display_model_initialise(void);
void display_model_set_contrast(uint8_t contrast_value);
extern void display_write_pixel (uint16_t x_coord, uint16_t y_coord, uint32_t colour);
extern void display_write_block (uint16_t x_start_coord, uint16_t y_start_coord, uint16_t x_end_coord, uint16_t y_end_coord, uint32_t colour);
void display_output_buffer (void);


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
uint8_t display_copy_buffer[((DISPLAY_WIDTH_PIXELS * DISPLAY_HEIGHT_PIXELS) >> 3)];		//256x128 = 4096 bytes

//--------------------------------------------------
//----- INTERNAL & EXTERNAL MEMORY DEFINITIONS -----
//--------------------------------------------------
//(Also defined below as extern)


#else
//---------------------------------------
//----- EXTERNAL MEMORY DEFINITIONS -----
//---------------------------------------


#endif


