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




//****************************************
//****************************************
//********** DISPLAY INITIALISE **********
//****************************************
//****************************************
void display_model_initialise(void)
{
	WORD data;
	
	//##### DEBUG PIN TEST #####
	//Comment out for normal operation
	//Can be useful to quickly check each pin for shorts etc.  Each pin is pulsed one by one in a continuous loop so it can be probed with a scope.
	/*
	WORD w_temp;
	DISPLAY_RESET(0);
	DISPLAY_CS(1);
	while (1)
	{
		ENABLE_INT;
		ClearWDT();
		DISABLE_INT;

		DISPLAY_CS(0);
		DISPLAY_CS(1);

		DISPLAY_RESET(1);
		DISPLAY_RESET(0);

		DISPLAY_REGISTER_SELECT(0);
		DISPLAY_REGISTER_SELECT(1);
		
		for (w_temp = 0x0001; w_temp; w_temp <<= 1)
		{
			DISPLAY_WRITE_DATA(w_temp);
		}
		
		DISPLAY_WRITE_DATA(w_temp);
	}
	*/
	//#########################



	//----- PAUSE IF NECESSARY FOR DISPLAY TO POWERUP -----
	//display_delay_ms(50);
	
	//----- RESET DISPLAY -----
	//When reset state is released (RESET#=H) normal operation can be started after 3 MCLK period
	DISPLAY_RESET(0);
	DISPLAY_CS(1);
    display_delay_ms(10);
    DISPLAY_RESET(1);
    display_delay_ms(10);



	//##### DEBUG READ DEBUG REGISTERS #####
	//This is a useful way of verifying that comms to the SSD1926 are good.  It's not requried for initialisation, but it is a useful check
	/*
	//Read from address 0
	DISPLAY_REGISTER_SELECT(0);			//Select command
    DISPLAY_CS(0);
	#ifdef DISPLAY_BUS_INTERFACE_IS_BIG_ENDIAN
		//Write M/R#, AB18:8 (SSD1926 in big endian mode)
		//Bit15 represent the M/R#, Bit15 = 1 means memory access, Bit15 = 0 means register access.
		//Bit14:11 = 0.
		//Bit10:0 represent the the address AB18:8.
		DISPLAY_WRITE_DATA(0x0000);				//Register access
		
		//Write AB7:0 | Mode_SL (SSD1926 in big endian mode)
		//Bit15:8 represent the address AB7:0 and Bit7:0 represent Mode_SL.
		//Mode_SL to select byte or word access during 16 bit mode. 0x00 means Byte access, 0x01 means word access.
		DISPLAY_WRITE_DATA(0x0001);				//Word access
	#else
		//Write AB15:8, M/R#, AB18:16 (SSD1926 in little endian mode)
		//Bit7 represent the M/R# (1 means memory access, 0 means register access)
		//Bit6:3 = 0
		//Bit2:0 | 15:8 represent the the address AB18:8
		DISPLAY_WRITE_DATA(0x0000);				//Register access
		
		//Write Mode_SL | AB7:0 (SSD1926 in little endian mode)
		//Bit7:0 represent the address AB7:0 and Bit15:8 represent Mode_SL (0x00 means Byte access, 0x01 means word access)
		DISPLAY_WRITE_DATA(0x0100);				//Word access
	#endif
		
	DISPLAY_REGISTER_SELECT(1);					//Select data

	#ifdef DISPLAY_READ_DATA_DO_DUMMY			//Do microcontroller dummy read if necessary
		DISPLAY_READ_DATA_DO_DUMMY(data);
	#endif

	DISPLAY_READ_DATA_DO_DUMMY(data);			//First read from SSD1926 is a dummy read (in addition to a dummy microcontroller read if needed)

	DISPLAY_READ_DATA(data);					//Read the register.  Will be 0x2880 for SSD1926 in big endian mode and 0x8028 for little endian mode
							//<<Test value if you want to 
	DISPLAY_CS(1);
	*/



	//See the "SSD1926 Application Note" (seperate to the SSD1926 datasheet) for full details on all of the display configuration parameters.  Whilst
	//quite a daunting document the basic settings to get a new generic display to work are typically quite straightforward by working through this initialisation
	//sequence and adjusting the settings shown for your display.
	

	//---------------------------
	//----- SETUP THE CLOCK -----
	//---------------------------
	//Your display will have a pixel clock frequency range you need to hit.  The following registers need to be setup to create that pixel clock frequency.
	//The PLL setup will typically be as shown unless you are using a different crystal frequency.
	//Once you've created the settings verfiy they are correct (and also that you are correctly talking to the SSD1926) by using a scope on the LCD_SHIFT
	//line to the screen - this is the pixel clock line to the display.

	//----- PLL SETUP -----
	//Crystal frequency x M / N = 80MHz.  For 4MHz crystal:
	display_set_register(0x0126, 0x0a);						//PLL Clock Setting Register 0 - Set N = 10
	display_set_register(0x0127, 200);						//PLL Clock Setting Register 1 - Set M = 200
	display_set_register(0x012b, 0xae);						//PLL Clock Setting Register 2 - Set to 0xAE
	display_set_register(0x0126, 0x8a);						//PLL Clock Setting Register 0 - Enable PLL
     
	//----- VIDEO BUFFER MEMORY CLOCK SETUP -----
	//Memory frequency = PLL frequency / (MCLK + 1)
	display_set_register(0x0004, 0);						//MCLK Divider, 0 = 80MHz

    //----- PIXEL OUTPUT CLOCK SETUP (LCD_SHIFT SIGNAL) -----
	//PCLK frequency = MCLK frequency * (PCLK Frequency Ratio + 1) / 0x100000
	display_set_register(0x0158, 0xcc);						//PCLK Frequency Ratio Register 0 - PCLK = 85196 (0x014ccc) = 6.5MHz pixel clock frequency (LCD_SHIFT)
	display_set_register(0x0159, 0x4c);						//PCLK Frequency Ratio Register 1 - Pixel clock = 10 MHz
	display_set_register(0x015a, 0x01);						//PCLK Frequency Ratio Register 2


	//------------------------------
	//----- SET THE PANEL TYPE -----
	//------------------------------

	//----- PANEL CONFIGURATION -----
    display_set_register(0x0010, 0x71);						//Panel Type Register - non CSTN LCD panel, color LCD panel, 24bit Active Panel Data Width, TFT LCD Panel Type


	//-------------------------------------------
	//----- SETUP THE DISPLAY DRIVE SIGNALS -----
	//-------------------------------------------
	//To get a new display to work set the rotation to 0 (in the Special Effects Register) and work through each of these settings.
	
	//See "Panel Timing Parameters" and "Panel Configuration Registers" section of "SSD1926 Application Note" for details of how the following registers setup the display

	//----- HORIZONTAL TOTAL HT -----
	//Total horizontal pixel clock periods
	//This register must be programmed such that the following condition is fulfilled: HDPS + HDP < HT
	//Horizontal Total ((REG[12h] bits7:0) x 8) + (REG[13h] bits2:0) + 1
	display_set_register(0x0013, ((420 - 1) & 0x0007));		//Horizontal Total Register 0 (bits2:0)
	display_set_register(0x0012, ((420 - 1) >> 3));			//Horizontal Total Register 1 (bits10:3)

	//----- HORIZONTAL DISPLAY PERIOD HDP -----
	//Set to your screen's horizontal pixel resolution
	//Horizontal Display Period ((REG[14h] bits6:0) + 1) x 8
	display_set_register(0x0014, ((320 - 8) / 8));			//Horizontal Display Period Register

	//----- HORIZONTAL DISPLAY PERIOD START HDPS -----
	//Number of null clocks from start of new line before horizontal data is displayed
	//Horizontal Display Period Start Position ((REG[17h] bits2:0, REG[16h] bits7:0))
	display_set_register(0x0016, (68 & 0x0ff)); 			//Horizontal Display Period Start Position Register 0
	display_set_register(0x0017, (68 >> 8));				//Horizontal Display Period Start Position Register 1

	//----- HORIZONTAL SYNC SIGNAL START HPS -----
	//LLINE Pulse Start Position (REG[23h] bits 2-0, REG[22h] bits 7-0) + 1
	display_set_register(0x0022, (0 & 0x0ff)); 				//LLINE Pulse Start Position Register 0
	display_set_register(0x0023, (0 >> 8));					//LLINE Pulse Start Position Register 1

	//----- HORIZONTAL SYNCRONIZATION PULSE WIDTH HPW -----
	//LLINE Pulse Width (REG[20h] bits6:0) + 1
	display_set_register(0x0020, 24);						//LLINE Pulse Width Register: horizontal sync signal is active low




	//----- VERTICAL TOTAL VT -----
	//Vertical Total ((REG[19h] bits2:0, REG[18h] bits7:0) + 1) x HT
	display_set_register(0x0018, (268 & 0x0ff));			//Vertical Total Register 0
	display_set_register(0x0019, (268 >> 8));				//Vertical Total Register 1

	//----- VERTICAL DISPLAY PERIOD VDP -----
	//Set to your screen's vertical pixel resolution
	//Vertical Display Period ((REG[1Dh] bits1:0, REG[1Ch] bits7:0) + 1) x HT
	display_set_register(0x001c, ((240 - 1) & 0x0ff)); 		//Vertical Display Period Register 0
	display_set_register(0x001d, ((240 - 1) >> 8)); 			//Vertical Display Period Register 1

	//----- VERTICAL DISPLAY PERIOD START VDPS -----
	//Vertical Display Period Start Position (REG[1Fh] bits2:0, REG[1Eh] bits7:0) x HT
	display_set_register(0x001e, (18 & 0x0ff)); 			//Vertical Display Period Start Position Register 0
	display_set_register(0x001f, (18 >> 8));				//Vertical Display Period Start Position Register 1

	//----- VERTICAL SYNC SIGNAL START VPS -----
	//LFRAME Pulse Start Position (REG[27h] bits2:0, REG[26h] bits7:0) x HT + (REG[31h] bits2:0, REG[30h] bits7:0)
	display_set_register(0x0026, (0 & 0x0ff));				//Vertical Display Period Start Position Register 0
	display_set_register(0x0027, (0 >> 8));					//Vertical Display Period Start Position Register 1

	//----- VERTICAL SYNCRONIZATION PULSE WIDTH VPW -----
	//LFRAME Pulse Width ((REG[24h] bits2:0) + 1) x HT + (REG[35h] bits2:0, REG[34h] bits7:0) – (REG[31h] bits2:0, REG[30h] bits7:0)
	display_set_register(0x0024, 3);						//LFRAME Pulse Width Register: vertical sync signal is active low.

	//----- SPECIAL EFFECTS REGISTER -----
	display_set_register(0x0071, 0x40);						//Special Effects Register: DisplayDataByteSwap=on, rotation=0º (rotation 90º and 180º require your hoz and vertical parameters to be swapped)




	//----- MAIN WINDOW DISPLAY START ADDRESS -----
	//Note that this is a double-word (32-bit) address. An entry of 00000h into these registers represents the first double-word of display memory, an entry of 00001h represents the second
	//double-word of the display memory, and so on.
	//bits16:0 = Image address ÷ 4 (for Display Rotate Mode 0º)
    display_set_register(0x0074, (0 & 0x0000ff));			//Main Window Display Start Address Register 0
    display_set_register(0x0075, (0 & 0x00ff00) >> 8);		//Main Window Display Start Address Register 1
    display_set_register(0x0076, (0 >> 16));				//Main Window Display Start Address Register 2

	//----- MAIN WINDOW LINE ADDRESS OFFSET -----
	//Specifies the offset in double words from the beginning of one display line to the beginning of the next display line in the main window.
	//Note that this is a 32-bit address increment.
	//bits9:0 = Display width in pixels ÷ (32 ÷ bpp)
	display_set_register(0x0078, ((320 / (32 / 16)) & 0x00ff));		//Main Window Line Address Offset Register 0
	display_set_register(0x0079, ((320 / (32 / 16)) >> 8));			//Main Window Line Address Offset Register 1

	//----- DISPLAY MODE REGISTER -----
	display_set_register(0x0070, 0x04);						//Display Mode Register - Enabled, dithering enabled, display color normal, 16 bits per pixel

	//----- RGB SETTINGS REGISTER -----
	display_set_register(0x01a4, 0xc0);						//RGB Setting Register - RGB (not YUV)

	//----- POWER SAVING CONFIGURATION REGISTER -----
	display_set_register(0x00a0, 0x00);						//Power Saving Configuration Register - On

	//----- LSHIFT POLARITY REGISTER -----
	display_set_register(0x0038, 0x00);						//LSHIFT Polarity Register:
															//0x01 = LSHIFT signal is falling trigger (data changes on falling edge of LSHIFT clock line)
															//0x00 = LSHIFT signal is rising trigger (data changes on rising edge of LSHIFT clock line)


	//----- SATURATION -----
	//display_set_register(0x002c, 0x40);					//Display Post-processing Saturation Control Register: 0x40 = default

	//----- BRIGHTNESS -----
	//display_set_register(0x002d, 0xff);					//Display Post-processing Brightness Control Register: 0x80 = default

	//----- CONTRAST -----
	//display_set_register(0x002e, 0x40);					//Display Post-processing Contrast Control Register: 0x40 = default



	//----- LCD POWER CONTROL REGISTER -----
	//display_set_register(0x00ad, 0x80);					//LCD Power Control Register - Writing a 1 to this bit drives LPOWER high (some implementations use the LPOWER pin to control the LCD bias power)


	//At this point you should have clock on the LCD_SHIFT pin, even if the chip isn't properly configured for your display and even if no display is connected.
	//If you don't then comms to the SSD1926 are bad for some reason.  Check the CNF# pins setup is correct and that there are no shorts etc on the AB3, D15:D0, RD, WR, CS and RESET pins.
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
	WORD_VAL w_temp;
	DWORD_VAL input_colour;
	WORD output_colour;
	
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

	#ifdef DISPLAY_BUS_INTERFACE_IS_BIG_ENDIAN
		//Colour is 16 bit:- 5Red|6Green|5Blue (big endian bus to SSD1926)
		input_colour.Val = colour;
		output_colour = (WORD)(input_colour.v[0] & 0xff) >> 3;		//Blue
		output_colour |= (WORD)(input_colour.v[1] & 0xfc) << 3;		//Green
		output_colour |= (WORD)(input_colour.v[2] & 0xf8) << 8;		//Red
	#else
		//Colour is 16 bit:- 3GreenL|5Blue|5Red|3GreenH (little endian bus to SSD1926)
		input_colour.Val = colour;
		output_colour = (WORD)(input_colour.v[0] & 0xf8) << 5;		//Blue
		output_colour |= (WORD)(input_colour.v[1] & 0xe0) >> 5;		//Green bits 5:3
		output_colour |= (WORD)(input_colour.v[1] & 0x1c) << 11;	//Green bits 2:0
		output_colour |= (WORD)(input_colour.v[2] & 0xf8);			//Red
	#endif


	//----- DO X COORD -----
	address += (x_coord << 1);

	//----- DO Y COORD -----
	address += ((DWORD)y_coord * DISPLAY_WIDTH_PIXELS) << 1;

	//----- SET ADDRESS -----
	DISPLAY_REGISTER_SELECT(0);		//Select Command

	#ifdef DISPLAY_BUS_INTERFACE_IS_BIG_ENDIAN
		//Write M/R#, AB18:8 (SSD1926 in big endian mode)
		//Bit15 represent the M/R#, Bit15 = 1 means memory access, Bit15 = 0 means register access.
		//Bit14:11 = 0.
		//Bit10:0 represent the the address AB18:8.
		w_temp.v[0] = ((DWORD_VAL) address).v[1];
		w_temp.v[1] = ((DWORD_VAL) address).v[2] | 0x80;
		DISPLAY_WRITE_DATA(w_temp.Val);
		
		//Write AB7:0 | Mode_SL (SSD1926 in big endian mode)
		//Bit15:8 represent the address AB7:0 and Bit7:0 represent Mode_SL.
		//Mode_SL to select byte or word access during 16 bit mode. 0x00 means Byte access, 0x01 means word access.
		w_temp.v[0] = 0x01;
		w_temp.v[1] = ((DWORD_VAL) address).v[0];
		DISPLAY_WRITE_DATA(w_temp.Val);
	#else
		//Write AB15:8, M/R#, AB18:16 (SSD1926 in little endian mode)
		//Bit7 represent the M/R# (1 means memory access, 0 means register access)
		//Bit6:3 = 0
		//Bit2:0 | 15:8 represent the the address AB18:8
		w_temp.v[1] = ((DWORD_VAL) address).v[1];
		w_temp.v[0] = ((DWORD_VAL) address).v[2] | 0x80;
		DISPLAY_WRITE_DATA(w_temp.Val);
		
		//Write Mode_SL | AB7:0 (SSD1926 in little endian mode)
		//Bit7:0 represent the address AB7:0 and Bit15:8 represent Mode_SL (0x00 means Byte access, 0x01 means word access)
		w_temp.v[1] = 0x01;
		w_temp.v[0] = ((DWORD_VAL) address).v[0];
		DISPLAY_WRITE_DATA(w_temp.Val);
	#endif

	DISPLAY_REGISTER_SELECT(1);		//Select Data
	
	//----- WRITE PIXEL -----
	DISPLAY_WRITE_DATA(output_colour);
}





