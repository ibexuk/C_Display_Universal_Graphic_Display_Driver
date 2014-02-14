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
//DRIVER C CODE FILE



//----- INCLUDE FILES FOR THIS SOURCE CODE FILE -----
#include "main.h"					//Global data type definitions (see https://github.com/ibexuk/C_Generic_Header_File )
#define	DISPLAY_C					//(Define used for following header file to flag that it is the header file for this source file)
#include "display.h"				//(Include header file for this source file)
#include "display-model.h"			//Contains defines and functions specific to the screen module being used - if changing display
									//screen change the "display-model.h" file that is included in the project


//----- OTHER PROJECT FILES REQUIRED BY THIS SOURCE CODE FILE -----
#include "ap-main.h"


//----- COMPILER LIBRARY FILES REQUIRED BY THIS SOURCE CODE FILE -----







//***************************************
//***************************************
//********** INITIALISE SCREEN **********
//***************************************
//***************************************
void display_initialise (void)
{
	//Call the initialise function for the screen model in use
	display_model_initialise();

	display_clear_screen(DISPLAY_COLOUR_WHITE); //DISPLAY_COLOUR_BLACK);
}





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
	WORD x_coord = 0;
	WORD count;



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
	for (count = 0; count < 256; count++)
	{
		display_write_pixel(x_coord, 4, colour.Val);
		display_write_pixel(x_coord, 5, colour.Val);
		display_write_pixel(x_coord, 6, colour.Val);
		display_write_pixel(x_coord, 7, colour.Val);
		
		colour.v[2]++;
		x_coord++;
	}
	
	//GREEN COLOUR FADE
	colour.Val = 0;
	x_coord = 4;
	for (count = 0; count < 256; count++)
	{
		display_write_pixel(x_coord, 9, colour.Val);
		display_write_pixel(x_coord, 10, colour.Val);
		display_write_pixel(x_coord, 11, colour.Val);
		display_write_pixel(x_coord, 12, colour.Val);
		
		colour.v[1]++;
		x_coord++;
	}
	
	//BLUE COLOUR FADE
	colour.Val = 0;
	x_coord = 4;
	for (count = 0; count < 256; count++)
	{
		display_write_pixel(x_coord, 14, colour.Val);
		display_write_pixel(x_coord, 15, colour.Val);
		display_write_pixel(x_coord, 16, colour.Val);
		display_write_pixel(x_coord, 17, colour.Val);
		
		colour.v[0]++;
		x_coord++;
	}
	
	//TURQUOISE COLOUR FADE
	colour.Val = 0;
	x_coord = 4;
	for (count = 0; count < 256; count++)
	{
		display_write_pixel(x_coord, 19, colour.Val);
		display_write_pixel(x_coord, 20, colour.Val);
		display_write_pixel(x_coord, 21, colour.Val);
		display_write_pixel(x_coord, 22, colour.Val);
		
		colour.v[1]++;
		colour.v[0]++;
		x_coord++;
	}
	
	
	//VIOLET COLOUR FADE
	colour.Val = 0;
	x_coord = 4;
	for (count = 0; count < 256; count++)
	{
		display_write_pixel(x_coord, 24, colour.Val);
		display_write_pixel(x_coord, 25, colour.Val);
		display_write_pixel(x_coord, 26, colour.Val);
		display_write_pixel(x_coord, 27, colour.Val);
		
		colour.v[2]++;
		colour.v[0]++;
		x_coord++;
	}
	
	//YELLOW COLOUR FADE
	colour.Val = 0;
	x_coord = 4;
	for (count = 0; count < 256; count++)
	{
		display_write_pixel(x_coord, 29, colour.Val);
		display_write_pixel(x_coord, 30, colour.Val);
		display_write_pixel(x_coord, 31, colour.Val);
		display_write_pixel(x_coord, 32, colour.Val);
		
		colour.v[2]++;
		colour.v[1]++;
		x_coord++;
	}
	
	//WHITE COLOUR FADE
	colour.Val = 0;
	x_coord = 4;
	for (count = 0; count < 256; count++)
	{
		display_write_pixel(x_coord, 34, colour.Val);
		display_write_pixel(x_coord, 35, colour.Val);
		display_write_pixel(x_coord, 36, colour.Val);
		display_write_pixel(x_coord, 37, colour.Val);
		
		colour.v[2]++;
		colour.v[1]++;
		colour.v[0]++;
		x_coord++;
	}

	
	//display_foreground_colour = DISPLAY_COLOUR_WHITE;
	//display_write_pixel(address, 239, display_foreground_colour);
	
	DISPLAY_CS(1);
}




//**********************************
//**********************************
//********** CLEAR SCREEN **********
//**********************************
//**********************************
void display_clear_screen (DWORD colour)
{
	display_write_block(0, 0, (DISPLAY_WIDTH_PIXELS - 1), (DISPLAY_HEIGHT_PIXELS - 1), colour);
}




//***************************************
//***************************************
//********** DISPLAY RECTANGLE **********
//***************************************
//***************************************
//thickness		Set to 0 to create a rectangle fill
void display_rectangle (DWORD color, BYTE thickness, WORD x_start_coord, WORD y_start_coord, WORD x_end_coord, WORD y_end_coord)
{

	if (x_start_coord > x_end_coord)
		return;
		
	if (y_start_coord > y_end_coord)
		return;

	if ((x_start_coord + thickness)  > (x_end_coord - thickness))
		return;
		
	if ((y_start_coord + thickness)  > (y_end_coord - thickness))
		return;

	if (x_end_coord >= DISPLAY_WIDTH_PIXELS)
		return;

	if (y_end_coord >= DISPLAY_HEIGHT_PIXELS)
		return;


	if (thickness == 0)
	{
		//--------------------------
		//----- RECTANGLE FILL -----
		//--------------------------
		display_write_block(x_start_coord, y_start_coord, x_end_coord, y_end_coord, color);
	}
	else
	{
		//--------------------------
		//----- RECTANGLE LINE -----
		//--------------------------
		
		thickness -= 1;

		//TOP
		display_write_block(x_start_coord,						//x_start_coord
							y_start_coord,						//y_start_coord
							x_end_coord,						//x_end_coord
							(y_start_coord + thickness),		//y_end_coord
							color);								//colour

		//LEFT
		display_write_block(x_start_coord,						//x_start_coord
							(y_start_coord + thickness + 1),	//y_start_coord
							(x_start_coord + thickness),		//x_end_coord
							(y_end_coord - thickness - 1),		//y_end_coord
							color);								//colour

		//RIGHT
		display_write_block((x_end_coord - thickness),			//x_start_coord
							(y_start_coord + thickness + 1),	//y_start_coord
							x_end_coord,						//x_end_coord
							(y_end_coord - thickness - 1),		//y_end_coord
							color);								//colour

		//BOTTOM
		display_write_block(x_start_coord,						//x_start_coord
							(y_end_coord - thickness),			//y_start_coord
							x_end_coord,						//x_end_coord
							y_end_coord,						//y_end_coord
							color);								//colour
	}

}







