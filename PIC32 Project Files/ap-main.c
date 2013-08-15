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
//PIC32 SAMPLE PROJECT C CODE FILE



//----- INCLUDE FILES FOR THIS SOURCE CODE FILE -----
#include "main.h"					//Global data type definitions (see https://github.com/ibexuk/C_Generic_Header_File )
#define	MAIN_C						//(Define used for following header file to flag that it is the header file for this source file)
#include "ap-main.h"				//(Include header file for this source file)
#undef MAIN_C

//----- OTHER PROJECT FILES REQUIRED BY THIS SOURCE CODE FILE -----
#include "display.h"
#include "display-html.h"

//----- COMPILER LIBRARY FILES REQUIRED BY THIS SOURCE CODE FILE -----



#include <plib.h>


//******************************************
//******************************************
//********** DEVICE CONFIGURATION **********
//******************************************
//******************************************
//These configuration defines do not need to be included, but having them means that the configuration bits will be automatically set and will
//be included in the .hex file created for the project, so that they do not need to be manually set when programming devices at manufacture.
//(The config names are given for each device in MPLAB help, under 'Topics... -> PIC32MX Configuration Settings')


#ifdef __DEBUG				//Debug mode selected in MPLAB
//----- WE'RE IN DEVELOPMENT MODE -----
	#pragma config FVBUSONIO = ON			//VBUS_ON pin (OFF = controlled by the Port Function)
	#pragma config FUSBIDIO = ON			//USBID pin (OFF = controlled by the Port Function)
	#pragma config FCANIO = ON				//Default CAN IO Pins
	#pragma config FETHIO = ON				//Default Ethernet IO Pins
	#pragma config FMIIEN = ON				//MII enabled
	#pragma config FSRSSEL = PRIORITY_7		//SRS Interrupt Priority Level
	#pragma config FPLLODIV = DIV_1			//PLL Output Divider
	#pragma config UPLLEN = OFF				//USB PLL Enable
	#pragma config UPLLIDIV = DIV_12		//USB PLL Input Divider
	#pragma config FPLLMUL = MUL_20			//PLL Multiplier
	#pragma config FPLLIDIV = DIV_2			//PLL Input Divider (Must produce 4-5MHz from crystal frequency)
	#pragma config FWDTEN = OFF				//Watchdog Timer
	#pragma config WDTPS = PS1048576		//Watchdog Timer Postscale (no postscale = 1mS timeout for PIC32)
	#pragma config FCKSM = CSECME			//Clock Switching & Fail Safe Clock Monitor
	#pragma config FPBDIV = DIV_1			//Bootup Peripheral Clock divisor
	#pragma config OSCIOFNC = ON			//CLKO Enable
	#pragma config POSCMOD = XT				//Primary Oscillator
	#pragma config IESO = ON				//Internal/External Switch-over
	#pragma config FSOSCEN = ON				//Secondary Oscillator Enable
	#pragma config FNOSC = PRIPLL			//Oscillator Selection
	#pragma config CP = OFF					//Code Protect
	#pragma config BWP = OFF				//Boot Flash Write Protect
	#pragma config PWP = OFF				//Program Flash Write Protect
	#pragma config ICESEL = ICS_PGx2		//ICE/ICD Comm Channel Select
	#pragma config DEBUG = ON				//Background Debugger Enable
#else
//----- WE'RE NOT IN DEVELOPMENT MODE -----
	#pragma config FVBUSONIO = ON			//VBUS_ON pin (OFF = controlled by the Port Function)
	#pragma config FUSBIDIO = ON			//USBID pin (OFF = controlled by the Port Function)
	#pragma config FCANIO = ON				//Default CAN IO Pins
	#pragma config FETHIO = ON				//Default Ethernet IO Pins
	#pragma config FMIIEN = ON				//MII enabled
	#pragma config FSRSSEL = PRIORITY_7		//SRS Interrupt Priority Level
	#pragma config FPLLODIV = DIV_1			//PLL Output Divider
	#pragma config UPLLEN = OFF				//USB PLL Enable
	#pragma config UPLLIDIV = DIV_12		//USB PLL Input Divider (Must produce 4-5MHz from crystal frequency)
	#pragma config FPLLMUL = MUL_20			//PLL Multiplier
	#pragma config FPLLIDIV = DIV_2			//PLL Input Divider
	#pragma config FWDTEN = ON				//Watchdog Timer
	#pragma config WDTPS = PS1048576		//Watchdog Timer Postscale (no postscale = 1mS timeout for PIC32)
	#pragma config FCKSM = CSECME			//Clock Switching & Fail Safe Clock Monitor
	#pragma config FPBDIV = DIV_1			//Bootup Peripheral Clock divisor
	#pragma config OSCIOFNC = ON			//CLKO Enable
	#pragma config POSCMOD = XT				//Primary Oscillator
	#pragma config IESO = ON				//Internal/External Switch-over
	#pragma config FSOSCEN = ON				//Secondary Oscillator Enable
	#pragma config FNOSC = PRIPLL			//Oscillator Selection
	#pragma config CP = OFF					//Code Protect
	#pragma config BWP = OFF				//Boot Flash Write Protect
	#pragma config PWP = OFF				//Program Flash Write Protect
	#pragma config ICESEL = ICS_PGx2		//ICE/ICD Comm Channel Select
	#pragma config DEBUG = ON				//Background Debugger Enable
