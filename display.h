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
//DRIVER C CODE HEADER FILE






//*****************************
//*****************************
//********** DEFINES **********
//*****************************
//*****************************
#ifndef DISPLAY_C_INIT		//(Do only once)
#define	DISPLAY_C_INIT


#define	DISPLAY_PROG_MEM_SIZE		DWORD			//DWORD or WORD depending on your device (16/32bit)

//-------------------------------
//----- BITMAP FILE DEFINES -----				//<<<<< CHECK FOR A NEW APPLICATION <<<<<
//-------------------------------

//----- USING PROGRAM MEMORY TO STORE FILES -----
//(Must be commented out if using external flash memory)
#include "display_files\output\display_files_c.h"

//----- USING EXTERNAL FLASH MEMORY TO STORE FILES -----
//(Must all be commented out if using program memory)
//#include "display_files\output\display_files_bin.h"



//------------------------------------
//----- FONT FILE ACCESS DEFINES -----				//<<<<< CHECK FOR A NEW APPLICATION <<<<<
//------------------------------------
//Set as required for your application
#ifdef DISPLAY_FILES_ARE_EXTERNAL		//(Set by the Display Files Bitmap Converter Application for Binary Output option)
	//----- YOU ARE STORING YOUR DISPLAY FILES BITMAP CONVERTER OUTPUT FILE IN EXTERNAL FLASH MEMORY -----
	//You need to provide access to the data:
	#define	DISPLAY_FONT_SET_READ_ADDRESS(address)		display_file_current_address = address						//address is the address starting from 0 within the "display_files_bin.bin" output file
	#define	DISPLAY_FONT_READ_NEXT_BYTE					some_get_byte_function(display_file_current_address++)		//Called repeatedly after DISPLAY_FONT_SET_READ_ADDRESS
#else
	//----- YOU ARE STORING YOUR DISPLAY FILES BITMAP CONVERTER OUTPUT FILE WITHIN PROGRAM MEMORY -----
	//These defines can be left as is
	#define	DISPLAY_FONT_SET_READ_ADDRESS(address)		p_display_file_current_address = (BYTE*)address
	#define	DISPLAY_FONT_READ_NEXT_BYTE					(*p_display_file_current_address++)
#endif



//--------------------------------------
//----- BITMAP FILE ACCESS DEFINES -----				//<<<<< CHECK FOR A NEW APPLICATION <<<<<
//--------------------------------------
//Set as required for your application.  Seperate defines are used to fonts in case your application stores imaage files in a different way to font files.  If the same storage method
//is used these defines will typically be a copy of the font file access defines.
#ifdef DISPLAY_FILES_ARE_EXTERNAL		//(Set by the Display Files Bitmap Converter Application for Binary Output option)
	//----- YOU ARE STORING YOUR DISPLAY FILES BITMAP CONVERTER OUTPUT FILE IN EXTERNAL FLASH MEMORY -----
	//You need to provide access to the data:
	#define	DISPLAY_BITMAP_SET_READ_ADDRESS(address)		display_file_current_address = address					//address is the address starting from 0 within the "display_files_bin.bin" output file.  Comment out if not requried (for instance if you load the file ready to diplay it before calling display_bitmap())
	#define	DISPLAY_BITMAP_READ_NEXT_BYTE					some_get_byte_function(display_file_current_address++)	//Called repeatedly after DISPLAY_BITMAP_SET_READ_ADDRESS
	//#define	DISPLAY_BITMAP_CLOSE_READ					some_function()											//Optional close file define.  Comment out if not requried
#else
	//----- YOU ARE STORING YOUR DISPLAY FILES BITMAP CONVERTER OUTPUT FILE WITHIN PROGRAM MEMORY -----
	//These defines can be left as is
	#define	DISPLAY_BITMAP_SET_READ_ADDRESS(address)		p_display_file_current_address = (BYTE*)address
	#define	DISPLAY_BITMAP_READ_NEXT_BYTE					(*p_display_file_current_address++)
#endif




//----------------------------
//----- STANDARD COLOURS -----
//----------------------------
//32bits: | null | red | green | blue |
#define	DISPLAY_COLOUR_BLACK				0x00000000
#define	DISPLAY_COLOUR_WHITE				0x00ffffff
#define	DISPLAY_COLOUR_RED					0x00ff0000
#define	DISPLAY_COLOUR_GREEN				0x0000ff00
#define	DISPLAY_COLOUR_BLUE					0x000000ff
#define	DISPLAY_COLOUR_YELLOW				0x00ffff00
#define	DISPLAY_COLOUR_TURQUOISE			0x0000ffff
#define	DISPLAY_COLOUR_VIOLET				0x00ff00ff
#define	DISPLAY_COLOUR_GREY_90				0x00191919
#define	DISPLAY_COLOUR_GREY_80				0x00333333
#define	DISPLAY_COLOUR_GREY_70				0x004c4c4c
#define	DISPLAY_COLOUR_GREY_60				0x00666666
#define	DISPLAY_COLOUR_GREY_50				0x007f7f7f
#define	DISPLAY_COLOUR_GREY_40				0x00999999
#define	DISPLAY_COLOUR_GREY_30				0x00b2b2b2
#define	DISPLAY_COLOUR_GREY_20				0x00cccccc
#define	DISPLAY_COLOUR_GREY_10				0x00e5e5e5
#define	DISPLAY_COLOUR_NULL					0xffffffff			//Can be used for display_background_colour to set to no background