//************************************
//************************************
//********** DISPLAY BITMAP **********
//************************************
//************************************
//The display driver works with .bmp files, which are uncompressed so fast to display and allow the full colour pallet to be used.
//The driver supports uncompressed "Windows" formatted .BMP files of the following types:
//	1 bit Monochrome bitmap
//	4 bit 16 colour bitmap
//	8 bit 256 colour bitmap (usually the best setting to use for non photo images due to the smaller file size)
//	16 bit (high colour) bitmap (many display drivers only provide 16 bits of colour resolution making using files with a larger bit value pointless).
//	24 bit (true colour) bitmap
//	32 bit (true colour) bitmap (not recommended as the 4th byte is unused resulting in no additional actual bitmap data – use 24 bit instead to reduce file size).
//The bitmap size must not be larger than the screen resolution.
//Note that files must be saves as uncompressed.  The Embedded Code sample files we’re created using Adobe Photoshop and Illustrator.
//
//p_bitmap				Pointer to the bitmap file you want to display (or 0x00 if you have setup some file source ready for DISPLAY_BITMAP_READ_NEXT_BYTE to be called)
//x_coord				Left starting coord
//y_coord				Top starting coord
//transparency_colour	Optional colour that should be treated as transparent (not written to the screen).  Send as 0xffffffff to not use.
//Returns				The height of the bitmap displayed
//After calling			display_auto_x_coordinate = the pixel after the right of the image
//						display_auto_y_coordinate = the pixel after the bottom pixel of the image
WORD display_bitmap (CONSTANT BYTE *p_bitmap, WORD x_coord, WORD y_coord, DWORD transparency_colour)
{
	BYTE data;
	DISPLAY_PROG_MEM_SIZE file_index;
	DWORD_VAL bitmap_data_offset;
	WORD_VAL bitmap_width;
	WORD_VAL bitmap_height;
	WORD_VAL bits_per_pixel;
	DWORD_VAL bitmap_data_size;
	DWORD_VAL number_of_colours;
	WORD row_index;
	WORD column_index;
	BYTE bit_index;
	BYTE palette_red[256];
	BYTE palette_green[256];
	BYTE palette_blue[256];
	DWORD_VAL colour;
	WORD count;
	

	colour.Val = 0;

#ifdef DISPLAY_BITMAP_SET_READ_ADDRESS
	if (p_bitmap)
	{
		//FILE NAME [#]
		//Move past the file name
		data = 1;
		while (data != 0)			//Look for null termination
			data = *p_bitmap++;
		
		//FILE SIZE [4]
		//Move past the file size
		p_bitmap += 4;
			
		//FILE START ADDRESS
		#ifdef DISPLAY_FILES_ARE_EXTERNAL			//This define gets set by the Display Files Conveter application for binary file outputs
			file_index = ((DISPLAY_PROG_MEM_SIZE)*p_bitmap++) << 24;
			file_index |= ((DISPLAY_PROG_MEM_SIZE)*p_bitmap++) << 16;
			file_index |= ((DISPLAY_PROG_MEM_SIZE)*p_bitmap++) << 8;
			file_index |= (DISPLAY_PROG_MEM_SIZE)*p_bitmap++;
		#else
			file_index = (DISPLAY_PROG_MEM_SIZE)p_bitmap;
		#endif
		
		DISPLAY_BITMAP_SET_READ_ADDRESS(file_index);
	}
#endif
	file_index = 0;

	//----------------------------------
	//----- READ THE BITMAP HEADER -----
	//----------------------------------
	
	//----- GET IDENTIFIER [2] -----
	//(Only allow 'BM' identifier - ignore others (others are non windows formats which we don't support))
	data = DISPLAY_BITMAP_READ_NEXT_BYTE;
	if (data != 'B')
		goto display_bitmap_fail;

	data = DISPLAY_BITMAP_READ_NEXT_BYTE;
	if (data != 'M')
		goto display_bitmap_fail;
	
	file_index += 2;
	
	//----- GET FILE SIZE [4] -----
	data = DISPLAY_BITMAP_READ_NEXT_BYTE;
	data = DISPLAY_BITMAP_READ_NEXT_BYTE;
	data = DISPLAY_BITMAP_READ_NEXT_BYTE;
	data = DISPLAY_BITMAP_READ_NEXT_BYTE;
	file_index += 4;
	
	//----- GET RESERVED [4] -----
	data = DISPLAY_BITMAP_READ_NEXT_BYTE;
	data = DISPLAY_BITMAP_READ_NEXT_BYTE;
	data = DISPLAY_BITMAP_READ_NEXT_BYTE;
	data = DISPLAY_BITMAP_READ_NEXT_BYTE;
	file_index += 4;
	
	//----- GET BITMAP DATA OFFSET [4] -----
	bitmap_data_offset.v[0] = DISPLAY_BITMAP_READ_NEXT_BYTE;
	bitmap_data_offset.v[1] = DISPLAY_BITMAP_READ_NEXT_BYTE;
	bitmap_data_offset.v[2] = DISPLAY_BITMAP_READ_NEXT_BYTE;
	bitmap_data_offset.v[3] = DISPLAY_BITMAP_READ_NEXT_BYTE;
	file_index += 4;
	
	//----- GET BITMAP HEADER SIZE [4] -----
	//(Only allow 0x00000028 - ignore others (others are non windows formats which we don't support))
	data = DISPLAY_BITMAP_READ_NEXT_BYTE;
	if (data != 0x28)
		goto display_bitmap_fail;

	data = DISPLAY_BITMAP_READ_NEXT_BYTE;
	if (data != 0x00)
		goto display_bitmap_fail;

	data = DISPLAY_BITMAP_READ_NEXT_BYTE;
	if (data != 0x00)
		goto display_bitmap_fail;

	data = DISPLAY_BITMAP_READ_NEXT_BYTE;
	if (data != 0x00)
		goto display_bitmap_fail;
		
	file_index += 4;

	//----- GET BITMAP WIDTH IN PIXELS [4] -----
	bitmap_width.v[0] = DISPLAY_BITMAP_READ_NEXT_BYTE;
	bitmap_width.v[1] = DISPLAY_BITMAP_READ_NEXT_BYTE;

	data = DISPLAY_BITMAP_READ_NEXT_BYTE;
	if (data != 0x00)
		goto display_bitmap_fail;

	data = DISPLAY_BITMAP_READ_NEXT_BYTE;
	if (data != 0x00)
		goto display_bitmap_fail;

	if (bitmap_width.Val > DISPLAY_WIDTH_PIXELS)
		goto display_bitmap_fail;
		
	file_index += 4;


	//Store the end coordinate of the image
	display_auto_x_coordinate = x_coord + bitmap_width.Val;


	//----- GET BITMAP HEIGHT IN PIXELS [4] -----
	bitmap_height.v[0] = DISPLAY_BITMAP_READ_NEXT_BYTE;
	bitmap_height.v[1] = DISPLAY_BITMAP_READ_NEXT_BYTE;

	data = DISPLAY_BITMAP_READ_NEXT_BYTE;
	if (data != 0x00)
		goto display_bitmap_fail;

	data = DISPLAY_BITMAP_READ_NEXT_BYTE;
	if (data != 0x00)
		goto display_bitmap_fail;
		
	if (bitmap_height.Val > DISPLAY_HEIGHT_PIXELS)
		goto display_bitmap_fail;
		
	file_index += 4;
	

	//Store the end coordinate of the image
	display_auto_y_coordinate = y_coord + bitmap_height.Val;
	
	
	//----- GET NUMBER OF BITMAP PLANES [2] -----
	//We only accept 1 plane
	data = DISPLAY_BITMAP_READ_NEXT_BYTE;
	if (data != 0x01)
		goto display_bitmap_fail;
		
	data = DISPLAY_BITMAP_READ_NEXT_BYTE;
	if (data != 0x00)
		goto display_bitmap_fail;
		
	file_index += 2;
	
	//----- GET NUMBER OF BITS PER PIXEL [2] -----
	//Bits per pixel used to store palette entry information. This also identifies in an indirect way the number of possible colors. Possible values are:
	//	1 - Monochrome bitmap, 4 - 16 color bitmap, 8 - 256 color bitmap, 16 - 16bit (high color) bitmap, 24 - 24bit (true color) bitmap, 32 - 32bit (true color) bitmap

	bits_per_pixel.v[0] = DISPLAY_BITMAP_READ_NEXT_BYTE;
	bits_per_pixel.v[1] = DISPLAY_BITMAP_READ_NEXT_BYTE;
	file_index += 2;

	//----- GET COMPRESSION [4] -----
	//We only accept 0 (none / BI_RGB)
	data = DISPLAY_BITMAP_READ_NEXT_BYTE;
	if (data != 0x00)
		goto display_bitmap_fail;
		
	data = DISPLAY_BITMAP_READ_NEXT_BYTE;
	if (data != 0x00)
		goto display_bitmap_fail;
		
	data = DISPLAY_BITMAP_READ_NEXT_BYTE;
	if (data != 0x00)
		goto display_bitmap_fail;
		
	data = DISPLAY_BITMAP_READ_NEXT_BYTE;
	if (data != 0x00)
		goto display_bitmap_fail;
	
	file_index += 4;
	
	//----- GET BITMAP DATA SIZE [4] -----
	//Size of the bitmap data in bytes. This number must be rounded to the next 4 byte boundary (when added to bitmap_data_offset).
	bitmap_data_size.v[0] = DISPLAY_BITMAP_READ_NEXT_BYTE;
	bitmap_data_size.v[1] = DISPLAY_BITMAP_READ_NEXT_BYTE;
	bitmap_data_size.v[2] = DISPLAY_BITMAP_READ_NEXT_BYTE;
	bitmap_data_size.v[3] = DISPLAY_BITMAP_READ_NEXT_BYTE;
	file_index += 4;
	
	//----- GET HORIZONTAL RESOLUTION [4] -----
	//Dump - don't care
	data = DISPLAY_BITMAP_READ_NEXT_BYTE;
	data = DISPLAY_BITMAP_READ_NEXT_BYTE;
	data = DISPLAY_BITMAP_READ_NEXT_BYTE;
	data = DISPLAY_BITMAP_READ_NEXT_BYTE;
	file_index += 4;
	
	//----- GET VERTICAL RESOLUTION [4] -----
	//Dump - don't care
	data = DISPLAY_BITMAP_READ_NEXT_BYTE;
	data = DISPLAY_BITMAP_READ_NEXT_BYTE;
	data = DISPLAY_BITMAP_READ_NEXT_BYTE;
	data = DISPLAY_BITMAP_READ_NEXT_BYTE;
	file_index += 4;
	
	//----- GET NUMBER OF COLOURS [4] -----
	//Number of colors used by this bitmap. For a 8-bit / pixel bitmap this will be 256.
	//If this value is zero, the bitmap uses the maximum number of colors corresponding to the value of the bits per pixel field for the compression mode specified
	//by the Compression field. 
	//If the Colors field is nonzero and the bits per pixel field less than 16, the Colors field specifies the actual number of colors the graphics engine or device
	//driver accesses. 
	//If the bits per pixel field  is 16 or greater, then Colors field specifies the size of the color table used to optimize performance of Windows color palettes. 
	//If bits per pixel equals 16 or 32, the optimal color palette starts immediately following the three double word masks.
 	//If the bitmap is a packed bitmap (a bitmap in which the bitmap array immediately follows the bitmap header and which is referenced by a single pointer), the
 	//Colors field must be either 0 or the actual size of the color table. 
	number_of_colours.v[0] = DISPLAY_BITMAP_READ_NEXT_BYTE;
	number_of_colours.v[1] = DISPLAY_BITMAP_READ_NEXT_BYTE;
	number_of_colours.v[2] = DISPLAY_BITMAP_READ_NEXT_BYTE;
	number_of_colours.v[3] = DISPLAY_BITMAP_READ_NEXT_BYTE;
	
	if (number_of_colours.Val > 256)
		goto display_bitmap_fail;
	if (number_of_colours.Val == 0)
	{
		switch (bits_per_pixel.Val)
		{
		case 1:
			number_of_colours.Val = 2;
			break;
		case 4:
			number_of_colours.Val = 16;
			break;
		case 8:
			number_of_colours.Val = 256;
			break;
		}
	}
		
	file_index += 4;

	//----- GET NUMBER OF IMPORTANT COLOURS [4] -----
	//Dump - don't care
	data = DISPLAY_BITMAP_READ_NEXT_BYTE;
	data = DISPLAY_BITMAP_READ_NEXT_BYTE;
	data = DISPLAY_BITMAP_READ_NEXT_BYTE;
	data = DISPLAY_BITMAP_READ_NEXT_BYTE;
	file_index += 4;
	
	//----- GET PALETTE [#] -----
	//The palette specification. For every entry in the palette four bytes are used to describe the RGB values of the color in the following way:
	//	1 byte for blue component
	//	1 byte for green component
	//	1 byte for red component
	//	1 byte filler which is set to zero

	if (bits_per_pixel.Val < 16)
	{
		for (count = 0; count < number_of_colours.Val; count++)
		{
			if (file_index >= bitmap_data_offset.Val)			//Just in case - shouldn't be requried
				break;
				
			palette_blue[count] = DISPLAY_BITMAP_READ_NEXT_BYTE;
			palette_green[count] = DISPLAY_BITMAP_READ_NEXT_BYTE;
			palette_red[count] = DISPLAY_BITMAP_READ_NEXT_BYTE;
			data = DISPLAY_BITMAP_READ_NEXT_BYTE;
			file_index += 4;
		}
	}

	//OFFSET TO THE START OF THE BITMAP DATA
	while (file_index < (bitmap_data_offset.Val - 1))
	{
		data = DISPLAY_BITMAP_READ_NEXT_BYTE;
		file_index++;
	}


	//--------------------------------
	//----- READ THE BITMAP DATA -----
	//--------------------------------
	//The data is stored in the .bmp file with # bits per pixel, starting from the bottom row of the bitmap.
	//Each line must end on a 4 byte boundary (i.e. null bits are added to lines that do not naturally end on a 4 byte boundary)

	DISPLAY_CS(0);
	for (row_index = (y_coord + bitmap_height.Val - 1); row_index >= y_coord; row_index--)
	{
		//----------------------------
		//----- DISPLAY NEXT ROW -----
		//----------------------------
		switch (bits_per_pixel.Val)
		{
		case 1:
			//-----------------------------------------------
			//----- 1 BIT PER PIXEL - MONOCHROME BITMAP -----
			//-----------------------------------------------
			//The palette contains two entries. Each bit in the bitmap array represents a pixel. If the bit is clear, the pixel is displayed with the color of the first entry
			//in the palette; if the bit is set, the pixel has the color of the second entry in the table.

			for (column_index = 0; column_index < bitmap_width.Val; )
			{
				data = DISPLAY_BITMAP_READ_NEXT_BYTE;
				file_index++;
				
				bit_index = 0x80;
				while (bit_index)
				{
					if (column_index >= bitmap_width.Val)		//Check for end of row
						break;
						
					if (data & bit_index)
					{
						colour.v[2] = palette_red[1];
						colour.v[1] = palette_green[1];
						colour.v[0] = palette_blue[1];
					}
					else
					{
						colour.v[2] = palette_red[0];
						colour.v[1] = palette_green[0];
						colour.v[0] = palette_blue[0];
					}
					if (colour.Val != transparency_colour)
						display_write_pixel((x_coord + column_index), row_index, colour.Val);
					
					bit_index >>= 1;
					column_index++;
				} //do while (bit_index)
			} //for (column_index = x_coord; column_index < bitmap_width; )

			break;

		case 4:
			//----------------------------------------------
			//----- 4 BITS PER PIXEL - 16 COLOR BITMAP -----
			//----------------------------------------------
			//The palette contains up to 16 entries. Each pixel in the bitmap is represented by a 4-bit index into the palette. For example, if the first byte in the bitmap is
			//1Fh, the byte represents two pixels. The first pixel contains the color in the second palette entry, and the second pixel contains the color in the sixteenth palette entry.

			for (column_index = 0; column_index < bitmap_width.Val; )
			{
				data = DISPLAY_BITMAP_READ_NEXT_BYTE;
				file_index++;
				
				colour.v[2] = palette_red[(data >> 4)];
				colour.v[1] = palette_green[(data >> 4)];
				colour.v[0] = palette_blue[(data >> 4)];
				if (colour.Val != transparency_colour)
					display_write_pixel((x_coord + column_index), row_index, colour.Val);
				column_index++;
				
				colour.v[2] = palette_red[(data & 0x0f)];
				colour.v[1] = palette_green[(data & 0x0f)];
				colour.v[0] = palette_blue[(data & 0x0f)];
				if (colour.Val != transparency_colour)
					display_write_pixel((x_coord + column_index), row_index, colour.Val);
				column_index++;
				
			} //for (column_index = x_coord; column_index < bitmap_width; )
			
			break;
			
		case 8:
			//-----------------------------------------------
			//----- 8 BITS PER PIXEL - 256 COLOR BITMAP -----
			//-----------------------------------------------
			//The palette contains up to 256 entries. In this case, each byte in the array represents a single pixel.

			for (column_index = 0; column_index < bitmap_width.Val; column_index++)
			{
				data = DISPLAY_BITMAP_READ_NEXT_BYTE;
				file_index++;
				
				colour.v[2] = palette_red[data];
				colour.v[1] = palette_green[data];
				colour.v[0] = palette_blue[data];
				if (colour.Val != transparency_colour)
					display_write_pixel((x_coord + column_index), row_index, colour.Val);
				
			} //for (column_index = x_coord; column_index < bitmap_width; column_index++)
			
			break;
			
		case 16:
			//---------------------------------------------------------
			//----- 16 BITS PER PIXEL - 16BIT (HIGH COLOR) BITMAP -----
			//---------------------------------------------------------
			//The bitmap has a maximum of 2^16 colors. If the Compression field of the bitmap file is set to 0 (BI_RGB), the Palette field does not contain any entries. Each word in the
			//bitmap array represents a single pixel. The relative intensities of red, green, and blue are represented with 5 bits for each color component. The value for blue is in the
			//least significant 5 bits, followed by 5 bits each for green and red, respectively. The most significant bit is not used.
			//Source data:
			// |0RRRRRGG|GGGBBBBB|
			//Becomes:
			// |00000000|RRRRR000|GGGGG000|BBBBB000|

			for (column_index = 0; column_index < bitmap_width.Val; column_index++)
			{
				data = DISPLAY_BITMAP_READ_NEXT_BYTE;
				colour.v[0] = (data & 0x1f) << 3;		//Blue
				colour.v[1] = (data & 0xe0) >> 2;		//Green

				data = DISPLAY_BITMAP_READ_NEXT_BYTE;
				colour.v[1] |= (data & 0x03) << 6;		//Green
				colour.v[2] = (data & 0x7c) << 1;		//Red
				
				//Bottom 3 bits should be on if color pixel is not off
				if (colour.v[0] & 0xf8)
					colour.v[0] |= 0x07;
				if (colour.v[1] & 0xf8)
					colour.v[1] |= 0x07;
				if (colour.v[2] & 0xf8)
					colour.v[2] |= 0x07;

				if (colour.Val != transparency_colour)
					display_write_pixel((x_coord + column_index), row_index, colour.Val);
				
				file_index += 2;
				
			} //for (column_index = x_coord; column_index < bitmap_width; column_index++)
			
			break;
			
		case 24:
			//---------------------------------------------------------
			//----- 24 BITS PER PIXEL - 24BIT (TRUE COLOR) BITMAP -----
			//---------------------------------------------------------
			//The bitmap has a maximum of 2^24 colors, and the Palette field does not contain any entries. Each 3-byte triplet in the bitmap array represents the relative intensities of
			//blue, green, and red, respectively, for a pixel.

			for (column_index = 0; column_index < bitmap_width.Val; column_index++)
			{
				colour.v[0] = DISPLAY_BITMAP_READ_NEXT_BYTE;
				colour.v[1] = DISPLAY_BITMAP_READ_NEXT_BYTE;
				colour.v[2] = DISPLAY_BITMAP_READ_NEXT_BYTE;
				
				if (colour.Val != transparency_colour)
					display_write_pixel((x_coord + column_index), row_index, colour.Val);
				
				file_index += 3;
				
			} //for (column_index = x_coord; column_index < bitmap_width; column_index++)
			
			break;
			
		case 32:
			//---------------------------------------------------------
			//----- 32 BITS PER PIXEL - 32BIT (TRUE COLOR) BITMAP -----
			//---------------------------------------------------------
			//The bitmap has a maximum of 2^32 colors. If the Compression field of the bitmap is set to 9 (BI_RGB), the Palette field does not contain any entries. Each dword in the bitmap
			//array represents the relative intensities of blue, green, and red, respectively, for a pixel. The high byte in each dword is not used.

			for (column_index = 0; column_index < bitmap_width.Val; column_index++)
			{
				colour.v[0] = DISPLAY_BITMAP_READ_NEXT_BYTE;
				colour.v[1] = DISPLAY_BITMAP_READ_NEXT_BYTE;
				colour.v[2] = DISPLAY_BITMAP_READ_NEXT_BYTE;
				data = DISPLAY_BITMAP_READ_NEXT_BYTE;
				
				if (colour.Val != transparency_colour)
					display_write_pixel((x_coord + column_index), row_index, colour.Val);
				
				file_index += 4;
				
			} //for (column_index = x_coord; column_index < bitmap_width; column_index++)
			
			break;


		} //switch (bits_per_pixel)
		
		
		//----- EACH LINE MUST END ON A 4 BYTE BOUNDARY -----
		while ((file_index - bitmap_data_offset.Val) & 0x0003)
		{
			data = DISPLAY_BITMAP_READ_NEXT_BYTE;
			file_index++;
		}

		if (row_index == y_coord)
			break;
	} //for (row_index = 0; row_index < bitmap_height; row_index++)		
	
	
display_bitmap_fail:
	//----- CLOSE THE FILE -----
	#ifdef DISPLAY_BITMAP_CLOSE_READ
		DISPLAY_BITMAP_CLOSE_READ;
	#endif
	DISPLAY_CS(1);
	return (bitmap_height.Val);
}