#endif









//*************************************************************************************************************************************
//*************************************************************************************************************************************
//*************************************************************************************************************************************
//*************************************************************************************************************************************
//*************************************************************************************************************************************
//*************************************************************************************************************************************
//*************************************************************************************************************************************

//********************************
//********************************
//********** INITIALISE **********
//********************************
//********************************
void initialise (void)
{
	BYTE data;
	
	//##### GENERAL NOTE ABOUT PIC32'S #####
	//Try and use the peripheral libraries instead of special function registers for everything (literally everything!) to avoid
	//bugs that can be caused by the pipeline and interrupts.
	

	//---------------------------------
	//----- CONFIGURE PERFORMANCE -----
	//---------------------------------
	
	//----- SETUP EVERYTHING FOR OPTIMUM PERFORMANCE -----
	SYSTEMConfigPerformance(80000000ul);		//Note this sets peripheral bus to '1' max speed (regardless of configuration bit setting)
												//Use PBCLK divider of 1:1 to calculate UART baud, timer tick etc


	//----- SET PERIPHERAL BUS DIVISOR -----
	//To minimize dynamic power the PB divisor should be chosen to run the peripherals at the lowest frequency that provides acceptable system performance
	mOSCSetPBDIV(OSC_PB_DIV_2);			//OSC_PB_DIV_1, OSC_PB_DIV_2, OSC_PB_DIV_4, OSC_PB_DIV_8, 

	//----- SETUP INTERRUPTS -----
	INTEnableSystemMultiVectoredInt();

	//-------------------------
	//----- SETUP IO PINS -----
	//-------------------------
	//(Device will powerup with all IO pins as inputs)

	//----- TURN OFF THE JTAG PORT -----
	//(JTAG is on by default)
	//mJTAGPortEnable(0);		//Must be on for Microchip Multimedia Development board

	#define	PORTA_IO	0xc2ff				//Setup the IO pin type (0 = output, 1 = input)
	mPORTAWrite(0xc033);					//Set initial ouput pin states
	mPORTASetPinsDigitalIn(PORTA_IO);		//(Sets high bits as input)
	mPORTASetPinsDigitalOut(~PORTA_IO);		//(Sets high bits as output)
	
	#define	PORTB_IO	0xfbff				//Setup the IO pin type (0 = output, 1 = input)
	mPORTBWrite(0x6d13);					//Set initial ouput pin states
	mPORTBSetPinsDigitalIn(PORTB_IO);		//(Sets high bits as input)
	mPORTBSetPinsDigitalOut(~PORTB_IO);		//(Sets high bits as output)

	mPORTBSetPinsDigitalIn(BIT_0 | BIT_1 | BIT_3 | BIT_4 | BIT_15);			//Joystick inputs

	#define	PORTC_IO	0xf01e				//Setup the IO pin type (0 = output, 1 = input)
	mPORTCWrite(0x3018);					//Set initial ouput pin states
	mPORTCSetPinsDigitalIn(PORTC_IO);		//(Sets high bits as input)
	mPORTCSetPinsDigitalOut(~PORTC_IO);		//(Sets high bits as output)

	#define	PORTD_IO	0x7bfe				//Setup the IO pin type (0 = output, 1 = input)
	mPORTDWrite(0xbdaf);					//Set initial ouput pin states
	mPORTDSetPinsDigitalIn(PORTD_IO);		//(Sets high bits as input)
	mPORTDSetPinsDigitalOut(~PORTD_IO);		//(Sets high bits as output)

	mPORTDSetPinsDigitalOut(BIT_2 | BIT_1);		//LED's 2 and 3
	mPORTDSetPinsDigitalIn(BIT_9);

	#define	PORTE_IO	0x03ff				//Setup the IO pin type (0 = output, 1 = input)
	mPORTEWrite(0x02a2);					//Set initial ouput pin states
	mPORTESetPinsDigitalIn(PORTE_IO);		//(Sets high bits as input)
	mPORTESetPinsDigitalOut(~PORTE_IO);		//(Sets high bits as output)

	#define	PORTF_IO	0x111f				//Setup the IO pin type (0 = output, 1 = input)
	mPORTFWrite(0x0039);					//Set initial ouput pin states
	mPORTFSetPinsDigitalIn(PORTF_IO);		//(Sets high bits as input)
	mPORTFSetPinsDigitalOut(~PORTF_IO);		//(Sets high bits as output)

	#define	PORTG_IO	0xd3cf				//Setup the IO pin type (0 = output, 1 = input)
	mPORTGWrite(0xf203);					//Set initial ouput pin states
	mPORTGSetPinsDigitalIn(PORTG_IO);		//(Sets high bits as input)
	mPORTGSetPinsDigitalOut(~PORTG_IO);		//(Sets high bits as output)



	//Read pins using:
	// mPORTAReadBits(BIT_0);
	//Write pins using:
	// mPORTAClearBits(BIT_0);
	// mPORTASetBits(BIT_0);
	// mPORTAToggleBits(BIT_0);



	//----- INPUT CHANGE NOTIFICATION CONFIGURATION -----
	//EnableCN0();
	ConfigCNPullups(CN2_PULLUP_ENABLE | CN3_PULLUP_ENABLE | CN5_PULLUP_ENABLE | CN6_PULLUP_ENABLE | CN12_PULLUP_ENABLE);		//Joystick pins



	//----- SETUP THE A TO D PINS -----
	ENABLE_ALL_DIG;


	//---------------------
	//----- SETUP USB -----
	//---------------------
	//The USB specifications require that USB peripheral devices must never source current onto the Vbus pin.  Additionally, USB peripherals should not source
	//current on D+ or D- when the host/hub is not actively powering the Vbus line.  When designing a self powered (as opposed to bus powered) USB peripheral
	//device, the firmware should make sure not to turn on the USB module and D+ or D- pull up resistor unless Vbus is actively powered.  Therefore, the
	//firmware needs some means to detect when Vbus is being powered by the host. A 5V tolerant I/O pin can be connected to Vbus (through a resistor), and
	//can be used to detect when Vbus is high (host actively powering), or low (host is shut down or otherwise not supplying power).  The USB firmware
	//can then periodically poll this I/O pin to know when it is okay to turn on the USB module/D+/D- pull up resistor.  When designing a purely bus powered
	//peripheral device, it is not possible to source current on D+ or D- when the host is not actively providing power on Vbus. Therefore, implementing this
	//bus sense feature is optional.  This firmware can be made to use this bus sense feature by making sure "USE_USB_BUS_SENSE_IO" has been defined in the
	//HardwareProfile.h file.    
    //	#if defined(USE_USB_BUS_SENSE_IO)
    //	tris_usb_bus_sense = INPUT_PIN; // See HardwareProfile.h
    //	#endif
    
	//If the host PC sends a GetStatus (device) request, the firmware must respond	and let the host know if the USB peripheral device is currently bus powered
	//or self powered.  See chapter 9 in the official USB specifications for details regarding this request.  If the peripheral device is capable of being both
	//self and bus powered, it should not return a hard coded value for this request.  Instead, firmware should check if it is currently self or bus powered, and
	//respond accordingly.  If the hardware has been configured like demonstrated on the PICDEM FS USB Demo Board, an I/O pin can be polled to determine the
	//currently selected power source.  On the PICDEM FS USB Demo Board, "RA2" is used for	this purpose.  If using this feature, make sure "USE_SELF_POWER_SENSE_IO"
	//has been defined in HardwareProfile.h, and that an appropriate I/O pin has been mapped to it in HardwareProfile.h.
    //	#if defined(USE_SELF_POWER_SENSE_IO)
    //	tris_self_power = INPUT_PIN;	// See HardwareProfile.h
    //	#endif
    
    
    //Enable the USB port now - we will check to see if Vbus is  powered at the end of init and disable it if not.
    //USBDeviceInit();	//usb_device.c.  Initializes USB module SFRs and firmware variables to known states.

	

	//------------------------
	//----- SETUP TIMERS -----
	//------------------------
	//(INCLUDE THE USAGE OF ALL TIMERS HERE EVEN IF NOT SETUP HERE SO THIS IS THE ONE POINT OF
	//REFERENCE TO KNOW WHICH TIMERS ARE IN USE AND FOR WHAT).

	//----- SETUP TIMER 1 -----
	//Used for: Available
	//OpenTimer1((T1_ON | T1_IDLE_CON | T1_GATE_OFF | T1_PS_1_4 | T1_SOURCE_INT), 20000);

	//----- SETUP TIMER 2 -----
	//Used for: 
	//OpenTimer2((T2_ON | T2_IDLE_CON | T2_GATE_OFF | T2_PS_1_1 | T2_SOURCE_INT), 0xffff);		//0xffff = 305Hz

	//----- SETUP TIMER 3 -----
	//Used for: 
	//OpenTimer3((T3_ON | T3_IDLE_CON | T3_GATE_OFF | T3_PS_1_1 | T3_SOURCE_INT), PIEZO_TIMER_PERIOD);

	//----- SETUP TIMER 4 -----
	//Used for: 
	//OpenTimer4((T4_ON | T4_IDLE_CON | T4_GATE_OFF | T4_PS_1_1 | T4_SOURCE_INT), 20000);

	//----- SETUP TIMER 5 -----
	//Used for: Heartbeat
	OpenTimer5((T5_ON | T5_IDLE_CON | T5_GATE_OFF | T5_PS_1_1 | T5_SOURCE_INT), 40000);		//1mS with 80MHz osc and PB_DIV_2
	ConfigIntTimer5(T5_INT_ON | T5_INT_PRIOR_7);											//1=lowest priority to 7=highest priority.  ISR function must specify same value



	//---------------------------------
	//----- SETUP EVAL BOARD CPLD -----
	//---------------------------------
	//Graphics bus width = 16
	mPORTGSetPinsDigitalOut(BIT_14);
	mPORTGSetBits(BIT_14);

	//SPI source select = SPI3 (not used)
	mPORTGSetPinsDigitalOut(BIT_12);
	mPORTGClearBits(BIT_12);
	
	//SPI peripheral destination select = Expansion Slot (not used)
	mPORTASetPinsDigitalOut(BIT_7 | BIT_6);
	mPORTASetBits(BIT_7);
	mPORTAClearBits(BIT_6);


	//--------------------------------------
    //----- PARALLEL MASTER PORT SETUP -----
	//--------------------------------------
    PMMODE = 0;
    PMAEN = 0;
    PMCON = 0;
    PMMODE = 0x0610;
    PMCONbits.PTRDEN = 1;                   //Enable RD line
    PMCONbits.PTWREN = 1;                   //Enable WR line
    PMCONbits.PMPEN = 1;                    //Enable PMP


	//------------------------------
	//----- INITIALISE DISPLAY -----
	//------------------------------
	display_initialise();


	display_test();

	//LOAD OUR GLOBAL HTML STYLES FILE READY FOR DISPLAY HTML PAGES
	BYTE dummy_styles_count;
	DWORD file_size;
	if (display_html_setup_read_file(global_css, 0, &file_size))
	{
		dummy_styles_count = 0;
		display_html_read_styles(&file_size, &dummy_styles_count, 1);			//1 = this is global styles file
	}

}