//Display Text Options:
#define	DISPLAY_TEXT_ALIGN_LEFT				0x0000
#define	DISPLAY_TEXT_ALIGN_CENTER			0x0001
#define	DISPLAY_TEXT_ALIGN_CENTRE			0x0001
#define	DISPLAY_TEXT_ALIGN_RIGHT			0x0002
#define	DISPLAY_TEXT_ALIGN_MASK				0x0003



#endif  //#ifndef DISPLAY_C_INIT



//*******************************
//*******************************
//********** FUNCTIONS **********
//*******************************
//*******************************
#ifdef DISPLAY_C
//-----------------------------------
//----- INTERNAL ONLY FUNCTIONS -----
//-----------------------------------
CONSTANT BYTE *display_const_string_get_next_char(CONSTANT BYTE *p_string, WORD *next_character);
BYTE *display_variable_string_get_next_char(BYTE *p_string, WORD *next_character);
void display_load_font (CONSTANT BYTE *p_font);
void display_get_font_character (WORD character);
void display_character (void);



//-----------------------------------------
//----- INTERNAL & EXTERNAL FUNCTIONS -----
//-----------------------------------------
//(Also defined below as extern)
void display_initialise (void);
void display_clear_screen (DWORD colour);
WORD display_bitmap (CONSTANT BYTE *p_bitmap, WORD x_coord, WORD y_coord, DWORD transparency_colour);
CONSTANT BYTE *display_const_string (CONSTANT BYTE *p_font, WORD options,
								BYTE horizontal_padding, BYTE vertical_padding,
								WORD x_start_coord, WORD y_start_coord, WORD x_end_coord, WORD y_end_coord,
								CONSTANT BYTE *p_ascii_string);
CONSTANT BYTE *display_variable_string (CONSTANT BYTE *p_font, WORD options,
								BYTE horizontal_padding, BYTE vertical_padding,
								WORD x_start_coord, WORD y_start_coord, WORD x_end_coord, WORD y_end_coord,
								BYTE *p_string);
void display_rectangle (DWORD color, BYTE thickness, WORD x_start_coord, WORD y_start_coord, WORD x_end_coord, WORD y_end_coord);
BYTE display_find_file (BYTE *search_for_filename);
void display_test (void);


#else
//------------------------------
//----- EXTERNAL FUNCTIONS -----
//------------------------------
extern void display_initialise (void);
extern void display_clear_screen (DWORD colour);
extern WORD display_bitmap (CONSTANT BYTE *p_bitmap, WORD x_coord, WORD y_coord, DWORD transparency_colour);
extern CONSTANT BYTE *display_const_string (CONSTANT BYTE *p_font, WORD options,
								BYTE horizontal_padding, BYTE vertical_padding,
								WORD x_start_coord, WORD y_start_coord, WORD x_end_coord, WORD y_end_coord,
								CONSTANT BYTE *p_ascii_string);
extern CONSTANT BYTE *display_variable_string (CONSTANT BYTE *p_font, WORD options,
								BYTE horizontal_padding, BYTE vertical_padding,
								WORD x_start_coord, WORD y_start_coord, WORD x_end_coord, WORD y_end_coord,
								BYTE *p_string);
extern void display_rectangle (DWORD color, BYTE thickness, WORD x_start_coord, WORD y_start_coord, WORD x_end_coord, WORD y_end_coord);
extern BYTE display_find_file (BYTE *search_for_filename);
extern void display_test (void);


#endif




//****************************
//****************************
//********** MEMORY **********
//****************************
//****************************
#ifdef DISPLAY_C
//--------------------------------------------
//----- INTERNAL ONLY MEMORY DEFINITIONS -----
//--------------------------------------------
WORD display_font_spacing;

#ifdef DISPLAY_FILES_ARE_EXTERNAL
	DISPLAY_PROG_MEM_SIZE display_file_current_address;
#else
	CONSTANT BYTE *p_display_file_current_address;
#endif


//--------------------------------------------------
//----- INTERNAL & EXTERNAL MEMORY DEFINITIONS -----
//--------------------------------------------------
//(Also defined below as extern)
DWORD display_foreground_colour;
DWORD display_background_colour;
WORD display_auto_x_coordinate;
WORD display_auto_y_coordinate;
WORD display_bitmap_width;
WORD display_bitmap_height;
WORD display_font_inter_line_space;
WORD display_last_text_line_y_coordinate;
WORD display_font_creation_flags;
WORD display_font_first_character;
WORD display_font_last_character;
DISPLAY_PROG_MEM_SIZE display_font_data_table_address;
DISPLAY_PROG_MEM_SIZE display_font_offset_table_address;
DISPLAY_PROG_MEM_SIZE display_font_width_table_address;
DISPLAY_PROG_MEM_SIZE display_font_extended_index_table_address;
DISPLAY_PROG_MEM_SIZE display_font_start_address;


#else
//---------------------------------------
//----- EXTERNAL MEMORY DEFINITIONS -----
//---------------------------------------
extern DWORD display_foreground_colour;
extern DWORD display_background_colour;
extern WORD display_auto_x_coordinate;
extern WORD display_auto_y_coordinate;
extern WORD display_bitmap_width;
extern WORD display_bitmap_height;
extern WORD display_font_inter_line_space;
extern WORD display_last_text_line_y_coordinate;


#endif