//******************************************
//******************************************
//********** DISPLAY TEXT STRING  **********
//******************************************
//******************************************
//Use the display_const_string and display_variable_string functions to display null terminated strings. Individual strings may be displayed with left, centre or right
//alignment and optionally contained within a box with padding and automatic multi line display if necessary.
//The X and Y start coordinate values specify the top left position of where to place the text, with the top left corner of the screen being coordinate 0, 0.
//If an end x coordinate is specified:
//	The start and end values create a virtual box for the text to be displayed within.  If a y end coordinate is also specified this creates the bottom of the virtual
//	box or if not the virtual box is made as high as required to display the text on one or more lines, with the bottom of the screen being the limit if the text is
//	too long to fit on the screen.
//If an end x coordinate is not specified:
//	The x start coordinate specifies the left, centre or right insertion point for the text depending on the alignment option specified.  A virtual box is created based
//	on the text width, using the screen edge as a limit if the text is too long to fit on a single line (i.e. the virtual box automatically becomes as wide as needed to
//	hold the text).  If the text is too long to fit on a single line a new virtual box is created for each subsequent line.  If a y coordinate is specified this limits
//	the bottom of the text, with the bottom of the screen being the limit if the text is too long to fit on the screen.
//When the string will be displayed on multiple lines line breaks are automatically created at space characters.
//The horizontal and vertical padding values may be set to a non zero value to cause the text to be contained within a smaller virtual inner box.  When using background colors this creates a blank area of background around the text, or where the text reaches the screen edge it ensures the specified number of pixels are not used (effectively moving the screen edge in by the number of pixels specified).
//Before calling:
//	Set display_foreground_colour with the requried text colour
//	Set display_background_colour with the requried background colour.  Set to 0xffffffff for no background colour (leave
//	existing colour).
//Call with:
//	*p_font							Pointer to the font to be used (or 0x00 if you have setup some file source ready for DISPLAY_FONT_READ_NEXT_BYTE to be called)
//	options							Text display options (see display.h)
//	horizontal_padding				Pixels of background requried left and right of text
//	vertical_padding				Pixels of background requried above and below text
//	x_start_coord, y_start_coord	Start coordinate
//	x_end_coord, y_end_coord		End coordinate (either may be 0 if area containment not required)
//	*p_ascii_string					Pointer to the string to display
//Returns 0x00 if entire string was displayed, or the pointer to the start of the next word if the entire string wouldn't fit
//After calling:
//	display_auto_x_coordinate = the pixel after the last pixel of the last character
//	display_auto_y_coordinate = the pixel below the last line displayed


