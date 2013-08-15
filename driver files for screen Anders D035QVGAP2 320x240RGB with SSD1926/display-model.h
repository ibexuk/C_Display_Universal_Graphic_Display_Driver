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
//---------- SCREEN:		Anders D035QVGAP2									----------
//---------- Resolution:	320 x 240 pixel										----------
//---------- Driver IC:		Solomon Systech SSD1926 (external)					----------
//---------- Colour:		16bit (5Red|6Green|5Blue)							----------
//---------- Interface:		16bit Parallel										----------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------


//SSD1926 features
//	256kB display RAM
//	Supports many display types and 1, 2, 4, 8, 16 or 32 bit per pixel (bpp) color depth
//	Popular driver sold in high volumes and therefore low cost (even not using much of it's powerful feature set it's still often lower cost than simpler display controllers).
//	This driver does not make use of the SSD1926 JPEG decoder.

//SSD1926 configuration for this driver:
//	AD_Mode=1:CNF2=1:CNF1=0:CNF0=0 - Host Bus Interface is Indirect 16 bit 8080
//	CNF4=0 - Little endian bus interface, CNF4=1 - Big endian bus interface (define below allows either setting)
//	CNF6=0 - MCLK=PLL_CLK (recommended for Indirect addressing mode)

//SSD1926 interface to MCU:
//	D15:0	Data bus
//	AB3		Command / Data select
//	CS		Chip select
//	RD		Read
//	WR		Write
//	Reset	Low to reset
//	WAIT#	Optional wait signal. 7 MCLK is needed for each cycle if WAIT# is not used for interface in 8080 Indirect addressing Mode.
//			CNF6=0 sets MCLK=PLL_CLK.  PLL_CLK is set to 80MHz in our initialisation of the SSD1926. So if WAIT# is not used each access cycle must take >=87.5nS

//TFT Interface Signals:
//	Shift		Clock (for sampling each data signal).  6.5MHz typ for 24bit mode. Duty ratio 50% typ
//	Hsync		Line synchronization signal (Hoz synchronous sig)
//	Vsync		Frame synchronization signal (Vert synchronous sig)
//	R/L & TB	Sets orientation.


//Address is 2 addresses per pixel starting from zero

//Coordinates are always 0,0 in top left corner.  Our screen in landscape:
//0,0		319,0
//0,239		319,239



/*
###############################################
##### TO USE THIS DRIVER IN A NEW PROJECT #####
###############################################
- Check this header file and the display.h header file for defines to setup and any usage notes
- Configure the control pins as outputs in your applications initialisation.
*/

//##############################################
//##### SSD1926 MEMORY ADDRESS SPACE SETUP #####
//##############################################
//SSD1926 memory 256kB.  0x00000 - 0x3FFFF
//
//MAIN WINDOW DISPLAY START ADDRESS (0x0076|0x0075|0x0074) = 0
//
//MAIN WINDOW LINE ADDRESS OFFSET (0x0079|0x0078) = ((320 / (32 / 16)) = 160
//	Specifies the offset in double words from the beginning of one display line to the beginning of the next display line in the main window (32-bit address increment).
//
//So 160 x 4 = 640 Bytes per line.  640 x 240 lines = 153600 Bytes used for entire display area.  So address range used 0x00000 - 0x257FF


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
#define	DISPLAY_USING_MICROCHIP_C32_COMPILER
//<< add other compiler types here


//-------------------------
//----- DISPLAY SETUP -----
//-------------------------
#define	DISPLAY_WIDTH_PIXELS			320			//Width is the X coordinate
#define	DISPLAY_HEIGHT_PIXELS			240			//Height is the Y coordinate


//------------------------
//----- USER OPTIONS -----
//------------------------
//#define	DISPLAY_BUS_INTERFACE_IS_BIG_ENDIAN		//Comment out if little endian selected using SSD1926 pin CNF4 (CNF4=H=BigEndian)


#ifdef DISPLAY_USING_MICROCHIP_C32_COMPILER
//########################################
//########################################
//##### USING MICROCHIP C32 COMPILER #####
//########################################
//########################################


//----------------------
//----- IO DEFINES -----
//----------------------

//CONTROL PINS:-
#define DISPLAY_CS(state)					(state ? mPORTCSetBits(BIT_2) : mPORTCClearBits(BIT_2))			//0 = Select Controller
#define DISPLAY_REGISTER_SELECT(state)		(state ? mPORTCSetBits(BIT_1) : mPORTCClearBits(BIT_1))			//0 = Command, 1 = Data
#define	DISPLAY_RESET(state)				(state ? mPORTCSetBits(BIT_3) : mPORTCClearBits(BIT_3))			//0 = Reset Controller

//PORTS:-
//These defines make us of the PIC32MX695F512L parallel port peripheral:
#define	DISPLAY_WRITE_DATA(data)			PMDIN = data; while(PMMODEbits.BUSY)							//Write 16bit word to controller
#define	DISPLAY_READ_DATA_DO_DUMMY(data)	data = PMDIN; while(PMMODEbits.BUSY)							//Read dummy 16bit word from controller if a read needs to be carried out first before valid data is ready (comment out if not required)
#define	DISPLAY_READ_DATA(data)				data = PMDIN; while(PMMODEbits.BUSY)							//Read 16bit word from controller (comment out if not required)
//There is no reason why the parallel port needs to be used and these defines can be altered as requried to read and write the 16 bits of data





//###############################################
//###############################################
//##### END OF USING MICROCHIP C32 COMPILER #####
//###############################################
//###############################################
#endif		//#ifdef DISPLAY_USING_MICROCHIP_C32_COMPILER





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
void display_delay_ms (WORD delay_ms);
void display_set_register (WORD index, BYTE value);


//-----------------------------------------
//----- INTERNAL & EXTERNAL FUNCTIONS -----
//-----------------------------------------
//(Also defined below as extern)
void display_model_initialise(void);
void display_write_pixel (WORD x_coord, WORD y_coord, DWORD colour);
void display_write_block (WORD x_start_coord, WORD y_start_coord, WORD x_end_coord, WORD y_end_coord, DWORD colour);


#else
//------------------------------
//----- EXTERNAL FUNCTIONS -----
//------------------------------
extern void display_model_initialise(void);
extern void display_write_pixel (WORD x_coord, WORD y_coord, DWORD colour);
extern void display_write_block (WORD x_start_coord, WORD y_start_coord, WORD x_end_coord, WORD y_end_coord, DWORD colour);


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