//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************

CONSTANT BYTE string1[] = {"Hello World!"};

//***********************************
//***********************************
//********** MAIN FUNCTION **********
//***********************************
//***********************************
int main (void)
{
	BYTE display_html_file_name[12];
	DWORD file_size;

	//**********************
	//**********************
	//***** INITIALISE *****
	//**********************
	//**********************
	initialise();





	//*********************
	//*********************
	//***** MAIN LOOP *****
	//*********************
	//*********************
	while(1)						//(Do forever)
	{
		//----- RESET THE WATCHDOG TIMEOUT TIMER -----
		ClearWDT();


		switches_1_new = 0;
		if (do_10ms_functions)
		{
			do_10ms_functions = 0;
		
			//----- READ SWITCHES -----
			read_switches();

		}


		//------------------------------------
		//----- CHECK FOR SWITCH PRESSES -----
		//------------------------------------
		if (SWITCH_UP_NEW_PRESS)
		{
			//----- UP PRESSED -----
			
			//Load the index.htm file
			if (display_html_setup_read_file(index_htm, 0, &file_size))			//Find HTML file ready to display it
				display_html_file(file_size);
		}
		else if (SWITCH_DOWN_NEW_PRESS)
		{
			//----- DOWN PRESSED -----
			
			//Load text1.htm using name as a variable to demonstrate finding file by name at run time
			display_html_file_name[0] = 'T';
			display_html_file_name[1] = 'E';
			display_html_file_name[2] = 'X';
			display_html_file_name[3] = 'T';
			display_html_file_name[4] = '1';
			display_html_file_name[5] = '.';
			display_html_file_name[6] = 'H';
			display_html_file_name[7] = 'T';
			display_html_file_name[8] = 'M';
			display_html_file_name[9] = 0x00;
			
			if (display_html_setup_read_file(0, display_html_file_name, &file_size))
			{
				display_html_file(file_size);
			}

		}
		else if (SWITCH_LEFT_NEW_PRESS)
		{
			//----- LEFT PRESSED -----
			
			//Load dynamic text demo HTML file - the text is added by the html_get_dynamic_text_character function below
			if (display_html_setup_read_file(dyntext_htm, 0, &file_size))
				display_html_file(file_size);
		}
		else if (SWITCH_RIGHT_NEW_PRESS)
		{
			//----- RIGHT PRESSED -----
			
			//Load text2.htm
			if (display_html_setup_read_file(text2_htm, 0, &file_size))
				display_html_file(file_size);
		}
		else if (SWITCH_FIRE_NEW_PRESS)
		{
			//----- FIRE PRESSED -----


			//Generate content using code instead of HTML
			display_clear_screen(DISPLAY_COLOUR_WHITE);
			
			display_bitmap(indeximg_bmp, 0, 0, 0x00ffffff);		//p_bitmap, x_coord, y_coord, transparency_colour(0xffffffff to not use)
		
		

			display_foreground_colour = DISPLAY_COLOUR_RED;
			display_background_colour = DISPLAY_COLOUR_TURQUOISE;
			display_const_string (disp_font_5w_7h, DISPLAY_TEXT_ALIGN_LEFT,				//Font, Options
									2, 2,												//Hoz padding, vertical padding
									20, 50,												//X start coord, Y start coord
									0, 0,												//X end coord, Y end coord (0 if area containment not required)
									string1);											//Text
	
	
			display_const_string(disp_font_5w_11h, DISPLAY_TEXT_ALIGN_LEFT,				//Font, Options
									2, 2,												//Hoz padding, vertical padding
									20, 70,												//X start coord, Y start coord
									0, 0,												//X end coord, Y end coord (0 if area containment not required)
									string1);											//Text


			display_foreground_colour = DISPLAY_COLOUR_VIOLET;
			display_background_colour = DISPLAY_COLOUR_YELLOW;
			display_const_string(disp_font_22h, DISPLAY_TEXT_ALIGN_CENTRE,				//Font, Options
									2, 2,												//Hoz padding, vertical padding
									20, 105,											//X start coord, Y start coord
									200, 160,											//X end coord, Y end coord (0 if area containment not required)
									string1);											//Text



			display_foreground_colour = DISPLAY_COLOUR_BLACK;
			display_background_colour = DISPLAY_COLOUR_NULL;							//(DISPLAY_COLOUR_NULL == no background colour / leave existing colour)
	
			display_const_string(disp_font_15h, DISPLAY_TEXT_ALIGN_LEFT,				//Font, Options
									2, 2,												//Hoz padding, vertical padding
									176, 134,											//X start coord, Y start coord
									0, 0,												//X end coord, Y end coord (0 if area containment not required)
									string1);											//Text


			display_foreground_colour = DISPLAY_COLOUR_WHITE;
			display_background_colour = DISPLAY_COLOUR_BLUE;
			display_const_string(disp_font_42h, DISPLAY_TEXT_ALIGN_CENTRE,				//Font, Options
									2, 2,												//Hoz padding, vertical padding
									0, 170,												//X start coord, Y start coord
									319, 0,												//X end coord, Y end coord (0 if area containment not required)
									string1);											//Text
									

		}

		
	}
}