//----------------------------------------
//----------------------------------------
//----- VERSION FOR CONSTANT STRINGS -----
//----------------------------------------
//----------------------------------------
CONSTANT BYTE *display_const_string (CONSTANT BYTE *p_font, WORD options,
								BYTE horizontal_padding, BYTE vertical_padding,
								WORD x_start_coord, WORD y_start_coord, WORD x_end_coord, WORD y_end_coord,
								CONSTANT BYTE *p_string)
{
	WORD next_character;
	CONSTANT BYTE *p_string_start_of_line;
	CONSTANT BYTE *p_string_end_of_line;
	CONSTANT BYTE *p_string_last_end_of_word;
	WORD string_width_at_last_end_of_word;
	WORD x_start_coord_copy;
	WORD w_temp;
	WORD string_width;
	BYTE text_remaining = 1;
	WORD max_width;
	WORD min_width;
	WORD leading_pad_width;
	WORD trailing_pad_width;
	BYTE doing_1st_line = 1;
	BYTE doing_vertical_align_dummy_run = 0;
	WORD vertical_padding_top = vertical_padding;			//Used for vertical align
	CONSTANT BYTE *p_string_copy;


	p_string_copy = p_string;
	p_string_start_of_line = p_string;
	x_start_coord_copy = x_start_coord;
	display_auto_y_coordinate = y_start_coord;


	//-------------------------
	//----- LOAD THE FONT -----
	//-------------------------
	display_load_font(p_font);

	//-------------------------------------------------
	//----- ENSURE START COORDIANTES ARE TOP LEFT -----
	//-------------------------------------------------
	if (x_end_coord)
	{
		if (x_start_coord > x_end_coord)
		{
			w_temp = x_start_coord;
			x_start_coord = x_end_coord;
			x_end_coord = w_temp;
		}
	}
	if (y_end_coord)
	{
		if (y_start_coord > y_end_coord)
		{
			w_temp = y_start_coord;
			y_start_coord = y_end_coord;
			y_end_coord = w_temp;
		}
	}


	//-------------------------------------
	//----- CALCULATE MIN & MAX WIDTH -----
	//-------------------------------------
	min_width = 0;
	if (x_end_coord)
	{
		max_width = min_width = (x_end_coord - x_start_coord) + 1;
	}
	else
	{
		if ((options & DISPLAY_TEXT_ALIGN_MASK) == DISPLAY_TEXT_ALIGN_LEFT)
		{
			max_width = DISPLAY_WIDTH_PIXELS - x_start_coord;
		}
		else if ((options & DISPLAY_TEXT_ALIGN_MASK) == DISPLAY_TEXT_ALIGN_RIGHT)
		{
			max_width = x_start_coord + 1;
		}
		else
		{
			if (x_start_coord > (DISPLAY_WIDTH_PIXELS >> 1))
				max_width = ((DISPLAY_WIDTH_PIXELS - x_start_coord) << 1);
			else
				max_width = (x_start_coord << 1);
		}
	}
	
	if (horizontal_padding)
		max_width -= (horizontal_padding << 1);
	
	if (max_width > DISPLAY_WIDTH_PIXELS)
		return(0);					//Error
	if (max_width == 0)
		return(0);					//Error


	//------------------------------------
	//----- CHECK FOR VERTICAL ALIGN -----
	//------------------------------------
	if (
		((options & DISPLAY_TEXT_VALIGN_MASK) == DISPLAY_TEXT_ALIGN_MIDDLE) ||
		((options & DISPLAY_TEXT_VALIGN_MASK) == DISPLAY_TEXT_ALIGN_BOTTOM)
	)
	{
		doing_vertical_align_dummy_run = 1;
	}
	
	while (1)
	{
		while (text_remaining)
		{
			//-----------------------------
			//-----------------------------
			//----- DISPLAY NEXT LINE -----
			//-----------------------------
			//-----------------------------
			
			x_start_coord = x_start_coord_copy;
			
			//-------------------------------------------------------
			//----- GET WIDTH OF STRING TO DISPLAY ON THIS LINE -----
			//-------------------------------------------------------
			string_width = 0;
			p_string = p_string_end_of_line = p_string_start_of_line;
			string_width_at_last_end_of_word = 0;
			

			while (1)
			{
				p_string = display_const_string_get_next_char(p_string, &next_character);
				if (next_character == 0x00)			//Do until null terminator
					break;

				if (next_character == ' ')
				{
					//-------------------------
					//----- END OF A WORD -----
					//-------------------------
					p_string_last_end_of_word = p_string - 2;
					string_width_at_last_end_of_word = (string_width - display_font_spacing);
				}
				
				display_get_font_character(next_character);		//Loads display_bitmap_width
				string_width += display_bitmap_width;
				
				if (string_width > max_width)
				{
					//--------------------------------------------------------------
					//----- TOO LONG FOR WIDTH - MOVE BACK TO LAST END OF WORD -----
					//--------------------------------------------------------------
					if (string_width_at_last_end_of_word == 0)
						return(0);									//No end of word found - unable to display
					
					//Reset ready for display of string
					p_string = p_string_start_of_line;
					p_string_end_of_line = p_string_last_end_of_word;
					string_width = string_width_at_last_end_of_word;
					
					//Setup for display of next line
					p_string_start_of_line = (p_string_last_end_of_word + 2);		//Move past space character
					break;
				}
				
				string_width += display_font_spacing;			//Add gap to next character
			}
			
			if (p_string_end_of_line == p_string_start_of_line)
			{
				//----- END OF STRING WAS REACHED -----
				p_string_end_of_line = (p_string - 2);
				p_string = p_string_start_of_line;
				string_width -= display_font_spacing;			//Remove the final inter character gap that won't be displayed
				text_remaining = 0;
				p_string_start_of_line = 0;
			}
			

			
			//We now have:
			//	p_string				1st character to display
			//	p_string_end_of_line	Last character to display
			//	string_width			Total width of this lines characters
			//	p_string_start_of_line	1st character of next line to display
			
			
			
			if ((options & DISPLAY_TEXT_ALIGN_MASK) == DISPLAY_TEXT_ALIGN_LEFT)
			{
				//------------------------
				//----- LEFT ALIGNED -----
				//------------------------
				leading_pad_width = horizontal_padding;
				trailing_pad_width = horizontal_padding;
				if (x_end_coord)
				{
					trailing_pad_width += (max_width - string_width);		//max_width is the requried width after padding
				}
			}
			else if ((options & DISPLAY_TEXT_ALIGN_MASK) == DISPLAY_TEXT_ALIGN_RIGHT)
			{
				//-------------------------
				//----- RIGHT ALIGNED -----
				//-------------------------
				leading_pad_width = horizontal_padding;
				trailing_pad_width = horizontal_padding;
				if (x_end_coord)
				{
					leading_pad_width += (max_width - string_width);		//max_width is the requried width after padding
				}
				else
				{
					x_start_coord -= (string_width + leading_pad_width + trailing_pad_width);
				}
			}
			else if ((options & DISPLAY_TEXT_ALIGN_MASK) == DISPLAY_TEXT_ALIGN_CENTRE)
			{
				//--------------------------
				//----- CENTRE ALIGNED -----
				//--------------------------
				leading_pad_width = horizontal_padding;
				trailing_pad_width = horizontal_padding;
				
				w_temp = 0;
				if (x_end_coord)
					w_temp = (max_width - string_width);		//max_width is the requried width after padding

				if (w_temp & 0x01)
				{
					leading_pad_width += (w_temp >> 1);
					trailing_pad_width += (w_temp >> 1) + 1;
				}
				else
				{
					leading_pad_width += (w_temp >> 1);
					trailing_pad_width += (w_temp >> 1);
				}

				
				if (x_end_coord == 0)
				{
					x_start_coord -= (string_width + leading_pad_width + trailing_pad_width) >> 1;
				}
			}


			//----------------------------------------
			//----- DISPLAY TOP VERTICAL PADDING -----
			//----------------------------------------
			if ((doing_1st_line) && (vertical_padding_top))
			{
				if ((display_background_colour != DISPLAY_COLOUR_NULL) && (!doing_vertical_align_dummy_run))
				{
					display_write_block(x_start_coord, display_auto_y_coordinate,
										(x_start_coord + leading_pad_width + string_width + trailing_pad_width - 1), (display_auto_y_coordinate + vertical_padding_top - 1),
										display_background_colour);
				}
				
				display_auto_y_coordinate += vertical_padding_top;
			}
			doing_1st_line = 0;

		
		
			//SETUP FOR DISPLAY
			display_auto_x_coordinate = x_start_coord;

		
			//---------------------------------------------------
			//----- DISPLAY LEADING WHITESPACE IF NECESSARY -----
			//---------------------------------------------------
			if (leading_pad_width)
			{
				if ((display_background_colour != DISPLAY_COLOUR_NULL) && (!doing_vertical_align_dummy_run))
				{
					display_write_block(display_auto_x_coordinate, display_auto_y_coordinate,
										(display_auto_x_coordinate + leading_pad_width - 1), (display_auto_y_coordinate + display_bitmap_height - 1),
										display_background_colour);
				}
				display_auto_x_coordinate += leading_pad_width;
			}

			
			//------------------------------
			//----- DISPLAY THE STRING -----
			//------------------------------
			p_string = display_const_string_get_next_char(p_string, &next_character);
			while (next_character > 0x0000)			//Do until null terminator
			{
				//GET THE CHARACTER VALUES
				display_get_font_character(next_character);
				
				//DISPLAY CHARACTER
				if (!doing_vertical_align_dummy_run)
					display_character();
				else
					display_auto_x_coordinate += display_bitmap_width;

				//CHECK FOR END OF LINE
				if (p_string == (p_string_end_of_line + 1))
					break;
		
				p_string = display_const_string_get_next_char(p_string, &next_character);
				if (next_character != 0x00)
				{
					//DISPLAY THE GAP TO NEXT CHARACTER
					if (display_background_colour != DISPLAY_COLOUR_NULL)
					{
						display_write_block(display_auto_x_coordinate, display_auto_y_coordinate,
											(display_auto_x_coordinate + display_font_spacing - 1), (display_auto_y_coordinate + display_bitmap_height - 1),
											display_background_colour);
					}
		
					display_auto_x_coordinate += display_font_spacing;
				}
		
		
			} //while (next_character > 0x00)


			//----------------------------------------------------
			//----- DISPLAY TRAILING WHITESPACE IF NECESSARY -----
			//----------------------------------------------------
			if (trailing_pad_width)
			{
				if ((display_background_colour != DISPLAY_COLOUR_NULL) && (!doing_vertical_align_dummy_run))
				{
					display_write_block(display_auto_x_coordinate, display_auto_y_coordinate,
										(display_auto_x_coordinate + trailing_pad_width - 1), (display_auto_y_coordinate + display_bitmap_height - 1),
										display_background_colour);
				}
				display_auto_x_coordinate += trailing_pad_width;
			}
			
			//Move down to next line
			display_last_text_line_y_coordinate = display_auto_y_coordinate;			//Store last line we displayed on for user functions if they need it
			display_auto_y_coordinate += display_bitmap_height;	
			
			if (text_remaining)
			{
				//-----------------------------------------
				//----- THERE IS MORE TEXT TO DISPLAY -----
				//-----------------------------------------
				if (display_background_colour != DISPLAY_COLOUR_NULL)
				{
					display_write_block(x_start_coord, display_auto_y_coordinate,
										(x_start_coord + leading_pad_width + string_width + trailing_pad_width - 1), (display_auto_y_coordinate + display_font_inter_line_space - 1),
										display_background_colour);
				}
				display_auto_y_coordinate += display_font_inter_line_space;

				//Check for not out of space
				if (y_end_coord)
				{
					if ((display_auto_y_coordinate + display_bitmap_height + vertical_padding) > y_end_coord)
					{
						text_remaining = 0;					//No more space to display
						display_auto_y_coordinate -= display_font_inter_line_space;
					}
				}
			}


		} //while (text_remaining)
		
		if (doing_vertical_align_dummy_run)
		{
			//---------------------------------------------------
			//----- NOW DISPLAY FOR REAL VERTICALLY ALIGNED -----
			//---------------------------------------------------
			doing_vertical_align_dummy_run = 0;

			if (display_auto_y_coordinate < y_end_coord)
			{
				if ((options & DISPLAY_TEXT_VALIGN_MASK) == DISPLAY_TEXT_ALIGN_MIDDLE)
					vertical_padding_top += (y_end_coord - display_auto_y_coordinate) >> 1;
				else if ((options & DISPLAY_TEXT_VALIGN_MASK) == DISPLAY_TEXT_ALIGN_BOTTOM)
					vertical_padding_top += y_end_coord - display_auto_y_coordinate;
			}
			
			p_string_start_of_line = p_string = p_string_copy;
			display_auto_y_coordinate = y_start_coord;
			text_remaining = 1;
			doing_1st_line = 1;
			continue;
		}
		break;
	} //while (1)


	//-------------------------------------------
	//----- DISPLAY BOTTOM VERTICAL PADDING -----
	//-------------------------------------------
	if (vertical_padding)
	{
		//display_auto_y_coordinate += display_bitmap_height;
		if (display_background_colour != DISPLAY_COLOUR_NULL)
		{
			display_write_block(x_start_coord, display_auto_y_coordinate,
								(x_start_coord + leading_pad_width + string_width + trailing_pad_width - 1), (display_auto_y_coordinate + vertical_padding - 1),
								display_background_colour);
		}
		
		display_auto_y_coordinate += vertical_padding;
	}

	//--------------------------------------------
	//----- DISPLAY BOTTOM FILL IF NECESSARY -----
	//--------------------------------------------
	if (display_auto_y_coordinate <= y_end_coord)
	{
		if (display_background_colour != DISPLAY_COLOUR_NULL)
		{
			display_write_block(x_start_coord, display_auto_y_coordinate,
								(x_start_coord + leading_pad_width + string_width + trailing_pad_width - 1), y_end_coord,
								display_background_colour);
		}
		
		display_auto_y_coordinate = y_end_coord + 1;
	}

	return(p_string_start_of_line);
}
//------------------------------
//----- GET NEXT CHARACTER -----
//------------------------------
//Check for multi byte encoded characters
CONSTANT BYTE *display_const_string_get_next_char(CONSTANT BYTE *p_string, WORD (*next_character))
{
	BYTE b_temp;
	
	(*next_character) = (WORD)*p_string++;

	if (((*next_character) == '&') && (*p_string == '#'))
	{
		//UNICODE CHARACTER (using the format &#N; where N is either a decimal number or a hexadecimal number prefixed by x)
		p_string++;
		(*next_character) = 0;
		if ((*p_string == 'x') || (*p_string == 'X'))
		{
			//USING HEX NOTATION
			p_string++;
			while (*p_string != ';')
			{
				(*next_character) <<= 4;
				b_temp = (*p_string++) - '0';
				if (b_temp > 9)			//'A' to 'F'
					b_temp -= 7;
				if (b_temp > 15)		//'a' to 'f'
					b_temp -= 32;
					
				(*next_character) += b_temp;
			}
			p_string++;
		}
		else
		{
			//USING DECIMAL NOTATION
			while (*p_string != ';')
			{
				(*next_character) *= 10;
				(*next_character) += (*p_string++) - '0';
			}
			p_string++;
		}
	}
	else if ((*next_character) & 0x0080)
	{
		//UTF-8 ENCODED CHARACTER (Only relevant start encodings are 0x00c0 and 0x00e0 for 2 or 3 byte encoded values)
		if (((*next_character) & 0x00e0) == 0x00e0)
		{
			(*next_character) &= 0x000f;
			(*next_character) <<= 12;
			
			(*next_character) |= (WORD)(*p_string++ & 0x3f) << 6;
			(*next_character) |= (WORD)(*p_string++ & 0x3f);
		}
		else
		{
			(*next_character) &= 0x001f;
			(*next_character) <<= 6;
			
			(*next_character) |= (WORD)(*p_string++ & 0x3f);
		}
	}
	return(p_string);
}



