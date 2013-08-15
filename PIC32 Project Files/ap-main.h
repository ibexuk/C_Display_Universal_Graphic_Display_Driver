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
//PIC32 SAMPLE PROJECT C CODE HEADER FILE


//*****************************
//*****************************
//********** DEFINES **********
//*****************************
//*****************************
#ifndef MAIN_C_INIT				//(Include this section only once for each source file that includes this header file)
#define	MAIN_C_INIT


//----------------------
//----- IO DEFINES -----
//----------------------
#define LED1(state)					(state ? mPORTDSetBits(BIT_1) : mPORTDClearBits(BIT_1))
#define LED2(state)					(state ? mPORTDSetBits(BIT_2) : mPORTDClearBits(BIT_2))


//--------------------------
//----- SWITCH DEFINES -----
//--------------------------
//Joystick:
//72 RB0/CN2 	Left
//70 RB1/CN3 	Up
//66 RB3/CN5 	Down
//64 RB4/CN6 	Right
//36 RB15/CN12	Fire

#define	SWITCH_UP_PRESSED			((switches_1_debounced & 0x0002) ? 1 : 0)
#define	SWITCH_DOWN_PRESSED			((switches_1_debounced & 0x0008) ? 1 : 0)
#define	SWITCH_LEFT_PRESSED			((switches_1_debounced & 0x0001) ? 1 : 0)
#define	SWITCH_RIGHT_PRESSED		((switches_1_debounced & 0x0010) ? 1 : 0)
#define	SWITCH_FIRE_PRESSED			((switches_1_debounced & 0x8000) ? 1 : 0)

#define	SWITCH_UP_NEW_PRESS			((switches_1_new & 0x0002) ? 1 : 0)
#define	SWITCH_DOWN_NEW_PRESS		((switches_1_new & 0x0008) ? 1 : 0)
#define	SWITCH_LEFT_NEW_PRESS		((switches_1_new & 0x0001) ? 1 : 0)
#define	SWITCH_RIGHT_NEW_PRESS		((switches_1_new & 0x0010) ? 1 : 0)
#define	SWITCH_FIRE_NEW_PRESS		((switches_1_new & 0x8000) ? 1 : 0)







#endif





//*******************************
//*******************************
//********** FUNCTIONS **********
//*******************************
//*******************************
#ifdef MAIN_C
//-----------------------------------
//----- INTERNAL ONLY FUNCTIONS -----
//-----------------------------------
void initialise (void);
void read_switches (void);


//-----------------------------------------
//----- INTERNAL & EXTERNAL FUNCTIONS -----
//-----------------------------------------
//(Also defined below as extern)


#else
//------------------------------
//----- EXTERNAL FUNCTIONS -----
//------------------------------


#endif




//****************************
//****************************
//********** MEMORY **********
//****************************
//****************************
#ifdef MAIN_C
//--------------------------------------------
//----- INTERNAL ONLY MEMORY DEFINITIONS -----
//--------------------------------------------


//--------------------------------------------------
//----- INTERNAL & EXTERNAL MEMORY DEFINITIONS -----
//--------------------------------------------------
//(Also defined below as extern)
BYTE do_10ms_functions;
WORD switches_1_debounced = 0;
WORD switches_1_new = 0;


#else
//---------------------------------------
//----- EXTERNAL MEMORY DEFINITIONS -----
//---------------------------------------
extern BYTE do_10ms_functions;
extern WORD switches_1_debounced;
extern WORD switches_1_new;


#endif