//**********************************
//**********************************
//********** SET REGISTER **********
//**********************************
//**********************************
void display_set_register (WORD address, BYTE value)
{
	DISPLAY_REGISTER_SELECT(0);			//Select command
    DISPLAY_CS(0);	

	#ifdef DISPLAY_BUS_INTERFACE_IS_BIG_ENDIAN
		//Write M/R#, AB18:8 (SSD1926 in big endian mode)
		//Bit15 represent the M/R#, Bit15 = 1 means memory access, Bit15 = 0 means register access.
		//Bit14:11 = 0.
		//Bit10:0 represent the the address AB18:8.
		DISPLAY_WRITE_DATA(((WORD_VAL) address).v[1]);
		
		//Write AB7:0 | Mode_SL (SSD1926 in big endian mode)
		//Bit15:8 represent the address AB7:0 and Bit7:0 represent Mode_SL.
		//Mode_SL to select byte or word access during 16 bit mode. 0x00 means Byte access, 0x01 means word access.
		DISPLAY_WRITE_DATA(address << 8);
	
		DISPLAY_REGISTER_SELECT(1);			//Select data
	
	    if(address & 0x0001)
	    {
			DISPLAY_WRITE_DATA(value);
		}
	    else
	    {
			DISPLAY_WRITE_DATA(value << 8);
		}
	#else
		//Write AB15:8, M/R#, AB18:16 (SSD1926 in little endian mode)
		//Bit7 represent the M/R# (1 means memory access, 0 means register access)
		//Bit6:3 = 0
		//Bit2:0 | 15:8 represent the the address AB18:8
		DISPLAY_WRITE_DATA(address & 0xff00);
		
		//Write Mode_SL | AB7:0 (SSD1926 in little endian mode)
		//Bit7:0 represent the address AB7:0 and Bit15:8 represent Mode_SL (0x00 means Byte access, 0x01 means word access)
		DISPLAY_WRITE_DATA(address & 0x00ff);
		
		DISPLAY_REGISTER_SELECT(1);			//Select data
	
	    if(address & 0x0001)
	    {
			DISPLAY_WRITE_DATA(value << 8);
		}
	    else
	    {
			DISPLAY_WRITE_DATA(value);
		}
	#endif
	
	DISPLAY_CS(1);
}