//----------------------------------------
//----------------------------------------
//----- VERSION FOR VARIABLE STRINGS -----
//----------------------------------------
//----------------------------------------
//(An exact copy of the constant funcitons but with BYTE* instead of CONSTANT BYTE* for p_string) 
CONSTANT BYTE *display_variable_string (CONSTANT BYTE *p_font, WORD options,
								BYTE horizontal_padding, BYTE vertical_padding,
								WORD x_start_coord, WORD y_start_coord, WORD x_end_coord, WORD y_end_coord,
								BYTE *p_string)
{
	WORD next_character;
	BYTE *p_string_start_of_line;
	BYTE *p_string_end_of_line;
	BYTE *p_string_last_end_of_word;
	WORD string_width_at_last_end_of_word;
	WORD x_start_coord_copy;
	WORD w_temp;
	WORD string_width;
	BYTE text_remaining = 1;
	WORD max_width;
	WORD min_width;
	WORD leading_pad_width;
	WORD trailing_pad_width;
	BYTE doing_1st_line = 1;
	BYTE doing_vertical_align_dummy_run = 0;
	WORD vertical_padding_top = vertical_padding;			//Used for vertical align
	BYTE *p_string_copy;


	p_string_copy = p_string;
	p_string_start_of_line = p_string;
	x_start_coord_copy = x_start_coord;
	display_auto_y_coordinate = y_start_coord;



	//-------------------------
	//----- LOAD THE FONT -----
	//-------------------------
	display_load_font(p_font);

	//-------------------------------------------------
	//----- ENSURE START COORDIANTES ARE TOP LEFT -----
	//-------------------------------------------------
	if (x_end_coord)
	{
		if (x_start_coord > x_end_coord)
		{
			w_temp = x_start_coord;
			x_start_coord = x_end_coord;
			x_end_coord = w_temp;
		}
	}
	if (y_end_coord)
	{
		if (y_start_coord > y_end_coord)
		{
			w_temp = y_start_coord;
			y_start_coord = y_end_coord;
			y_end_coord = w_temp;
		}
	}

	//-------------------------------------
	//----- CALCULATE MIN & MAX WIDTH -----
	//-------------------------------------
	min_width = 0;
	if (x_end_coord)
	{
		max_width = min_width = (x_end_coord - x_start_coord) + 1;
	}
	else
	{
		if ((options & DISPLAY_TEXT_ALIGN_MASK) == DISPLAY_TEXT_ALIGN_LEFT)
		{
			max_width = DISPLAY_WIDTH_PIXELS - x_start_coord;
		}
		else if ((options & DISPLAY_TEXT_ALIGN_MASK) == DISPLAY_TEXT_ALIGN_RIGHT)
		{
			max_width = x_start_coord + 1;
		}
		else
		{
			if (x_start_coord > (DISPLAY_WIDTH_PIXELS >> 1))
				max_width = ((DISPLAY_WIDTH_PIXELS - x_start_coord) << 1);
			else
				max_width = (x_start_coord << 1);
		}
	}
	
	if (horizontal_padding)
		max_width -= (horizontal_padding << 1);
	
	if (max_width > DISPLAY_WIDTH_PIXELS)
		return(0);					//Error
	if (max_width == 0)
		return(0);					//Error


	//------------------------------------
	//----- CHECK FOR VERTICAL ALIGN -----
	//------------------------------------
	if (
		((options & DISPLAY_TEXT_VALIGN_MASK) == DISPLAY_TEXT_ALIGN_MIDDLE) ||
		((options & DISPLAY_TEXT_VALIGN_MASK) == DISPLAY_TEXT_ALIGN_BOTTOM)
	)
	{
		doing_vertical_align_dummy_run = 1;
	}
	
	while (1)
	{
		while (text_remaining)
		{
			//-----------------------------
			//-----------------------------
			//----- DISPLAY NEXT LINE -----
			//-----------------------------
			//-----------------------------
			
			x_start_coord = x_start_coord_copy;
			
			//-------------------------------------------------------
			//----- GET WIDTH OF STRING TO DISPLAY ON THIS LINE -----
			//-------------------------------------------------------
			string_width = 0;
			p_string = p_string_end_of_line = p_string_start_of_line;
			string_width_at_last_end_of_word = 0;
			
			while (1)
			{
				p_string = display_variable_string_get_next_char(p_string, &next_character);
				if (next_character == 0x00)			//Do until null terminator
					break;
				
				if (next_character == ' ')
				{
					//-------------------------
					//----- END OF A WORD -----
					//-------------------------
					p_string_last_end_of_word = p_string - 2;
					string_width_at_last_end_of_word = (string_width - display_font_spacing);
				}
				
				display_get_font_character(next_character);		//Loads display_bitmap_width
				string_width += display_bitmap_width;
				
				if (string_width > max_width)
				{
					//--------------------------------------------------------------
					//----- TOO LONG FOR WIDTH - MOVE BACK TO LAST END OF WORD -----
					//--------------------------------------------------------------
					if (string_width_at_last_end_of_word == 0)
						return(0);									//No end of word found - unable to display
					
					//Reset ready for display of string
					p_string = p_string_start_of_line;
					p_string_end_of_line = p_string_last_end_of_word;
					string_width = string_width_at_last_end_of_word;
					
					//Setup for display of next line
					p_string_start_of_line = (p_string_last_end_of_word + 2);		//Move past space character
					break;
				}
				
				string_width += display_font_spacing;			//Add gap to next character
			}
			
			if (p_string_end_of_line == p_string_start_of_line)
			{
				//----- END OF STRING WAS REACHED -----
				p_string_end_of_line = (p_string - 2);
				p_string = p_string_start_of_line;
				string_width -= display_font_spacing;			//Remove the final inter character gap that won't be displayed
				text_remaining = 0;
				p_string_start_of_line = 0;
			}
			

			
			//We now have:
			//	p_string				1st character to display
			//	p_string_end_of_line	Last character to display
			//	string_width			Total width of this lines characters
			//	p_string_start_of_line	1st character of next line to display
			
			
			
			if ((options & DISPLAY_TEXT_ALIGN_MASK) == DISPLAY_TEXT_ALIGN_LEFT)
			{
				//------------------------
				//----- LEFT ALIGNED -----
				//------------------------
				leading_pad_width = horizontal_padding;
				trailing_pad_width = horizontal_padding;
				if (x_end_coord)
				{
					trailing_pad_width += (max_width - string_width);		//max_width is the requried width after padding
				}
			}
			else if ((options & DISPLAY_TEXT_ALIGN_MASK) == DISPLAY_TEXT_ALIGN_RIGHT)
			{
				//-------------------------
				//----- RIGHT ALIGNED -----
				//-------------------------
				leading_pad_width = horizontal_padding;
				trailing_pad_width = horizontal_padding;
				if (x_end_coord)
				{
					leading_pad_width += (max_width - string_width);		//max_width is the requried width after padding
				}
				else
				{
					x_start_coord -= (string_width + leading_pad_width + trailing_pad_width);
				}
			}
			else if ((options & DISPLAY_TEXT_ALIGN_MASK) == DISPLAY_TEXT_ALIGN_CENTRE)
			{
				//--------------------------
				//----- CENTRE ALIGNED -----
				//--------------------------
				leading_pad_width = horizontal_padding;
				trailing_pad_width = horizontal_padding;
				
				w_temp = 0;
				if (x_end_coord)
					w_temp = (max_width - string_width);		//max_width is the requried width after padding

				if (w_temp & 0x01)
				{
					leading_pad_width += (w_temp >> 1);
					trailing_pad_width += (w_temp >> 1) + 1;
				}
				else
				{
					leading_pad_width += (w_temp >> 1);
					trailing_pad_width += (w_temp >> 1);
				}

				
				if (x_end_coord == 0)
				{
					x_start_coord -= (string_width + leading_pad_width + trailing_pad_width) >> 1;
				}
			}



			//----------------------------------------
			//----- DISPLAY TOP VERTICAL PADDING -----
			//----------------------------------------
			if ((doing_1st_line) && (vertical_padding_top))
			{
				if ((display_background_colour != DISPLAY_COLOUR_NULL) && (!doing_vertical_align_dummy_run))
				{
					display_write_block(x_start_coord, display_auto_y_coordinate,
										(x_start_coord + leading_pad_width + string_width + trailing_pad_width - 1), (display_auto_y_coordinate + vertical_padding_top - 1),
										display_background_colour);
				}
				
				display_auto_y_coordinate += vertical_padding_top;
			}
			doing_1st_line = 0;

		
		
			//SETUP FOR DISPLAY
			display_auto_x_coordinate = x_start_coord;

		
			//---------------------------------------------------
			//----- DISPLAY LEADING WHITESPACE IF NECESSARY -----
			//---------------------------------------------------
			if (leading_pad_width)
			{
				if ((display_background_colour != DISPLAY_COLOUR_NULL) && (!doing_vertical_align_dummy_run))
				{
					display_write_block(display_auto_x_coordinate, display_auto_y_coordinate,
										(display_auto_x_coordinate + leading_pad_width - 1), (display_auto_y_coordinate + display_bitmap_height - 1),
										display_background_colour);
				}
				display_auto_x_coordinate += leading_pad_width;
			}

			
			//------------------------------
			//----- DISPLAY THE STRING -----
			//------------------------------
			p_string = display_variable_string_get_next_char(p_string, &next_character);
			while (next_character > 0x0000)			//Do until null terminator
			{
		
				//GET THE CHARACTER VALUES
				display_get_font_character(next_character);
				
				//DISPLAY CHARACTER
				if (!doing_vertical_align_dummy_run)
					display_character();
				else
					display_auto_x_coordinate += display_bitmap_width;

				//CHECK FOR END OF LINE
				if (p_string == (p_string_end_of_line + 1))
					break;
		
				p_string = display_variable_string_get_next_char(p_string, &next_character);			
				if (next_character != 0x00)
				{
					//DISPLAY THE GAP TO NEXT CHARACTER
					if (display_background_colour != DISPLAY_COLOUR_NULL)
					{
						display_write_block(display_auto_x_coordinate, display_auto_y_coordinate,
											(display_auto_x_coordinate + display_font_spacing - 1), (display_auto_y_coordinate + display_bitmap_height - 1),
											display_background_colour);
					}
		
					display_auto_x_coordinate += display_font_spacing;
				}
		
		
			} //while (next_character > 0x00)


			//----------------------------------------------------
			//----- DISPLAY TRAILING WHITESPACE IF NECESSARY -----
			//----------------------------------------------------
			if (trailing_pad_width)
			{
				if ((display_background_colour != DISPLAY_COLOUR_NULL) && (!doing_vertical_align_dummy_run))
				{
					display_write_block(display_auto_x_coordinate, display_auto_y_coordinate,
										(display_auto_x_coordinate + trailing_pad_width - 1), (display_auto_y_coordinate + display_bitmap_height - 1),
										display_background_colour);
				}
				display_auto_x_coordinate += trailing_pad_width;
			}

			//Move down to next line
			display_last_text_line_y_coordinate = display_auto_y_coordinate;			//Store last line we displayed on for user functions if they need it
			display_auto_y_coordinate += display_bitmap_height;		
			
			if (text_remaining)
			{
				//-----------------------------------------
				//----- THERE IS MORE TEXT TO DISPLAY -----
				//-----------------------------------------
				if (display_background_colour != DISPLAY_COLOUR_NULL)
				{
					display_write_block(x_start_coord, display_auto_y_coordinate,
										(x_start_coord + leading_pad_width + string_width + trailing_pad_width - 1), (display_auto_y_coordinate + display_font_inter_line_space - 1),
										display_background_colour);
				}
				display_auto_y_coordinate += display_font_inter_line_space;

				//Check for not out of space
				if (y_end_coord)
				{
					if ((display_auto_y_coordinate + display_bitmap_height +  vertical_padding) > y_end_coord)
					{
						text_remaining = 0;					//No more space to display
						display_auto_y_coordinate -= display_font_inter_line_space;
					}
				}
			}
		
		} //while (text_remaining)
		
		if (doing_vertical_align_dummy_run)
		{
			//---------------------------------------------------
			//----- NOW DISPLAY FOR REAL VERTICALLY ALIGNED -----
			//---------------------------------------------------
			doing_vertical_align_dummy_run = 0;

			if (display_auto_y_coordinate < y_end_coord)
			{
				if ((options & DISPLAY_TEXT_VALIGN_MASK) == DISPLAY_TEXT_ALIGN_MIDDLE)
					vertical_padding_top += (y_end_coord - display_auto_y_coordinate) >> 1;
				else if ((options & DISPLAY_TEXT_VALIGN_MASK) == DISPLAY_TEXT_ALIGN_BOTTOM)
					vertical_padding_top += y_end_coord - display_auto_y_coordinate;
			}
			
			p_string_start_of_line = p_string = p_string_copy;
			display_auto_y_coordinate = y_start_coord;
			text_remaining = 1;
			doing_1st_line = 1;
			continue;
		}
		break;
	} //while (1)


	//-------------------------------------------
	//----- DISPLAY BOTTOM VERTICAL PADDING -----
	//-------------------------------------------
	if (vertical_padding)
	{
		if (display_background_colour != DISPLAY_COLOUR_NULL)
		{
			display_write_block(x_start_coord, display_auto_y_coordinate,
								(x_start_coord + leading_pad_width + string_width + trailing_pad_width - 1), (display_auto_y_coordinate + vertical_padding - 1),
								display_background_colour);
		}
		display_auto_y_coordinate += vertical_padding;
	}

	//--------------------------------------------
	//----- DISPLAY BOTTOM FILL IF NECESSARY -----
	//--------------------------------------------
	if (display_auto_y_coordinate <= y_end_coord)
	{
		if (display_background_colour != DISPLAY_COLOUR_NULL)
		{
			display_write_block(x_start_coord, display_auto_y_coordinate,
								(x_start_coord + leading_pad_width + string_width + trailing_pad_width - 1), y_end_coord,
								display_background_colour);
		}
		display_auto_y_coordinate = y_end_coord + 1;
	}

	return(p_string_start_of_line);
}
//------------------------------
//----- GET NEXT CHARACTER -----
//------------------------------
//Check for multi byte encoded characters
BYTE *display_variable_string_get_next_char(BYTE *p_string, WORD *next_character)
{
	BYTE b_temp;
	
	(*next_character) = (WORD)*p_string++;

	if (((*next_character) == '&') && (*p_string == '#'))
	{
		//UNICODE CHARACTER (using the format &#N; where N is either a decimal number or a hexadecimal number prefixed by x)
		p_string++;
		(*next_character) = 0;
		if ((*p_string == 'x') || (*p_string == 'X'))
		{
			//USING HEX NOTATION
			p_string++;
			while (*p_string != ';')
			{
				(*next_character) <<= 4;
				b_temp = (*p_string++) - '0';
				if (b_temp > 9)			//'A' to 'F'
					b_temp -= 7;
				if (b_temp > 15)		//'a' to 'f'
					b_temp -= 32;
					
				(*next_character) += b_temp;
			}
			p_string++;
		}
		else
		{
			//USING DECIMAL NOTATION
			while (*p_string != ';')
			{
				(*next_character) *= 10;
				(*next_character) += (*p_string++) - '0';
			}
			p_string++;
		}
	}
	else if ((*next_character) & 0x0080)
	{
		//UTF-8 ENCODED CHARACTER (Only relevant start encodings are 0x00c0 and 0x00e0 for 2 or 3 byte encoded values)
		if (((*next_character) & 0x00e0) == 0x00e0)
		{
			(*next_character) &= 0x000f;
			(*next_character) <<= 12;
			
			(*next_character) |= (WORD)(*p_string++ & 0x3f) << 6;
			(*next_character) |= (WORD)(*p_string++ & 0x3f);
		}
		else
		{
			(*next_character) &= 0x001f;
			(*next_character) <<= 6;
			
			(*next_character) |= (WORD)(*p_string++ & 0x3f);
		}
	}
	return(p_string);
}