//******************************************
//******************************************
//********** ANCHOR LINK FUNCTION **********
//******************************************
//******************************************
void html_store_anchor_link_function(BYTE *href_value, WORD start_x_coord, WORD start_y_coord, WORD end_x_coord, WORD end_y_coord)
{
	Nop();
}

//***********************************************
//***********************************************
//********** GET DYNAMIC TEXT FUNCTION **********
//***********************************************
//***********************************************
CONSTANT BYTE dynamic_text_0[] = {"Lorem"};
CONSTANT BYTE dynamic_text_1[] = {"ipsum"};
CONSTANT BYTE dynamic_text_2[] = {"dolor"};
CONSTANT BYTE dynamic_text_3[] = {"sit amet,"};
CONSTANT BYTE dynamic_text_4[] = {"consectetur"};

//This funtion needs return each character of the requried return text each time it is called, terminating in a null after the final character.
BYTE html_get_dynamic_text_character(BYTE dynamic_text_index, BYTE dynamic_text_character_index)
{
	static BYTE index_last = 0xff;
	WORD byte_index;
	
	if (index_last != dynamic_text_index)
	{
		//START OF A NEW DYNAMIC TEXT STRING
		index_last = dynamic_text_index;
		byte_index = 0;
	}
	
	switch (dynamic_text_index)
	{
	case 0:
		return(dynamic_text_0[byte_index++]);
	case 1:
		return(dynamic_text_1[byte_index++]);
	case 2:
		return(dynamic_text_2[byte_index++]);
	case 3:
		return(dynamic_text_3[byte_index++]);
	case 4:
		return(dynamic_text_4[byte_index++]);
	default:
		return(0);
	}

	
}