//*******************************
//*******************************
//********** LOAD FONT **********
//*******************************
//*******************************
//(Note that the BitFontCreatorPro file bfctype.h contains the definition of the BitFontCreatorPro binary file structure refered to in this function)
void display_load_font (CONSTANT BYTE *p_font)
{
	BYTE data;
	

	display_font_start_address = 0;
	if (p_font)
	{	
	//----- READ THE HEADER FILE DEFINITION -----
	
		//FILE NAME [#]
		//Move past the file name
		data = 1;
		while (data != 0)			//Look for null termination
			data = *p_font++;
		
		//FILE SIZE [4]
		//Move past the file size
		p_font += 4;
			
		//FILE START ADDRESS
		#ifdef DISPLAY_FILES_ARE_EXTERNAL			//This define gets set by the Display Files Conveter application for binary file outputs
			display_font_start_address = ((DISPLAY_PROG_MEM_SIZE)*p_font++) << 24;
			display_font_start_address |= ((DISPLAY_PROG_MEM_SIZE)*p_font++) << 16;
			display_font_start_address |= ((DISPLAY_PROG_MEM_SIZE)*p_font++) << 8;
			display_font_start_address |= (DISPLAY_PROG_MEM_SIZE)*p_font++;
		#else
			display_font_start_address = (DISPLAY_PROG_MEM_SIZE)p_font;
		#endif
	}	
	
	//----- GET VALUES FROM FONT FILE HEADER -----
	//Note that binary file encoding is lowest byte of a value to highest byte
	
	DISPLAY_FONT_SET_READ_ADDRESS(display_font_start_address);

	//MAGIC NUMBER [2]
	//Ignore
	data = DISPLAY_FONT_READ_NEXT_BYTE;
	data = DISPLAY_FONT_READ_NEXT_BYTE;

	//VERSION [2]
	//Ignore
	data = DISPLAY_FONT_READ_NEXT_BYTE;
	data = DISPLAY_FONT_READ_NEXT_BYTE;
	
	//FLAGS [2]
	display_font_creation_flags = DISPLAY_FONT_READ_NEXT_BYTE;
	display_font_creation_flags |= (WORD)DISPLAY_FONT_READ_NEXT_BYTE << 8;

	//WIDTH [2]
	//0 for variable-width fonts
	display_bitmap_width = DISPLAY_FONT_READ_NEXT_BYTE;
	display_bitmap_width |= (WORD)DISPLAY_FONT_READ_NEXT_BYTE << 8;

	//HEIGHT [2]
	display_bitmap_height = DISPLAY_FONT_READ_NEXT_BYTE;
	display_bitmap_height |= (WORD)DISPLAY_FONT_READ_NEXT_BYTE << 8;
	
	//ASCENT [2]
	//Ignore
	data = DISPLAY_FONT_READ_NEXT_BYTE;
	data = DISPLAY_FONT_READ_NEXT_BYTE;
	
	//FIRST CHARACTER [2]
	display_font_first_character = DISPLAY_FONT_READ_NEXT_BYTE;
	display_font_first_character |= (WORD)DISPLAY_FONT_READ_NEXT_BYTE << 8;
	
	//LAST CHARACTER [2]
	display_font_last_character = DISPLAY_FONT_READ_NEXT_BYTE;
	display_font_last_character |= (WORD)DISPLAY_FONT_READ_NEXT_BYTE << 8;

	//CHARACTER IN FONT [2]
	//Ignore
	data = DISPLAY_FONT_READ_NEXT_BYTE;
	data = DISPLAY_FONT_READ_NEXT_BYTE;
	
	//RESERVED [1]
	//Ignore
	data = DISPLAY_FONT_READ_NEXT_BYTE;
	
	//BYTES PER UNIT [1]
	//Ignore
	data = DISPLAY_FONT_READ_NEXT_BYTE;
	
	//DATA UNITS [4]
	//Ignore
	data = DISPLAY_FONT_READ_NEXT_BYTE;
	data = DISPLAY_FONT_READ_NEXT_BYTE;
	data = DISPLAY_FONT_READ_NEXT_BYTE;
	data = DISPLAY_FONT_READ_NEXT_BYTE;
	
	//DATA TABLE POINTER [4]
	display_font_data_table_address = DISPLAY_FONT_READ_NEXT_BYTE;
	display_font_data_table_address |= (DWORD)DISPLAY_FONT_READ_NEXT_BYTE << 8;
	display_font_data_table_address |= (DWORD)DISPLAY_FONT_READ_NEXT_BYTE << 16;
	display_font_data_table_address |= (DWORD)DISPLAY_FONT_READ_NEXT_BYTE << 24;
	
	//OFFSET TABLE POINTER [4]
	display_font_offset_table_address = DISPLAY_FONT_READ_NEXT_BYTE;
	display_font_offset_table_address |= (DWORD)DISPLAY_FONT_READ_NEXT_BYTE << 8;
	display_font_offset_table_address |= (DWORD)DISPLAY_FONT_READ_NEXT_BYTE << 16;
	display_font_offset_table_address |= (DWORD)DISPLAY_FONT_READ_NEXT_BYTE << 24;
	
	//WIDTH TABLE POINTER [4]
	display_font_width_table_address = DISPLAY_FONT_READ_NEXT_BYTE;
	display_font_width_table_address |= (DWORD)DISPLAY_FONT_READ_NEXT_BYTE << 8;
	display_font_width_table_address |= (DWORD)DISPLAY_FONT_READ_NEXT_BYTE << 16;
	display_font_width_table_address |= (DWORD)DISPLAY_FONT_READ_NEXT_BYTE << 24;

	//ASCII TABLE POINTER [4]
	//Ignore
	data = DISPLAY_FONT_READ_NEXT_BYTE;
	data = DISPLAY_FONT_READ_NEXT_BYTE;
	data = DISPLAY_FONT_READ_NEXT_BYTE;
	data = DISPLAY_FONT_READ_NEXT_BYTE;
	
	//EXTENDED INDEX TABLE POINTER [4]
	display_font_extended_index_table_address = DISPLAY_FONT_READ_NEXT_BYTE;
	display_font_extended_index_table_address |= (DWORD)DISPLAY_FONT_READ_NEXT_BYTE << 8;
	display_font_extended_index_table_address |= (DWORD)DISPLAY_FONT_READ_NEXT_BYTE << 16;
	display_font_extended_index_table_address |= (DWORD)DISPLAY_FONT_READ_NEXT_BYTE << 24;


	
	//----- SET INTER CHARACXTER GAP AND INTER LINE GAP -----
	//Adjust this if desired with your own prefered values
	if (display_bitmap_height >= 30)
	{
		display_font_spacing = 4;
		display_font_inter_line_space = 2;
	}
	else if (display_bitmap_height >= 20)
	{
		display_font_spacing = 3;
		display_font_inter_line_space = 2;
	}
	else if (display_bitmap_height >= 14)
	{
		display_font_spacing = 2;
		display_font_inter_line_space = 2;
	}
	else
	{
		display_font_spacing = 1;
		display_font_inter_line_space = 1;
	}
}




//****************************************
//****************************************
//********** GET FONT CHARACTER **********
//****************************************
//****************************************
//Call display_load_font() first.
//character
//	The unicode character code
//(Note that the BitFontCreatorPro file bfctype.h contains the definition of the BitFontCreatorPro binary file structure refered to in this function)
void display_get_font_character (WORD character)
{
	WORD index;
	WORD ranges_count;
	WORD count;
	WORD start_character;
	WORD characters_count;
	DWORD offset;

		if((character < display_font_first_character) || (character > display_font_last_character))
	{
		index = 0;						 		//Character not found - use the first character index
		goto display_get_font_character1;
	}

	if (display_font_extended_index_table_address == 0)
	{
		index = 0;						 		//Error - use the first character index
		goto display_get_font_character1;
	}

	index = 0;


	//--------------------------------------------------------------------------
	//----- SEARCH FONTS EXTENDED CHARACTER INDEX TABLE FOR THIS CHARACTER -----
	//--------------------------------------------------------------------------

	//Read header (GLYPH_SET)
	DISPLAY_FONT_SET_READ_ADDRESS(display_font_start_address + display_font_extended_index_table_address + 6);
	//cbThis [4]
	//	Ignore
	//nChars [2]
	//	Ignore


	//nRanges [2]
	ranges_count = DISPLAY_FONT_READ_NEXT_BYTE;
	ranges_count |= (WORD)DISPLAY_FONT_READ_NEXT_BYTE << 8;

	//*pRanges [4] (pointer to the first range)
	offset = DISPLAY_FONT_READ_NEXT_BYTE;
	offset |= (DWORD)DISPLAY_FONT_READ_NEXT_BYTE << 8;
	offset |= (DWORD)DISPLAY_FONT_READ_NEXT_BYTE << 16;
	offset |= (DWORD)DISPLAY_FONT_READ_NEXT_BYTE << 24;
	DISPLAY_FONT_SET_READ_ADDRESS(display_font_start_address + offset);

	//READ EACH CHARACTER RANGES (GLYPH_RANGE array)
	for (count = 0; count < ranges_count; count++)
	{
		//IS CHARACTER IN THIS RANGE?
		
		//charLow [2] (first character code in this range)
		start_character = DISPLAY_FONT_READ_NEXT_BYTE;
		start_character |= (WORD)DISPLAY_FONT_READ_NEXT_BYTE << 8;
	
		//nGlyphs [2] (# characters in this range)
		characters_count = DISPLAY_FONT_READ_NEXT_BYTE;
		characters_count |= (WORD)DISPLAY_FONT_READ_NEXT_BYTE << 8;
		
		if(character < (start_character + characters_count))
		{
			//Character is in this range - return the index
			index += (character - start_character); 
			goto display_get_font_character1;
		}
		else
		{
			index += characters_count;
		}
	}
	index = 0;		   //Not found - use the first character index

display_get_font_character1:

	//-----------------------------------
	//----- GET THE CHARACTER WIDTH -----
	//-----------------------------------
	if ((display_font_creation_flags & 0x0002) && (display_bitmap_width > 0))
	{
		//Monospaced font - width has already been set from font header
		//display_bitmap_width = display_bitmap_width;
	}
	else
	{
		DISPLAY_FONT_SET_READ_ADDRESS(display_font_start_address + display_font_width_table_address + (index << 1));
		display_bitmap_width = DISPLAY_FONT_READ_NEXT_BYTE;
		display_bitmap_width |= (WORD)DISPLAY_FONT_READ_NEXT_BYTE << 8;
	}



	//-------------------------------------------------
	//----- SET THE CHARACTER DATA START ADDRESSS -----
	//-------------------------------------------------
	DISPLAY_FONT_SET_READ_ADDRESS(display_font_start_address + display_font_offset_table_address + (index << 2));
	offset = DISPLAY_FONT_READ_NEXT_BYTE;
	offset |= (DWORD)DISPLAY_FONT_READ_NEXT_BYTE << 8;
	offset |= (DWORD)DISPLAY_FONT_READ_NEXT_BYTE << 16;
	offset |= (DWORD)DISPLAY_FONT_READ_NEXT_BYTE << 24;
	
	DISPLAY_FONT_SET_READ_ADDRESS(display_font_start_address + display_font_data_table_address + offset);
	
	//The next use of DISPLAY_FONT_READ_NEXT_BYTE will read the first byte of the character data
}