//***********************************
//***********************************
//********** READ SWITCHES **********
//***********************************
//***********************************
void read_switches (void)
{

	WORD switches_1_read;
	static WORD switches_1_last = 0;
	static WORD switches_1_debounced_last = 0;



	//RESET THE NEW SWITCH PRESS REGISTER
	switches_1_new = 0;

	//GET THE SWITCH INPUTS
	switches_1_read = (~mPORTBRead() & 0x801b);

	//DEBOUNCE
	switches_1_debounced = switches_1_last & switches_1_read;

	//FLAG NEW BUTTON PRESSES
	switches_1_new = switches_1_debounced ^ switches_1_debounced_last;
	switches_1_new &= switches_1_debounced;

	//STORE LAST REGISTERS FOR NEXT TIME
	switches_1_debounced_last = switches_1_debounced;
	switches_1_last = switches_1_read;
	
	
}






//*************************************************************************************************************************************
//*************************************************************************************************************************************
//*************************************************************************************************************************************
//*************************************************************************************************************************************
//*************************************************************************************************************************************
//*************************************************************************************************************************************
//*************************************************************************************************************************************

//*********************************************
//*********************************************
//********** HEARTBEAT IRQ (Timer 5) **********
//*********************************************
//*********************************************
void __ISR(_TIMER_5_VECTOR, ipl7) Timer5IntHandler (void) 	//(ipl# must match the priority level assigned to the irq where its enabled)
{
	static BYTE hb_10ms_timer = 0;
	static BYTE hb_100ms_timer = 0;
	static BYTE hb_1sec_timer = 0;

	
	
	INTClearFlag(INT_T5);						//Reset the timer irq flag

	//-----------------------------
	//-----------------------------
	//----- HERE EVERY 1 mSec -----
	//-----------------------------
	//-----------------------------

	hb_10ms_timer++;
	if (hb_10ms_timer == 10)
	{
		//------------------------------
		//------------------------------
		//----- HERE EVERY 10 mSec -----
		//------------------------------
		//------------------------------
		hb_10ms_timer = 0;

		do_10ms_functions = 1;
		
		
	} //if (hb_10ms_timer == 10)

	hb_100ms_timer++;
	if (hb_100ms_timer == 100)
	{
		//-------------------------------
		//-------------------------------
		//----- HERE EVERY 100 mSec -----
		//-------------------------------
		//-------------------------------
		hb_100ms_timer = 0;




		hb_1sec_timer++;
		if (hb_1sec_timer == 10)
		{
			//----------------------------
			//----------------------------
			//----- HERE EVERY 1 Sec -----
			//----------------------------
			//----------------------------
			hb_1sec_timer = 0;





		} //if (hb_1sec_timer == 10)
	} //if (hb_100ms_timer == 100)

}



//***************************************************
//***************************************************
//********** UNHANDLED EXCEPTION INTERRUPT **********
//***************************************************
//***************************************************
//Useful interrupt to include as it will be called if any exception occurs that is not handled
//There are CPU registers that will tell you where the exception occurred etc.
//View them in the watch window for a more useful description to be shown.
void __attribute__((nomips16)) _general_exception_handler(void)
{
	Nop();



}