//***************************************
//***************************************
//********** DISPLAY CHARACTER **********
//***************************************
//***************************************
//Displays a single character.
//	display_foreground_colour 	Set to the requried text colour
//	display_background_colour	Set to the requried background colour or set to 0xffffffff for no background colour
//	display_auto_x_coordinate	Set to left start coordinate (automatically updated by this function to next pixel after the right most pixel of the displayed character)
//	display_auto_y_coordinate	Set to the bottom start coordinate
//	p_image_memory_address

void display_character (void)
{
	WORD x_coord;
	WORD y_coord;
	WORD row_count;
	WORD column_count;
	BYTE bit_pointer;
	BYTE data;
	
		
	data = DISPLAY_FONT_READ_NEXT_BYTE;

	//Font's are stored starting with the top left pixel
	y_coord = display_auto_y_coordinate;


	//The Bit Font Creator Pro output file is ordered as follows:
	//	Byte0, bit 7 = top left pixel of the character.
	//	You simply work through from bit 7 to bit 0 of each byte and then move onto the next, moving down a row after display_bitmap_width number of bits
	//	and until you have done display_bitmap_height number of rows.
	
	DISPLAY_CS(0);
	bit_pointer = 0x80;
	for (row_count = 0; row_count < display_bitmap_height; row_count++)
	{
		//----- DISPLAY NEXT ROW -----
		x_coord = display_auto_x_coordinate;
		for (column_count = 0; column_count < display_bitmap_width; column_count++)
		{
			//----- DISPLAY NEXT COLUMN -----
			if (data & bit_pointer)
			{
				//PIXEL ON (IS PART OF CHARACTER)
				display_write_pixel(x_coord, y_coord, display_foreground_colour);
			}
			else
			{
				//PIXEL OFF (IS PART OF BACKGROUND)
				if (display_background_colour != DISPLAY_COLOUR_NULL)
					display_write_pixel(x_coord, y_coord, display_background_colour);
			}
			
			//Move to next bit (pixel)
			x_coord++;
			
			bit_pointer >>= 1;
			if (bit_pointer == 0)
			{
				bit_pointer = 0x80;
				data = DISPLAY_FONT_READ_NEXT_BYTE;
			}
		} //for (column_count = 0; column_count < display_bitmap_width; column_count++)
		
		y_coord++;
		
	} //for (row_count = 0; row_count < display_bitmap_height; row_count++)
	DISPLAY_CS(1);


	//----- FINISHED DISPLAYING THE CHARACTER -----
	display_auto_x_coordinate = x_coord;
}









//*******************************
//*******************************
//********** FIND FILE **********
//*******************************
//*******************************
//Use this function to search for a bitmap file that has been converted using the Display Files Converter application.  This function is a method to search for files who's name is
//not know at design time (i.e. from a variable string filename, for instance in a HTML file).  Files may have been stored using the C Header File or Binary File option of the
//Display Files Converter application and this function will find them by using the Display Files Converter outputted header file.
//search_for_filename	The available files will be searched to see if any match this filename.
//Returns				1 if the file was found and DISPLAY_FILE_SET_READ_ADDRESS was used to setup its address (i.e. p_bitmap can be set to 0 when calling display_bitmap() ),
//						0 if not
BYTE display_find_file (BYTE *search_for_filename)
{
#ifdef DISPLAY_BITMAP_SET_READ_ADDRESS

	BYTE file_count;
	BYTE b_count;
	BYTE filename_matches;
	BYTE next_character;
	CONSTANT BYTE *p_file;
	BYTE data;
	DISPLAY_PROG_MEM_SIZE file_index;


	//-------------------------------------------------------------------
	//----- WE NEED TO SEARCH OUR AVAILABLE FILE NAMES FOR THE FILE -----
	//-------------------------------------------------------------------
	file_count = 0xff;
	filename_matches = 0;
	while (1)
	{
		file_count++;
		if (display_filenames[file_count] == 0)			//End of filenames?
			break;
				
		//----- DOES REQUESTED FILE MATCH THIS FILE? -----
		p_file = display_filenames[file_count];		//Set to first character of this next file
		b_count = 0;								//Reset back to first character of requested file
		filename_matches = 1;						//Default to this filename matches
		
		//CHECK THE FILENAME
		while (*p_file != 0x00)
		{
			//Get next character of the requested filename
			next_character = search_for_filename[b_count++];

			//Convert all characters to uppercase
			if ((next_character >= 'a') && (next_character <= 'z'))
				next_character -= 0x20;

			//The '/' character is converted to "_" in our internal filenames to make them C compiler friendly
			if (next_character == '/')
				next_character = '_';

			//The '.' character is converted to "_" in our internal filenames to make them C compiler friendly
			if (next_character == '.')
				next_character = '_';

			//Exit if we're reached the end of the request filename
			if (next_character == 0x00)
			{
				b_count--;
				break;
			}
			
			data = *p_file++;
			if ((data >= 'a') && (data <= 'z'))						//Convert all characters to uppercase
				data -= 0x20;
				
			if (next_character != data)								//Does it match this current file?
			{
				//Doesn't match - move onto next file
				filename_matches = 0;
				break;
			}	
		}
		if ((filename_matches) && (search_for_filename[b_count] == 0x00))			//Check we matched all the way to the end of the requested filename
		{
			//----------------------------
			//----- THIS IS THE FILE -----
			//----------------------------
			break;
		}		
	}
	
	if (filename_matches == 0)
	{
		//----- UNABLE TO FIND THE REQUESTED FILE -----
		return(0);
	}


	p_file = display_filenames[file_count];		//Set to first character of this next file
	
	//FILE NAME [#]
	//Move past the file name
	data = 1;
	while (data != 0)			//Look for null termination
		data = *p_file++;
	
	//FILE SIZE [4]
	//Move past the file size
	p_file += 4;
		
	//FILE START ADDRESS
	#ifdef DISPLAY_FILES_ARE_EXTERNAL			//This define gets set by the Display Files Conveter application for binary file outputs
		file_index = ((DISPLAY_PROG_MEM_SIZE)*p_file++) << 24;
		file_index |= ((DISPLAY_PROG_MEM_SIZE)*p_file++) << 16;
		file_index |= ((DISPLAY_PROG_MEM_SIZE)*p_file++) << 8;
		file_index |= (DISPLAY_PROG_MEM_SIZE)*p_file++;
	#else
		file_index = (DISPLAY_PROG_MEM_SIZE)p_file;
	#endif
	
	DISPLAY_BITMAP_SET_READ_ADDRESS(file_index);

	return(1);
#else
	return(0);
#endif
}


