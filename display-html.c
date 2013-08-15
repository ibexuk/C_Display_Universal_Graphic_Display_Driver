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
//HTML DRIVER C CODE FILE



//----- INCLUDE FILES FOR THIS SOURCE CODE FILE -----
#include "main.h"					//Global data type definitions (see https://github.com/ibexuk/C_Generic_Header_File )
#define	DISPLAY_HTML_C		//(Our header file define)
#include "display.h"				//(Include header file for this source file)
#include "display-model.h"			//Contains defines and functions specific to the screen module being used - if changing display
									//screen change the "display-model.h" file that is included in the project
#include "display-html.h"			//(After the display and display-model includes)

//----- OTHER PROJECT FILES REQUIRED BY THIS SOURCE CODE FILE -----
#include "ap-main.h"

#ifdef HTML_INCLUDE_OPTION_SELECT				//>>>>>>>>>>>> REMOVE THIS SECTION! #####################################
	#include "option-select.h"
#endif											//<<<<<<<<<<<< REMOVE THIS SECTION! #####################################

//----- COMPILER LIBRARY FILES REQUIRED BY THIS SOURCE CODE FILE -----






//*********************************************
//*********************************************
//********** HTML SETUP TO READ FILE **********
//*********************************************
//*********************************************
//Use this function to setup ready to read for files that have been converted using the Display Files Converter application.  Files may have been stored using the C
//Header File or Binary File option.
//p_file_pointer		Load with the pointer to the file or set to 0 for function to search for file name using search_for_filename
//search_for_filename	If p_file_pointer == 0 then the available files will be searched to see if any match this filename.  Set to 0 if not required.
//Returns				1 if file found and ready to read, 0 if not
BYTE display_html_setup_read_file (CONSTANT BYTE *p_file_pointer, BYTE *search_for_filename, DWORD *file_size)
{
	BYTE file_count;
	BYTE b_count;
	BYTE filename_matches;
	BYTE next_character;
	CONSTANT BYTE *p_file;
	BYTE data;
	DWORD file_index;


	if (p_file_pointer)
	{
		//-------------------------------------------
		//----- WE HAVE THE POINTER TO THE FILE -----
		//-------------------------------------------
		p_file = p_file_pointer;

		//FILE NAME [#]
		//Move past the file name
		data = 1;
		while (data != 0)			//Look for null termination
			data = *p_file++;
	}
	else
	{
		//-------------------------------------------------------------------
		//----- WE NEED TO SEARCH OUR AVAILABLE FILE NAMES FOR THE FILE -----
		//-------------------------------------------------------------------
		if (search_for_filename == 0)
			return(0);


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
				p_file++;
				break;
			}		
		}
		
		if (filename_matches == 0)
		{
			//----- UNABLE TO FIND THE REQUESTED FILE -----
			return(0);
		}

	}
	
	//FILE SIZE [4]
	*file_size = ((DWORD)*p_file++) << 24;
	*file_size |= ((DWORD)*p_file++) << 16;
	*file_size |= ((DWORD)*p_file++) << 8;
	*file_size |= (DWORD)*p_file++;
			
	//FILE START ADDRESS
	#ifdef DISPLAY_FILES_ARE_EXTERNAL			//This define gets set by the Display Files Conveter application for binary file outputs
		file_index = ((DWORD)*p_file++) << 24;
		file_index |= ((DWORD)*p_file++) << 16;
		file_index |= ((DWORD)*p_file++) << 8;
		file_index |= (DWORD)*p_file++;
	#else
		file_index = (DWORD)p_file;
	#endif
		
	DISPLAY_HTML_SET_READ_ADDRESS(file_index);
	
	
	return(1);
}




//***************************************
//***************************************
//********** DISPLAY HTML FILE **********
//***************************************
//***************************************
//Call this function after calling display_html_setup_read_file() or after your application has setup accessing your html file ready for the DISPLAY_HTML_READ_NEXT_BYTE define to be used
void display_html_file (DWORD file_size)
{
	BYTE count;
	BYTE count1;
	BYTE data;
	BYTE character;
	BYTE b_temp;
	WORD w_temp;
	DWORD dw_temp;
	DWORD dw_temp1;
	BYTE previous_characters[10];
	BYTE reading_opening_tag = 0;
	BYTE reading_closing_tag = 0;
	BYTE text_buffer[MAX_TEXT_BUFFER_LENGTH];
	BYTE *p_text_buffer = &text_buffer[0];
	BYTE reading_non_display_text = 0;
	BYTE this_doc_styles_count = 0;
	CONSTANT BYTE *p_font_to_use;
	DISPLAY_HTML_TAGS_STACK tags_stack[MAX_TAGS_NESTING_LEVELS];	
	BYTE tags_stack_current_depth = 0;
	BYTE inside_tag = NOT_INSIDE_TAG;
	BYTE link_name_or_value[13];
	BYTE *p_link_name_or_value;
	BYTE we_are_in_page_body = 0;
	BYTE inside_tag_paramater_state = PARAMETER_READING_NAME;
	BYTE style_attribute_name[MAX_STYLE_ATTRIBUTE_NAME_LENGTH];
	BYTE *p_style_attribute_name;
	BYTE style_attribute_value[MAX_STYLE_ATTRIBUTE_VALUE_LENGTH];
	BYTE *p_style_attribute_value;
	DWORD transparency_colour;
	BYTE space_character_before_this_character = 0;
	BYTE character_pre_upper_case_change;
	BYTE dynamic_text_index;
	BYTE dynamic_text_character_index = 0xff;
	
	#ifdef HTML_PASS_ANCHOR_LINK_FUNCTION				//Don't do if we're not storing links
		BYTE anchor_link_value[13];
		BYTE *p_anchor_link_value;
	#endif

#ifdef HTML_INCLUDE_OPTION_SELECT				//>>>>>>>>>>>> REMOVE THIS SECTION! #####################################
	static BYTE option_select_active = 0;
	BYTE option_select_doing_index;
	BYTE option_select_no_of_display_lines;
	BYTE option_select_this_option_is_default;
		
	//Reset option select if it was previous in use
	if (option_select_active)
	{
		option_select_active = 0;
		option_select_end();
	}
#endif											//<<<<<<<<<<<< REMOVE THIS SECTION! #####################################



	//Set start of section style values
	current_style_background_colour = current_style_background_colour_copy = DISPLAY_COLOUR_NULL;
	current_style_border_colour = DISPLAY_COLOUR_NULL;
	tags_stack[0].top = 0;
	tags_stack[0].left = 0;
	tags_stack[0].height = 0;
	tags_stack[0].width = DISPLAY_WIDTH_PIXELS;
	tags_stack[0].border_width = 0;
	tags_stack[0].align = 0;
	tags_stack[0].font_size = 0;
	tags_stack[0].fore_colour = DISPLAY_COLOUR_BLACK;
	tags_stack[0].padding_hoz = 0;
	tags_stack[0].padding_vert = 0;


	previous_characters[2] = 0x00;
	previous_characters[1] = 0x00;
	previous_characters[0] = 0x00;
	character = 0x00;
	
	p_link_name_or_value = &link_name_or_value[0];
	#ifdef HTML_PASS_ANCHOR_LINK_FUNCTION				//Don't do if we're not storing links
		p_anchor_link_value = &anchor_link_value[0];
		anchor_link_value[0] = 0x00;
	#endif


	//Read each byte
	while (file_size--)
	{
		//-------------------------
		//----- GET NEXT BYTE -----
		//-------------------------
		if (dynamic_text_character_index < 0xff)
		{
			//GET NEXT BYTE OF DYNAMIC TEXT
			file_size++;
			
			#ifdef HTML_GET_DYNAMIC_TEXT_FUNCTION
				data = HTML_GET_DYNAMIC_TEXT_FUNCTION(dynamic_text_index, dynamic_text_character_index++);
			#else
				dynamic_text_character_index = 0xff;
				continue;
			#endif
			
			if (data == 0x00)
			{
				//End of string
				dynamic_text_character_index = 0xff;
				continue;
			}
		}
		else
		{
			//GET NEXT BYTE AS NORMAL
			data = DISPLAY_HTML_READ_NEXT_BYTE;
		}

		//--------------------------------
		//----- DEAL WITH WHITESPACE -----
		//--------------------------------
		
		//Convert TAB to SPACE
		if (data == '\t')
			data = ' ';

		//Ignore non displayable characters (e.g. line breaks, etc)
		if (data < 0x20)
			continue;

		//Discard multiple space characters
		if ((data == ' ') && (character == ' '))
			continue;

		//If we are inside a tag convert all characters to uppercase and ignore spaces
		character_pre_upper_case_change = data;
		if (inside_tag != NOT_INSIDE_TAG)
		{
			if (data == ' ')
			{
				space_character_before_this_character = 2;					//Flag if there was a space as some inside tag functions need to re-add it
				continue;
			}
			
			if ((data >= 'a') && (data <= 'z'))
				data -= 0x20;
		}


		//-----------------------------------
		//----- ADD CHARACTER TO BUFFER -----
		//-----------------------------------
		count = 9;
		do
		{
			previous_characters[count] = previous_characters[(count - 1)];
		} while (--count);
		previous_characters[0] = character;
		character = data;

		if (space_character_before_this_character)
			space_character_before_this_character--;
		
		if (character == '<')
		{
			//------------------------
			//----- START OF TAG -----
			//------------------------
			inside_tag = INSIDE_TAG_LOOKING_FOR_TYPE;
			
			inside_tag_paramater_state = PARAMETER_READING_NAME;
			continue;
		}


		if (inside_tag != NOT_INSIDE_TAG)
		{
			//-------------------------------
			//-------------------------------
			//----- WE ARE INSIDE A TAG -----
			//-------------------------------
			//-------------------------------
			//How we deal with tags:
			//We look for tag opening and closing character "<" and ">" and flag when we are inside one
			//If the tag is a tag we process then we specifically detect that individual tag starting and that individual tag ending and handle accordingly, pushing
			//and popping the style stage as needed and displaying anything that needs to be displayed.  All other tags are simply ignored so apart from detecting when
			//we are inside a tag and ignoring text characters there is nothing else we need to do.
			
			//ALL CHARACTERS WILL BE UPPERCASE (we have already converted)


			//---------------------------------
			//---------------------------------
			//----- LOOK FOR OPENING TAGS -----
			//---------------------------------
			//---------------------------------

			if ((previous_characters[1] == '<') && (previous_characters[0] == 'H') && (character >= '1') && (character <= '6'))
			{
				//---------------------------------
				//----- THIS IS A HEADING TAG -----
				//---------------------------------
				inside_tag = INSIDE_TAG_IS_HEADING_TAG;
				
				display_html_push_stack_for_new_tag(&tags_stack[0], &tags_stack_current_depth, &text_buffer[0], p_text_buffer);
				p_text_buffer = &text_buffer[0];
				
				link_name_or_value[0] = 'H';
				link_name_or_value[1] = character;
				link_name_or_value[2] = 0x00;
				display_html_look_for_matching_style(tags_stack, (this_doc_styles_count + global_styles_count), &link_name_or_value[0]);
				continue;
			}

			if ((previous_characters[2] == '<') && (previous_characters[1] == 'D') && (previous_characters[0] == 'I') && (character == 'V'))
			{
				//--------------------------------------
				//----- THIS IS A START OF DIV TAG -----
				//--------------------------------------
				inside_tag = INSIDE_TAG_IS_DIV_TAG;

				display_html_push_stack_for_new_tag(&tags_stack[0], &tags_stack_current_depth, &text_buffer[0], p_text_buffer);
				p_text_buffer = &text_buffer[0];

				continue;
			}

			if ((previous_characters[2] == '<') && (previous_characters[1] == 'I') && (previous_characters[0] == 'M') && (character == 'G'))
			{
				//-----------------------------------------
				//----- THIS IS START OF AN IMAGE TAG -----
				//-----------------------------------------
				inside_tag = INSIDE_TAG_IS_IMG_TAG;
				
				transparency_colour = 0xffffffff;
				
				p_link_name_or_value = &link_name_or_value[0];
				continue;
			}

#ifdef HTML_INCLUDE_OPTION_SELECT				//>>>>>>>>>>>> REMOVE THIS SECTION! #####################################
			if ((previous_characters[5] == '<') && (previous_characters[4] == 'S') && (previous_characters[3] == 'E') && (previous_characters[2] == 'L') && (previous_characters[1] == 'E') && (previous_characters[0] == 'C') && (character == 'T'))
			{
				//-----------------------------------------
				//----- THIS IS START OF A SELECT TAG -----
				//-----------------------------------------
				
				if (option_select_active)			//We only support a single option select on a page
					continue;
				
				//Display any preceeding text first
				display_any_pending_elements(tags_stack, tags_stack_current_depth, &text_buffer[0], p_text_buffer);
				p_text_buffer = &text_buffer[0];
				
				inside_tag = INSIDE_TAG_IS_SELECT_TAG;
				
				//Set default values
				option_select_active = 1;				//1 = active but not yet created
				option_select_doing_index = 0;
				option_select_no_of_display_lines = 0xff;
				option_select_this_option_is_default = 0;
				
				//We will now call the option select fucntions as each option is read
				
				continue;
			}
			
			if ((previous_characters[5] == '<') && (previous_characters[4] == 'O') && (previous_characters[3] == 'P') && (previous_characters[2] == 'T') && (previous_characters[1] == 'I') && (previous_characters[0] == 'O') && (character == 'N'))
			{
				//------------------------------------------
				//----- THIS IS START OF AN OPTION TAG -----
				//------------------------------------------
				//This will be inside select tags and is for an option select
				
				if (option_select_active == 0)			//Ignore if not inside select tags
					continue;
								
				inside_tag = INSIDE_TAG_IS_OPTION_TAG;
				p_text_buffer = &text_buffer[0];			//Should already be, but make sure
				
				p_link_name_or_value = &link_name_or_value[0];
								
				continue;
			}
#endif											//<<<<<<<<<<<< REMOVE THIS SECTION! #####################################


#ifdef HTML_INCLUDE_TEXT_INPUT					//>>>>>>>>>>>> REMOVE THIS SECTION! #####################################
			if ((previous_characters[4] == '<') && (previous_characters[3] == 'I') && (previous_characters[2] == 'N') && (previous_characters[1] == 'P') && (previous_characters[0] == 'U') && (character == 'T'))
			{
				//-----------------------------------------
				//----- THIS IS START OF AN INPUT TAG -----
				//-----------------------------------------
				
				if (text_inputs_active_count >= MAX_TEXT_INPUT_BOXES)			//Check another is allowed
					continue;
				
				//Display any preceeding text first
				display_any_pending_elements(tags_stack, tags_stack_current_depth, &text_buffer[0], p_text_buffer);
				p_text_buffer = &text_buffer[0];
				
				inside_tag = INSIDE_TAG_IS_INPUT_TAG;
				
				//Set default values
				text_inputs_active_count++;
				last_text_box_contents[(text_inputs_active_count - 1)][0] = 0x00;		//Clear any existing string
				text_box_top[(text_inputs_active_count - 1)] = tags_stack[0].top;
				text_box_left[(text_inputs_active_count - 1)] = tags_stack[0].left;
				text_box_width[(text_inputs_active_count - 1)] = tags_stack[0].width;
				text_box_max_length[(text_inputs_active_count - 1)] = 0xff;
				text_box_font[(text_inputs_active_count - 1)] = tags_stack[0].font_size;
				text_box_fore_colour[(text_inputs_active_count - 1)] = tags_stack[0].fore_colour;
				text_box_background_colour[(text_inputs_active_count - 1)] = current_style_background_colour_copy;

				continue;
			}
#endif											//<<<<<<<<<<<< REMOVE THIS SECTION! #####################################

			if ((previous_characters[0] == '<') && (character == 'A'))
			{
				//-------------------------------------------------
				//----- THIS IS START OF AN ANCHOR (LINK) TAG -----
				//-------------------------------------------------
				#ifdef HTML_PASS_ANCHOR_LINK_FUNCTION				//Don't do if we're not storing links
					inside_tag = INSIDE_TAG_IS_ANCHOR_LINK_TAG;
								
					p_anchor_link_value = &anchor_link_value[0];
					anchor_get_next_start_coord = 1;
					anchor_link_end_x = 0;
					anchor_link_end_y = 0;
					continue;
				#endif
			}
			if ((previous_characters[4] == '<') && (previous_characters[3] == 'S') && (previous_characters[2] == 'T') && (previous_characters[1] == 'Y') && (previous_characters[0] == 'L') && (character == 'E'))
			{
				//----------------------------------------
				//----- THIS IS START OF A STYLE TAG -----
				//----------------------------------------
				
				//WE PROCESS STYLE DEFINITIONS SEPERATELY AS THEY HAVE A DIFFERNET FORMATTING TO STANDARD HTML TAGS
				display_html_read_styles(&file_size, &this_doc_styles_count, 0);
				
				//The funtions exits with just the closing tag to go
				inside_tag = INSIDE_TAG_IS_CLOSING_TAG;
				
				continue;
			}

			if ((previous_characters[3] == '<') && (previous_characters[2] == 'B') && (previous_characters[1] == 'O') && (previous_characters[0] == 'D') && (character == 'Y'))
			{
				//---------------------------------------
				//----- THIS IS START OF A BODY TAG -----
				//---------------------------------------
				we_are_in_page_body = 1;
				
				//display_clear_screen(DISPLAY_COLOUR_WHITE);
				
				continue;
			}



			//---------------------------------
			//---------------------------------
			//----- LOOK FOR CLOSING TAGS -----
			//---------------------------------
			//---------------------------------

			if ((previous_characters[8] == '<') && (previous_characters[7] == '!') && (previous_characters[6] == '-') && (previous_characters[5] == '-') && (previous_characters[4] == '#') && (previous_characters[1] == '-') && (previous_characters[0] == '-') && (character == '>'))
			{
				//---------------------------------------
				//----- DYNAMIC TEXT COMMENT MARKER -----
				//---------------------------------------
				//This is a special marker we use to include dynamic text: <!--#00-->
				
				data = previous_characters[3] - 0x30;
				if (data <= 9)						//Should be 0 to 9
				{
					dynamic_text_index = data * 10;
					data = previous_characters[2] - 0x30;
					if (data <= 9)					//Should be 0 to 9
					{
						dynamic_text_index += data;
						dynamic_text_character_index = 0;			//This flags that dynamic text needs to be added
					}
				}
			}		

			if ((previous_characters[1] == '-') && (previous_characters[0] == '-') && (character == '>'))
			{
				//-------------------------------
				//----- COMMENT CLOSING TAG -----
				//-------------------------------
				//Comments are an exception in that they don't end with "/>"
				
				inside_tag = INSIDE_TAG_IS_CLOSING_TAG;
			}


			if ((previous_characters[0] == '/') && (character == '>'))
			{
				//-------------------------------
				//----- CLOSING OF THIS TAG -----
				//-------------------------------
				if (
				(inside_tag == INSIDE_TAG_IS_HEADING_TAG) ||
				(inside_tag == INSIDE_TAG_IS_DIV_TAG)
				)
				{
					//----------------------------------------------------------------
					//----- THIS WAS A TAG WE PUSHED THE TAG STACK FOR SO POP IT -----
					//----------------------------------------------------------------
					display_html_pop_stack_for_closing_tag(&tags_stack[0], &tags_stack_current_depth, &text_buffer[0], p_text_buffer);
					p_text_buffer = &text_buffer[0];
				}
				else if (inside_tag == INSIDE_TAG_IS_IMG_TAG)
				{
					//----------------------------------------------
					//----- THIS WAS AN IMAGE TAG - DISPLAY IT -----
					//----------------------------------------------
					if (p_link_name_or_value > &link_name_or_value[2])
					{
						//Display any preceeding text first
						display_any_pending_elements(tags_stack, tags_stack_current_depth, &text_buffer[0], p_text_buffer);
						p_text_buffer = &text_buffer[0];

						//Display the image
						*p_link_name_or_value = 0x00;
						
						//We convert any .png filenames to be .bmp to allow html designers to work with .png files if desired
						if (
						(*(p_link_name_or_value - 3) == 'P') &&
						(*(p_link_name_or_value - 2) == 'N') &&
						(*(p_link_name_or_value - 1) == 'G')
						)
						{
							*(p_link_name_or_value - 3) = 'B';
							*(p_link_name_or_value - 2) = 'M';
							*(p_link_name_or_value - 1) = 'P';
						}
						
						
						//Check for just entered anchor link section and need to store start coords
						#ifdef HTML_PASS_ANCHOR_LINK_FUNCTION				//Don't do if we're not storing links				
							if (anchor_get_next_start_coord)
							{
								anchor_get_next_start_coord = 0;
								anchor_link_start_x = (tags_stack[0].left + tags_stack[0].border_width + tags_stack[0].padding_hoz);
								anchor_link_start_y = (tags_stack[0].top + tags_stack[0].border_width + tags_stack[0].padding_vert);
							}
						#endif
						
						
						b_temp = DISPLAY_HTML_OPEN_IMAGE_FILE(link_name_or_value);
						if (b_temp != 0)
						{
							tags_stack[0].top += display_bitmap(0,
															(tags_stack[0].left + tags_stack[0].border_width + tags_stack[0].padding_hoz),
															(tags_stack[0].top + tags_stack[0].border_width + tags_stack[0].padding_vert),
															transparency_colour);
						}

					//Always store the last end coords for possible next anchor link closing tag
					#ifdef HTML_PASS_ANCHOR_LINK_FUNCTION				//Don't do if we're not storing links				
						if (anchor_link_end_x < display_auto_x_coordinate)
							anchor_link_end_x = display_auto_x_coordinate;
						if (anchor_link_end_y < display_auto_y_coordinate)
							anchor_link_end_y = display_auto_y_coordinate;
					#endif

					}
				}
				inside_tag = INSIDE_TAG_IS_CLOSING_TAG;
				//continue;		//If character is '>' then let it get processed as end of tag below
			}

			if ((previous_characters[2] == '<') && (previous_characters[1] == '/') && (previous_characters[0] == 'H') && (character >= '1') && (character <= '6'))
			{
				//-----------------------------------------------
				//----- CLOSING TAG OF HEADING TEXT SECTION -----
				//-----------------------------------------------

				//THIS WAS A TAG WE PUSHED THE TAG STACK FOR SO POP IT (Which will display the previous text)
				display_html_pop_stack_for_closing_tag(&tags_stack[0], &tags_stack_current_depth, &text_buffer[0], p_text_buffer);
				p_text_buffer = &text_buffer[0];

				inside_tag = INSIDE_TAG_IS_CLOSING_TAG;
				continue;
			}

			if ((previous_characters[3] == '<') && (previous_characters[2] == '/') && (previous_characters[1] == 'D')  && (previous_characters[0] == 'I') && (character == 'V'))
			{
				//--------------------------------------
				//----- CLOSING TAG OF DIV SECTION -----
				//--------------------------------------

				//THIS WAS A TAG WE PUSHED THE TAG STACK FOR SO POP IT (Which will display the previous text)
				display_html_pop_stack_for_closing_tag(&tags_stack[0], &tags_stack_current_depth, &text_buffer[0], p_text_buffer);
				p_text_buffer = &text_buffer[0];

				inside_tag = INSIDE_TAG_IS_CLOSING_TAG;
				continue;
			}

			if ((previous_characters[2] == '<')  && (previous_characters[1] == '/')  && (previous_characters[0] == 'A') && (character == '>'))
			{
				//------------------------------------------------
				//----- CLOSING TAG OF ANCHOR (LINK) SECTION -----
				//------------------------------------------------
				#ifdef HTML_PASS_ANCHOR_LINK_FUNCTION
					if (anchor_link_value[0] != 0x00)			//Ensure there was a start to this tag
					{
						*p_anchor_link_value = 0x00;
						
						//End corods will be actual coord +1
						anchor_link_end_x--;
						anchor_link_end_y--;
						
						//Check end >= to start coord
						if (anchor_link_end_x < anchor_link_start_x)
							anchor_link_end_x = anchor_link_start_x;
						if (anchor_link_end_y < anchor_link_start_y)
							anchor_link_end_y = anchor_link_start_y;

						HTML_PASS_ANCHOR_LINK_FUNCTION(&anchor_link_value[0],
														anchor_link_start_x, anchor_link_start_y,
														anchor_link_end_x, anchor_link_end_y);

						anchor_link_value[0] = 0x00;
					}
				#endif
				inside_tag = INSIDE_TAG_IS_CLOSING_TAG;
				continue;
			}


#ifdef HTML_INCLUDE_OPTION_SELECT				//>>>>>>>>>>>> REMOVE THIS SECTION! #####################################
			if ((previous_characters[6] == '<') && (previous_characters[5] == '/') && (previous_characters[4] == 'O') && (previous_characters[3] == 'P') && (previous_characters[2] == 'T') && (previous_characters[1] == 'I') && (previous_characters[0] == 'O') && (character == 'N'))
			{
				//-----------------------------------------
				//----- CLOSING TAG OF OPTION SECTION -----
				//-----------------------------------------
				
				if (option_select_active == 0)
					continue;
				
				if (option_select_active == 1)
				{
					//----------------------------------------------
					//----- FIRST OPTION - SETUP OPTION SELECT -----
					//----------------------------------------------
					if (option_select_no_of_display_lines == 0xff)
						option_select_no_of_display_lines = 1;
					
					//Set colours
					dw_temp = tags_stack[0].fore_colour;
					dw_temp1 = current_style_background_colour_copy;
					if (dw_temp == DISPLAY_COLOUR_NULL)
						dw_temp = DISPLAY_COLOUR_BLACK;
					if (dw_temp1 == DISPLAY_COLOUR_NULL)
						dw_temp1 = DISPLAY_COLOUR_WHITE;
					if (option_select_no_of_display_lines == 1)
					{
						opt_sel_fore_colour_selected = dw_temp;
						opt_sel_background_colour_selected = dw_temp1;
						opt_sel_fore_colour_not_selected = dw_temp;
						opt_sel_background_colour_not_selected = dw_temp1;
					}
					else
					{
						opt_sel_fore_colour_selected = current_style_background_colour_copy;
						opt_sel_background_colour_selected = dw_temp;
						opt_sel_fore_colour_not_selected = dw_temp;
						opt_sel_background_colour_not_selected = dw_temp1;
					}

					
					//Set width
					w_temp = tags_stack[0].width;
					if (w_temp == 0)
						w_temp = DISPLAY_WIDTH_PIXELS - tags_stack[0].left;
						
					//Set align
					b_temp = OPT_SEL_OPTIONS_CENTER_ALIGN;
					if (tags_stack[0].align == 0)
						b_temp = OPT_SEL_OPTIONS_LEFT_ALIGN;

					//Select font
					p_font_to_use = html_fonts[(HTML_TOTAL_NUMBER_OF_FONTS - 1)];			//Select default font size as largest
					for (count = 0; count < HTML_TOTAL_NUMBER_OF_FONTS; count++)
					{
						if (tags_stack[0].font_size <= html_font_sizes[count])
						{
							p_font_to_use = html_fonts[count];
							break;
						}
					}
					
					option_select_start_new (0,														//Selected line at start (current value)
											tags_stack[0].left, w_temp, b_temp,						//Column 0:- start_x_coord, width, options
											0, 0, OPT_SEL_OPTIONS_COLUMN_NOT_USED,					//Column 1:- start_x_coord, width, options
											0, 0, OPT_SEL_OPTIONS_COLUMN_NOT_USED,					//Column 2:- start_x_coord, width, options
											0, 0, OPT_SEL_OPTIONS_COLUMN_NOT_USED,					//Column 3:- start_x_coord, width, options
											0, 0, OPT_SEL_OPTIONS_COLUMN_NOT_USED,					//Column 4:- start_x_coord, width, options
											option_select_no_of_display_lines, tags_stack[0].top,	//max lines_to_display, y_start_coord
											(tags_stack[0].font_size + 2), 1,						//line_height, text_y_offset
											p_font_to_use);										//Font
				}
				option_select_active = 2;

				//---------------------------
				//----- ADD THIS OPTION -----
				//---------------------------
				if (option_select_doing_index < OPTION_EDIT_MAX_OPTION_LINES)
				{
					*p_text_buffer = 0x00;
					option_select_add_line_variable(&text_buffer[0],
											0, 0, 0, 0,												 		//Column 1: value, min, max, options
											0, 0, 0, 0,														//Column 2
											0, 0, 0, 0,														//Column 3
											0, 0, 0, 0);													//Column 4
				
					if (option_select_this_option_is_default)
					{
						option_select_set_selected_line(option_select_doing_index);
						option_select_this_option_is_default = 0;
					}
	
					option_select_doing_index++;
				}
			}
#endif											//<<<<<<<<<<<< REMOVE THIS SECTION! #####################################


			if ((previous_characters[5] == '<') && (previous_characters[4] == '/') && (previous_characters[3] == 'S') && (previous_characters[2] == 'T') && (previous_characters[1] == 'Y') && (previous_characters[0] == 'L') && (character == 'E'))
			{
				//------------------------------------
				//----- CLOSING OF STYLE SECTION -----
				//------------------------------------
				inside_tag = INSIDE_TAG_IS_CLOSING_TAG;
				
				continue;
			}

			if ((previous_characters[1] == '<') && (previous_characters[0] == '/') && (character == 'P'))
			{
				//------------------------------------------
				//----- THIS IS THE END OF A PARAGRAPH -----
				//------------------------------------------
				//Display any preceeding text first
				if (p_text_buffer != &text_buffer[0])
				{
					display_any_pending_elements(tags_stack, tags_stack_current_depth, &text_buffer[0], p_text_buffer);
					p_text_buffer = &text_buffer[0];

					if (tags_stack[0].height)
						tags_stack[0].height = tags_stack[0].height - (display_bitmap_height + display_font_inter_line_space);
					tags_stack[0].top += (display_bitmap_height + display_font_inter_line_space);
				}
			}

			if ((previous_characters[3] == '<') && (previous_characters[2] == 'B') && (previous_characters[1] == 'R') && (previous_characters[0] == '/') && (character == '>'))
			{
				//--------------------------------
				//----- THIS IS A LINE BREAK -----
				//--------------------------------
				//Display any preceeding text first
				if (p_text_buffer != &text_buffer[0])
				{
					display_any_pending_elements(tags_stack, tags_stack_current_depth, &text_buffer[0], p_text_buffer);
					p_text_buffer = &text_buffer[0];
				}
			}


			if ((previous_characters[4] == '<') && (previous_characters[3] == '/') && (previous_characters[2] == 'B') && (previous_characters[1] == 'O') && (previous_characters[0] == 'D') && (character == 'Y'))
			{
				//----------------------------
				//----- CLOSING BODY TAG -----
				//----------------------------
				break;					//End reading file
			}


			if (character == '>')
			{
				//----------------------
				//----- END OF TAG -----
				//----------------------
				inside_tag = NOT_INSIDE_TAG;
				continue;
			}
			
		
			//-------------------------------------------------------
			//-------------------------------------------------------
			//----- FROM HERE WE ARE LOOKING FOR TAG PROPERTIES -----
			//-------------------------------------------------------
			//-------------------------------------------------------
			
			if (inside_tag_paramater_state == PARAMETER_READING_NAME)
			{
				//---------------------------------------------
				//----- WE ARE LOOKING FOR PARAMETER NAME -----
				//---------------------------------------------
				if ((previous_characters[0] == '=') && (character == '\"'))
				{
					//------------------------------------------------------------
					//----- END OF PARAMETER NAME - START OF PARAMETER VALUE -----
					//------------------------------------------------------------
					if ((previous_characters[3] == 'S') && (previous_characters[2] == 'R') && (previous_characters[1] == 'C'))
					{
						//--------------------------------------
						//----- THIS IS A 'SRC=' PARAMETER -----
						//--------------------------------------
						inside_tag_paramater_state = PARAMETER_READING_SRC;
						p_link_name_or_value = &link_name_or_value[0];
					}
					else if ((previous_characters[3] == 'A') && (previous_characters[2] == 'L') && (previous_characters[1] == 'T'))
					{
						//--------------------------------------
						//----- THIS IS A 'ALT=' PARAMETER -----
						//--------------------------------------
						inside_tag_paramater_state = PARAMETER_READING_ALT;
					}
					else if ((previous_characters[5] == 'C') && (previous_characters[4] == 'L') && (previous_characters[3] == 'A') && (previous_characters[2] == 'S') && (previous_characters[1] == 'S'))
					{
						//----- THIS IS A 'CLASS=' PARAMETER -----
						inside_tag_paramater_state = PARAMETER_READING_CLASS;
						p_link_name_or_value = &link_name_or_value[0];
					}
#ifdef HTML_INCLUDE_OPTION_SELECT				//>>>>>>>>>>>> REMOVE THIS SECTION! #####################################
					else if ((previous_characters[4] == 'S') && (previous_characters[3] == 'I') && (previous_characters[2] == 'Z') && (previous_characters[1] == 'E'))
					{
						//---------------------------------------
						//----- THIS IS A 'SIZE=' PARAMETER -----
						//---------------------------------------
						if (inside_tag == INSIDE_TAG_IS_SELECT_TAG)
						{
							inside_tag_paramater_state = PARAMETER_READING_SELECT_SIZE;
							p_style_attribute_name = &style_attribute_name[0];
						}
					}
					else if ((previous_characters[8] == 'S') && (previous_characters[7] == 'E') && (previous_characters[6] == 'L') && (previous_characters[5] == 'E') && (previous_characters[4] == 'C') && (previous_characters[3] == 'T') && (previous_characters[2] == 'E') && (previous_characters[1] == 'D'))
					{
						//-------------------------------------------
						//----- THIS IS A 'SELECTED=' PARAMETER -----
						//-------------------------------------------
						if (inside_tag == INSIDE_TAG_IS_OPTION_TAG)
						{
							inside_tag_paramater_state = PARAMETER_READING_OPTION_SELECTED;
							p_link_name_or_value = &link_name_or_value[0];
							p_style_attribute_name = &style_attribute_name[0];
						}
					}
#endif											//<<<<<<<<<<<< REMOVE THIS SECTION! #####################################
#ifdef HTML_INCLUDE_TEXT_INPUT					//>>>>>>>>>>>> REMOVE THIS SECTION! #####################################
					else if ((previous_characters[4] == 'T') && (previous_characters[3] == 'Y') && (previous_characters[2] == 'P') && (previous_characters[1] == 'E'))
					{
						//---------------------------------------
						//----- THIS IS A 'TYPE=' PARAMETER -----
						//---------------------------------------
						if (inside_tag == INSIDE_TAG_IS_INPUT_TAG)
						{
							inside_tag_paramater_state = PARAMETER_READING_INPUT_TYPE;
							p_link_name_or_value = &link_name_or_value[0];
							p_style_attribute_name = &style_attribute_name[0];
						}
					}
					else if ((previous_characters[5] == 'V') && (previous_characters[4] == 'A') && (previous_characters[3] == 'L') && (previous_characters[2] == 'U') && (previous_characters[1] == 'E'))
					{
						//----------------------------------------
						//----- THIS IS A 'VALUE=' PARAMETER -----
						//----------------------------------------
						if (inside_tag == INSIDE_TAG_IS_INPUT_TAG)
						{
							inside_tag_paramater_state = PARAMETER_READING_INPUT_VALUE;
							p_link_name_or_value = &last_text_box_contents[(text_inputs_active_count - 1)][0];
							p_style_attribute_name = &style_attribute_name[0];
						}
					}
					else if ((previous_characters[9] == 'M') && (previous_characters[8] == 'A') && (previous_characters[7] == 'X') && (previous_characters[6] == 'L') && (previous_characters[5] == 'E') && (previous_characters[4] == 'N') && (previous_characters[3] == 'G')  && (previous_characters[2] == 'T') && (previous_characters[1] == 'H'))
					{
						//--------------------------------------------
						//----- THIS IS A 'MAXLENGTH=' PARAMETER -----
						//--------------------------------------------
						if (inside_tag == INSIDE_TAG_IS_INPUT_TAG)
						{
							inside_tag_paramater_state = PARAMETER_READING_INPUT_MAX_LENGTH;
							p_link_name_or_value = &link_name_or_value[0];
							p_style_attribute_name = &style_attribute_name[0];
						}
					}
#endif											//<<<<<<<<<<<< REMOVE THIS SECTION! #####################################
					else if ((previous_characters[5] == 'S') && (previous_characters[4] == 'T') && (previous_characters[3] == 'Y') && (previous_characters[2] == 'L') && (previous_characters[1] == 'E'))
					{
						//----------------------------------------
						//----- THIS IS A 'STYLE=' PARAMETER -----
						//----------------------------------------
						inside_tag_paramater_state = PARAMETER_READING_STYLE_ATTRIBUTE_NAME;
						p_style_attribute_name = &style_attribute_name[0];
					}
					else if ((previous_characters[4] == 'H') && (previous_characters[3] == 'R') && (previous_characters[2] == 'E') && (previous_characters[1] == 'F'))
					{
						//----------------------------------------
						//----- THIS IS A 'HREF=' PARAMETER -----
						//----------------------------------------
						inside_tag_paramater_state = PARAMETER_READING_HREF;
						p_style_attribute_name = &style_attribute_name[0];
					}

				}
			}
			else if (inside_tag_paramater_state == PARAMETER_READING_SRC)			
			{
				//------------------------------------------
				//------------------------------------------
				//----- WE ARE READING 'SRC' FILE NAME -----
				//------------------------------------------
				//------------------------------------------
				if (character == '\"')
				{
					inside_tag_paramater_state = PARAMETER_READING_NAME;		//Reset ready for next parameter
				}
				else if (p_link_name_or_value < &link_name_or_value[12])							//Always leave final byte for trailing null to be added when link is used
				{
					*p_link_name_or_value++ = character;
				}
			}
			else if (inside_tag_paramater_state == PARAMETER_READING_ALT)
			{
				//--------------------------------
				//--------------------------------
				//----- WE ARE READING 'ALT' -----
				//--------------------------------
				//--------------------------------
				if (character == '\"')
				{
					inside_tag_paramater_state = PARAMETER_READING_NAME;		//Reset ready for next parameter
				}
				else if (inside_tag == INSIDE_TAG_IS_IMG_TAG)
				{
					//----- THIS IS AN IMAGE - ALT MAY BE USED TO SPECIFY A TRANSPARENCY COLOUR -----
					//(This is a special modifier we use)
					if (character == '#')
					{
						transparency_colour = 0x00000000;
					}
					else if (transparency_colour != 0xffffffff)
					{
						if (character > '9')
							data = (character - ('A' - 10));
						else
							data = (character - '0');
			
						if (data > 15)
						{
							transparency_colour = 0xffffffff;
						}
						else
						{			
							transparency_colour <<= 4;
							transparency_colour |= (DWORD)data;
						}
					}
				}
			}			
			else if (inside_tag_paramater_state == PARAMETER_READING_CLASS)			
			{
				//---------------------------------------
				//---------------------------------------
				//----- WE ARE READING 'CLASS' NAME -----
				//---------------------------------------
				//---------------------------------------
				if (character == '\"')
				{
					//----- END OF CLASS NAME -----
					if (p_link_name_or_value > &link_name_or_value[0])
					{
						*p_link_name_or_value = 0x00;
						
						//----- SEE IF CLASS MATCHES A STYLE NAME -----
						display_html_look_for_matching_style(tags_stack, (this_doc_styles_count + global_styles_count), &link_name_or_value[0]);
					}
					
					inside_tag_paramater_state = PARAMETER_READING_NAME;		//Reset ready for next parameter
				}
				else if (p_link_name_or_value < &link_name_or_value[12])							//Always leave final byte for trailing null to be added when link is used
				{
					*p_link_name_or_value++ = character;
				}
			}
#ifdef HTML_INCLUDE_OPTION_SELECT				//>>>>>>>>>>>> REMOVE THIS SECTION! #####################################
			else if (inside_tag_paramater_state == PARAMETER_READING_SELECT_SIZE)			
			{
				//--------------------------------------
				//--------------------------------------
				//----- WE ARE READING SELECT SIZE -----
				//--------------------------------------
				//--------------------------------------
				if (character == '\"')
				{
					//----- END OF SIZE -----
					inside_tag_paramater_state = PARAMETER_READING_NAME;		//Reset ready for next parameter
				}
				else if (p_link_name_or_value < &link_name_or_value[12])							//Always leave final byte for trailing null to be added when link is used
				{
					//----- NEXT CHARACTER OF SIZE -----
					data = (character - 0x30);
					if (data > 9)
					{
						inside_tag_paramater_state = PARAMETER_READING_NAME;	//Invalid value
						continue;
					}					
					
					if (option_select_no_of_display_lines == 0xff)				//Default value
						option_select_no_of_display_lines = 0;
					
					option_select_no_of_display_lines *= 10;
					option_select_no_of_display_lines += data;
				}
			}		
			else if (inside_tag_paramater_state == PARAMETER_READING_OPTION_SELECTED)			
			{
				//------------------------------------------
				//------------------------------------------
				//----- WE ARE READING OPTION SELECTED -----
				//------------------------------------------
				//------------------------------------------
				if (character == '\"')
				{
					inside_tag_paramater_state = PARAMETER_READING_NAME;		//Reset ready for next parameter
					
					//Flag if this option is the default option
					if ((link_name_or_value[0] == 'S') && (link_name_or_value[1] == 'E') && (link_name_or_value[2] == 'L') && (link_name_or_value[3] == 'E') && (link_name_or_value[4] == 'C') && (link_name_or_value[5] == 'T') && (link_name_or_value[6] == 'E') && (link_name_or_value[7] == 'D'))
						option_select_this_option_is_default = 1;
				}
				else if (p_link_name_or_value < &link_name_or_value[12])							//Always leave final byte for trailing null to be added when link is used
				{
					*p_link_name_or_value++ = character;
				}
			}
#endif											//<<<<<<<<<<<< REMOVE THIS SECTION! #####################################
#ifdef HTML_INCLUDE_TEXT_INPUT					//>>>>>>>>>>>> REMOVE THIS SECTION! #####################################
			else if (inside_tag_paramater_state == PARAMETER_READING_INPUT_TYPE)			
			{
				//-------------------------------------
				//-------------------------------------
				//----- WE ARE READING INPUT TYPE -----
				//-------------------------------------
				//-------------------------------------
				if (character == '\"')
				{
					inside_tag_paramater_state = PARAMETER_READING_NAME;		//Reset ready for next parameter
					
					//If input type is not text then cancel input box
					if ((link_name_or_value[0] != 'T') || (link_name_or_value[1] != 'E') || (link_name_or_value[2] != 'X') || (link_name_or_value[3] != 'T'))
					{
						if (text_inputs_active_count)
							text_inputs_active_count--;
						
						inside_tag = INSIDE_TAG_NULL;
					}

				}
				else if (p_link_name_or_value < &link_name_or_value[12])							//Always leave final byte for trailing null to be added when link is used
				{
					*p_link_name_or_value++ = character;
				}
			}
			else if (inside_tag_paramater_state == PARAMETER_READING_INPUT_VALUE)			
			{
				//--------------------------------------
				//--------------------------------------
				//----- WE ARE READING INPUT VALUE -----
				//--------------------------------------
				//--------------------------------------
				if (character == '\"')
				{
					inside_tag_paramater_state = PARAMETER_READING_NAME;		//Reset ready for next parameter
					*p_link_name_or_value = 0x00;
				}
				else if (p_link_name_or_value < &last_text_box_contents[(text_inputs_active_count - 1)][(MAX_TEXT_BOX_LENGTH - 1)])		//Always leave final byte for trailing null to be added when link is used
				{
					if (space_character_before_this_character)		//Spaces have been stripped so add it if there was one
						*p_link_name_or_value++ = ' ';
						
					if (p_link_name_or_value < &last_text_box_contents[(text_inputs_active_count - 1)][(MAX_TEXT_BOX_LENGTH - 1)])
						*p_link_name_or_value++ = character_pre_upper_case_change;

					//Check for special HTML codes (&lt; = '<', &gt; = '>')
					if (p_link_name_or_value >= &last_text_box_contents[(text_inputs_active_count - 1)][4])
					{
						if ((*(p_link_name_or_value - 4) == '&') && (*(p_link_name_or_value - 3) == 'l') && (*(p_link_name_or_value - 2) == 't') && (*(p_link_name_or_value - 1) == ';'))
						{
							p_link_name_or_value -= 4;
							*p_link_name_or_value++ = '<';
						}
						if ((*(p_link_name_or_value - 4) == '&') && (*(p_link_name_or_value - 3) == 'g') && (*(p_link_name_or_value - 2) == 't') && (*(p_link_name_or_value - 1) == ';'))
						{
							p_link_name_or_value -= 4;
							*p_link_name_or_value++ = '>';
						}
					}

				}
			}
			else if (inside_tag_paramater_state == PARAMETER_READING_INPUT_MAX_LENGTH)			
			{
				//------------------------------------------
				//------------------------------------------
				//----- WE ARE READING INPUT MAXLENGTH -----
				//------------------------------------------
				//------------------------------------------
				if (character == '\"')
				{
					inside_tag_paramater_state = PARAMETER_READING_NAME;		//Reset ready for next parameter
				}
				else if (p_link_name_or_value < &link_name_or_value[12])							//Always leave final byte for trailing null to be added when link is used
				{
					//----- NEXT CHARACTER OF MAX LENGTH -----
					data = (character - 0x30);
					if (data > 9)
					{
						inside_tag_paramater_state = PARAMETER_READING_NAME;	//Invalid value
						continue;
					}					
					
					if (text_box_max_length[(text_inputs_active_count - 1)] == 0xff)				//Default value
						text_box_max_length[(text_inputs_active_count - 1)] = 0;
					
					text_box_max_length[(text_inputs_active_count - 1)] *= 10;
					text_box_max_length[(text_inputs_active_count - 1)] += data;
				}
			}
#endif											//<<<<<<<<<<<< REMOVE THIS SECTION! #####################################
			#ifdef HTML_PASS_ANCHOR_LINK_FUNCTION
			else if ((inside_tag_paramater_state == PARAMETER_READING_HREF) && (inside_tag == INSIDE_TAG_IS_ANCHOR_LINK_TAG))
			{
				//--------------------------------------------------
				//--------------------------------------------------
				//----- WE ARE READING 'HREF' ANCHOR LINK NAME -----
				//--------------------------------------------------
				//--------------------------------------------------
				if (character == '\"')
				{
					inside_tag_paramater_state = PARAMETER_READING_NAME;		//Reset ready for next parameter
				}
				else if (p_anchor_link_value < &anchor_link_value[12])							//Always leave final byte for trailing null to be added when link is used
				{
					*p_anchor_link_value++ = character;
				}
			}
			#endif
			else if ((inside_tag_paramater_state == PARAMETER_READING_STYLE_ATTRIBUTE_NAME) || (inside_tag_paramater_state == PARAMETER_READING_STYLE_ATTRIBUTE_VALUE))
			{
				//---------------------------------------------
				//---------------------------------------------
				//----- WE ARE READING 'STYLE' ATTRIBUTES -----
				//---------------------------------------------
				//---------------------------------------------
				//E.g. STYLE="top:60px; left:20px; height:30px; width:80px;"
				
				data = character - 0x30;				//Convert to number ready for value attributes
				
				if (character == '\"')
				{
					//-----------------------------------
					//----- END OF STYLE ATTRIBUTES -----
					//-----------------------------------
					inside_tag_paramater_state = PARAMETER_READING_NAME;		//Reset ready for next parameter
				}
				else if (character == ':')
				{
					//---------------------------------
					//----- END OF ATTRIBUTE NAME -----
					//---------------------------------
					*p_style_attribute_name = 0x00;			//Add terminating null
					
					inside_tag_paramater_state = PARAMETER_READING_STYLE_ATTRIBUTE_VALUE;
					p_style_attribute_value = &style_attribute_value[0];			//Reset ready for next value
				}
				else if (character == ';')
				{
					//----------------------------------
					//----- END OF ATTRIBUTE VALUE -----
					//----------------------------------
					*p_style_attribute_value = 0x00;			//Add terminating null
					
					inside_tag_paramater_state = PARAMETER_READING_STYLE_ATTRIBUTE_NAME;
					p_style_attribute_name = &style_attribute_name[0];			//Reset ready for next name

					//Process the new attribute
					process_attribute_value(1, tags_stack, 0, &style_attribute_name[0], &style_attribute_value[0]);
				}
				else if (inside_tag_paramater_state == PARAMETER_READING_STYLE_ATTRIBUTE_NAME)
				{
					//----------------------------------
					//----- READING ATTRIBUTE NAME -----
					//----------------------------------
					if (p_style_attribute_name < &style_attribute_name[(MAX_STYLE_ATTRIBUTE_NAME_LENGTH - 2)])			//Allways leave space for terminating null
						*p_style_attribute_name++ = character;
				}
				else if (inside_tag_paramater_state == PARAMETER_READING_STYLE_ATTRIBUTE_VALUE)
				{
					//-----------------------------------
					//----- READING ATTRIBUTE VALUE -----
					//-----------------------------------
					if (p_style_attribute_value < &style_attribute_value[(MAX_STYLE_ATTRIBUTE_VALUE_LENGTH - 2)])		//Allways leave space for terminating null
						*p_style_attribute_value++ = character;
				}
			}
		}
		else
		{
			//-----------------------------------
			//-----------------------------------
			//----- WE ARE NOT INSIDE A TAG -----
			//-----------------------------------
			//-----------------------------------
			
			//Ignore text pre the page body
			if (!we_are_in_page_body)
				continue;

			//Ignore leading whitespace in text
			if ((p_text_buffer == &text_buffer[0]) && (character == ' '))
				continue;
			
			//Check for text buffer full
			if (p_text_buffer >= (&text_buffer[MAX_TEXT_BUFFER_LENGTH] - 1))		//Always allow final byte for the trailing null to be added prior to display
			{
				display_any_pending_elements(tags_stack, tags_stack_current_depth, &text_buffer[0], p_text_buffer);
				p_text_buffer = &text_buffer[0];
			}
			
			//Add character to text buffer
			*p_text_buffer++ = character;
			
			//Check for special HTML codes (&lt; = '<', &gt; = '>')
			if (p_text_buffer >= &text_buffer[4])
			{
				if ((*(p_text_buffer - 4) == '&') && (*(p_text_buffer - 3) == 'l') && (*(p_text_buffer - 2) == 't') && (*(p_text_buffer - 1) == ';'))
				{
					p_text_buffer -= 4;
					*p_text_buffer++ = '<';
				}
				if ((*(p_text_buffer - 4) == '&') && (*(p_text_buffer - 3) == 'g') && (*(p_text_buffer - 2) == 't') && (*(p_text_buffer - 1) == ';'))
				{
					p_text_buffer -= 4;
					*p_text_buffer++ = '>';
				}
			}


		} //if (inside_tag)
		
	} //while (file_size--)
	
	//----- CLOSE THE FILE -----
	#ifdef DISPLAY_HTML_CLOSE_READ
		DISPLAY_HTML_CLOSE_READ;
	#endif

}




//**************************************************
//**************************************************
//********** DIV STYLES BUFFER PUSH & POP **********
//**************************************************
//**************************************************
//These are sub functions used by the display_html_file() function to push and pop styles as the html file is parsed.
//We nest styles from tags.  tags_stack[0] is the current style in use.  As a new style is encountered the buffer is shifted down.
//Default values are used outside of any DIV tags.
//Backgrounds and borders
//We allow nesting of DIV tags but we display the page as HTML is decoded (because we don't have the resources to buffer an entire page before displaying).
//Therefore as new nested DIV tags are encountered we have to display any background or border from the previous DIV tag as the new DIV tag is overlaid on top
//of the previous.  This means that background and borders are never actually nested and are instead only relevant to the current DIV tag.  When a new DIV tag
//is encountered the we check to see if the current DIV tag has a background and if so it is displayed before moving on.


//****************
//***** PUSH *****
//****************
void display_html_push_stack_for_new_tag (DISPLAY_HTML_TAGS_STACK *tags_stack, BYTE *current_depth, BYTE *text_buffer_start, BYTE *p_text_buffer)
{
	BYTE count;
	
	//----- DISPLAY ANY BACKGROUND, BORDER AND TEXT THAT IS PENDING BEFORE WE MOVE INTO THE NEXT TAG -----
	//We have to do this first as the new tag is going to display over the top or after the current tag
	display_any_pending_elements(tags_stack, *current_depth, text_buffer_start, p_text_buffer);


	//----- NEW TAG - PUSH TAGS STACK BUFFER DOWN ONE PLACE -----
	if (*current_depth < 0xff)				//Just in case
		(*current_depth)++;
		
	//----- IF WE'VE EXCEEDED THE BUFFER DEPTH DO NOTHING MORE -----
	if (*current_depth >= MAX_TAGS_NESTING_LEVELS)
		return;

	//----- MOVE THE BUFFER DOWN -----
	count = (MAX_TAGS_NESTING_LEVELS - 1);
	do
	{
		tags_stack[count] = tags_stack[(count - 1)];
	} while (--count);
	
	//----- SETUP THE CURRENT STYLE WITH DEFAULT VALUES -----
	tags_stack[0].top = tags_stack[1].top;
	tags_stack[0].left = tags_stack[1].left;
	tags_stack[0].height = 0;
	tags_stack[0].width = tags_stack[1].width;
	tags_stack[0].border_width = 0;
	tags_stack[0].align = tags_stack[1].align;
	tags_stack[0].font_size = tags_stack[1].font_size;
	tags_stack[0].fore_colour = tags_stack[1].fore_colour;
	tags_stack[0].padding_hoz = 0;
	tags_stack[0].padding_vert = 0;
}

//***************
//***** POP *****
//***************
void display_html_pop_stack_for_closing_tag (DISPLAY_HTML_TAGS_STACK *tags_stack, BYTE *current_depth, BYTE *text_buffer_start, BYTE *p_text_buffer)	
{
	BYTE count;
	WORD current_top;
	
	//----- DISPLAY ANY BACKGROUND, BORDER AND TEXT THAT IS PENDING BEFORE WE MOVE INTO THE NEXT TAG -----
	//We have to do this first as the new tag is going to display over the top or after the current tag
	display_any_pending_elements(tags_stack, *current_depth, text_buffer_start, p_text_buffer);

	//----- POP TAGS STACK BUFFER UP ONE PLACE -----
	if (*current_depth)				//Just in case
	{
		(*current_depth)--;
		
		current_top = tags_stack[0].top;

		for (count = 0; count < (MAX_TAGS_NESTING_LEVELS - 1); count++)
		{
			tags_stack[count] = tags_stack[(count + 1)];
		}
		
		//Ensure we move down the page if necessary
		if (tags_stack[0].top < current_top)
			tags_stack[0].top = current_top;
	}
	else
	{
		//----- AT ROOT LEVEL - NO MORE TAGS TO POP -----	
		//SETUP THE CURRENT STYLE WITH DEFAULT VALUES
		//tags_stack[0].top = 0;		//Keep current top coordinate
		tags_stack[0].left = 0;
		tags_stack[0].height = 0;
		tags_stack[0].width = 0;
		tags_stack[0].border_width = 0;
		tags_stack[0].align = 0;
		tags_stack[0].font_size = 0;
		tags_stack[0].fore_colour = DISPLAY_COLOUR_BLACK;
		tags_stack[0].padding_hoz = 0;
		tags_stack[0].padding_vert = 0;
	}
}


//**************************************************
//**************************************************
//********** DISPLAY ANY PENDING ELEMENTS **********
//**************************************************
//**************************************************
//This is a sub function used by the display_html_file() function to display any pending text or bitmap before it starts a new section of the HTML page.
void display_any_pending_elements (DISPLAY_HTML_TAGS_STACK *tags_stack, BYTE current_depth, BYTE *text_buffer_start, BYTE *p_text_buffer)
{
	WORD x_end_coord;
	WORD y_end_coord;
	WORD options;
	CONSTANT BYTE *p_font_to_use;
	WORD new_top_coord = 0xffff;
	BYTE count;



	//--------------------------------------------------------
	//----- IF CURRENT STYLE HAS A BORDER DISPLAY IT NOW -----
	//--------------------------------------------------------
	//The total width and height or an area is the combination of width/height, border thickness and padding
	if ((tags_stack[0].border_width != 0) && (current_style_border_colour != DISPLAY_COLOUR_NULL) && (tags_stack[0].height != 0) && (tags_stack[0].width != 0))
	{
		//Check for just entered anchor link section and need to store start coords
		#ifdef HTML_PASS_ANCHOR_LINK_FUNCTION				//Don't do if we're not storing links				
			if (anchor_get_next_start_coord)
			{
				anchor_get_next_start_coord = 0;
				anchor_link_start_x = (tags_stack[0].left);
				anchor_link_start_y = (tags_stack[0].top);
			}
		#endif


		//TOP
		display_write_block ((tags_stack[0].left),																									//x_start_coord
							(tags_stack[0].top),																									//y_start_coord
							(tags_stack[0].left + (tags_stack[0].border_width << 1) + (tags_stack[0].padding_hoz << 1) + tags_stack[0].width - 1),	//x_end_coord,
							(tags_stack[0].top + tags_stack[0].border_width - 1),																	//y_end_coord,
							current_style_border_colour);																							//colour
		//LEFT
		display_write_block ((tags_stack[0].left),																									//x_start_coord
							(tags_stack[0].top + tags_stack[0].border_width),																		//y_start_coord
							(tags_stack[0].left + tags_stack[0].border_width - 1),																	//x_end_coord,
							(tags_stack[0].top + tags_stack[0].border_width + (tags_stack[0].padding_vert << 1) + tags_stack[0].height - 1),		//y_end_coord,
							current_style_border_colour);																							//colour
		//RIGHT
		display_write_block ((tags_stack[0].left + tags_stack[0].border_width + (tags_stack[0].padding_hoz << 1) + tags_stack[0].width),			//x_start_coord
							(tags_stack[0].top + tags_stack[0].border_width),																		//y_start_coord
							(tags_stack[0].left + (tags_stack[0].border_width << 1) + (tags_stack[0].padding_hoz << 1) + tags_stack[0].width - 1),	//x_end_coord,
							(tags_stack[0].top + tags_stack[0].border_width + (tags_stack[0].padding_vert << 1) + tags_stack[0].height - 1),		//y_end_coord,
							current_style_border_colour);																						//colour
		//BOTTOM
		display_write_block ((tags_stack[0].left),																									//x_start_coord
							(tags_stack[0].top + tags_stack[0].border_width + (tags_stack[0].padding_vert << 1) + tags_stack[0].height),			//y_start_coord
							(tags_stack[0].left + (tags_stack[0].border_width << 1) + (tags_stack[0].padding_hoz << 1) + tags_stack[0].width - 1),	//x_end_coord,
							(tags_stack[0].top + (tags_stack[0].border_width << 1) + (tags_stack[0].padding_vert << 1) + tags_stack[0].height - 1),	//y_end_coord,
							current_style_border_colour);																							//colour


		//Always store the last end coords for possible next anchor link closing tag
		#ifdef HTML_PASS_ANCHOR_LINK_FUNCTION				//Don't do if we're not storing links				
			if (anchor_link_end_x < (tags_stack[0].left + (tags_stack[0].border_width << 1) + (tags_stack[0].padding_hoz << 1) + tags_stack[0].width - 1))
				anchor_link_end_x = (tags_stack[0].left + (tags_stack[0].border_width << 1) + (tags_stack[0].padding_hoz << 1) + tags_stack[0].width - 1);
			if (anchor_link_end_y < (tags_stack[0].top + (tags_stack[0].border_width << 1) + (tags_stack[0].padding_vert << 1) + tags_stack[0].height - 1))
				anchor_link_end_y = (tags_stack[0].top + (tags_stack[0].border_width << 1) + (tags_stack[0].padding_vert << 1) + tags_stack[0].height - 1);
		#endif
	}
	current_style_border_colour = DISPLAY_COLOUR_NULL;	//Reset it

	//-------------------------------------------------
	//----- IF THERE IS TEXT TO DISPLAY DO IT NOW -----
	//-------------------------------------------------
	if (p_text_buffer != text_buffer_start)
	{
		//Check for just entered anchor link section and need to store start coords
		#ifdef HTML_PASS_ANCHOR_LINK_FUNCTION				//Don't do if we're not storing links				
			if (anchor_get_next_start_coord)
			{
				anchor_get_next_start_coord = 0;
				anchor_link_start_x = (tags_stack[0].left + tags_stack[0].border_width);
				anchor_link_start_y = (tags_stack[0].top + tags_stack[0].border_width);
			}
		#endif
		
		
		//Remove any trailing space
		p_text_buffer--;
		if (*p_text_buffer == ' ')
			p_text_buffer--;
		*++p_text_buffer = 0x00;					//Add terminating null (calling functions ensure there is at least 1 byte left in the buffer)
		p_text_buffer = text_buffer_start;			//Reset pointer to start for future text
		
		options = DISPLAY_TEXT_ALIGN_LEFT;
		if (tags_stack[0].align == 1)
		{
			options = DISPLAY_TEXT_ALIGN_CENTRE;

			if (tags_stack[0].width == 0)
				tags_stack[0].width = DISPLAY_WIDTH_PIXELS - tags_stack[0].left;
		}
		else if (tags_stack[0].align == 2)
		{
			options = DISPLAY_TEXT_ALIGN_RIGHT;

			if (tags_stack[0].width == 0)
				tags_stack[0].width = DISPLAY_WIDTH_PIXELS - tags_stack[0].left;
		}

		//Select font
		p_font_to_use = html_fonts[(HTML_TOTAL_NUMBER_OF_FONTS - 1)];			//Select default font size as largest
		for (count = 0; count < HTML_TOTAL_NUMBER_OF_FONTS; count++)
		{
			if (tags_stack[0].font_size <= html_font_sizes[count])
			{
				p_font_to_use = html_fonts[count];
				break;
			}
		}


		x_end_coord = 0;
		if (tags_stack[0].width > 0)
			x_end_coord = tags_stack[0].left + tags_stack[0].border_width + (tags_stack[0].padding_hoz << 1) + tags_stack[0].width - 1;

		y_end_coord = 0;
		if (tags_stack[0].height)
			y_end_coord = tags_stack[0].top + tags_stack[0].border_width + (tags_stack[0].padding_vert << 1) + tags_stack[0].height - 1;
		

		display_foreground_colour = tags_stack[0].fore_colour;
		display_background_colour = current_style_background_colour;

		display_variable_string (p_font_to_use, options,																					//Font, Options
									tags_stack[0].padding_hoz, tags_stack[0].padding_vert,													//Hoz padding, vertical padding
									(tags_stack[0].left + tags_stack[0].border_width), (tags_stack[0].top + tags_stack[0].border_width),	//X start coord, Y start coord
									x_end_coord, y_end_coord,																				//X end coord, Y end coord
									text_buffer_start);																						//Text



		new_top_coord = display_last_text_line_y_coordinate + display_bitmap_height + display_font_inter_line_space;
		
		current_style_background_colour = DISPLAY_COLOUR_NULL;		//Reset it


		//Always store the last end coords for possible next anchor link closing tag
		#ifdef HTML_PASS_ANCHOR_LINK_FUNCTION				//Don't do if we're not storing links				
			if (anchor_link_end_x < display_auto_x_coordinate)
				anchor_link_end_x = display_auto_x_coordinate;
			if (anchor_link_end_y < display_auto_y_coordinate)
				anchor_link_end_y = display_auto_y_coordinate;
		#endif
	}


	//-------------------------------------------------------------------
	//----- IF CURRENT STYLE HAS A BACKGROUND COLOUR DISPLAY IT NOW -----
	//-------------------------------------------------------------------
	//(After text so that text will have already done it if there was text to display)
	if ((current_style_background_colour != DISPLAY_COLOUR_NULL) && (tags_stack[0].height != 0) && (tags_stack[0].width != 0))
	{
		//Check for just entered anchor link section and need to store start coords
		#ifdef HTML_PASS_ANCHOR_LINK_FUNCTION				//Don't do if we're not storing links				
			if (anchor_get_next_start_coord)
			{
				anchor_get_next_start_coord = 0;
				anchor_link_start_x = (tags_stack[0].left + tags_stack[0].border_width);
				anchor_link_start_y = (tags_stack[0].top + tags_stack[0].border_width);
			}
		#endif

		
		display_write_block ((tags_stack[0].left + tags_stack[0].border_width),																	//x_start_coord
							(tags_stack[0].top + tags_stack[0].border_width),																	//y_start_coord
							(tags_stack[0].left + tags_stack[0].border_width + (tags_stack[0].padding_hoz << 1) + tags_stack[0].width - 1),		//x_end_coord,
							(tags_stack[0].top + tags_stack[0].border_width + (tags_stack[0].padding_vert << 1) + tags_stack[0].height - 1),	//y_end_coord,
							current_style_background_colour);																					//colour


		//Always store the last end coords for possible next anchor link closing tag
		#ifdef HTML_PASS_ANCHOR_LINK_FUNCTION				//Don't do if we're not storing links				
			if (anchor_link_end_x < (tags_stack[0].left + tags_stack[0].border_width + (tags_stack[0].padding_hoz << 1) + tags_stack[0].width - 1))
				anchor_link_end_x = (tags_stack[0].left + tags_stack[0].border_width + (tags_stack[0].padding_hoz << 1) + tags_stack[0].width - 1);
			if (anchor_link_end_y < (tags_stack[0].top + tags_stack[0].border_width + (tags_stack[0].padding_vert << 1) + tags_stack[0].height - 1))
				anchor_link_end_y = (tags_stack[0].top + tags_stack[0].border_width + (tags_stack[0].padding_vert << 1) + tags_stack[0].height - 1);
		#endif
	}
	current_style_background_colour = DISPLAY_COLOUR_NULL;		//Reset it
	
	//-----------------------------------------------------------------
	//----- MOVE TOP DOWN BELOW END OF TEXT IF TEXT WAS DISPLAYED -----
	//-----------------------------------------------------------------
	if (new_top_coord < 0xffff)
	{
		if (tags_stack[0].height)
			tags_stack[0].height = tags_stack[0].height - (new_top_coord - tags_stack[0].top);
		tags_stack[0].top = new_top_coord;
	}
}




//*********************************************
//*********************************************
//********** PROCESS ATTRIBUTE VALUE **********
//*********************************************
//*********************************************
//This is a sub function used by the display_html_file() function to process an attribute value as it is found.
//doing_html_tag		1 for HTML tag, 0 for style
//tags_stack			Ignored for style
//style_index			Ignored for HTML tag
void process_attribute_value (BYTE doing_html_tag, DISPLAY_HTML_TAGS_STACK *tags_stack, BYTE style_index, BYTE *style_attribute_name, BYTE *style_attribute_value)
{
	WORD pixels_value = 0xffff;
	DWORD colour_value = 0xffffffff;
	BYTE *p_buffer;
	BYTE data;
	WORD multiplier;
	
	//-----------------------------------
	//----- CONVERT NUMBERIC VALUES -----
	//-----------------------------------

	//----- IS THERE A 'PX' PIXELS VALUE ? -----
	p_buffer = find_string_in_string(style_attribute_value, STYLE_PARAMETER_VALUE_PX);
	if (p_buffer)
	{
		//Work back through digits before px
		pixels_value = 0;
		p_buffer--;
		multiplier = 1;
		
		while (p_buffer >= style_attribute_value)
		{
			data = (*p_buffer--) - 0x30;
			if (data > 9)
				break;
			pixels_value += (data * multiplier);
			multiplier *= 10;
		}
		
	}

	//----- IS THERE A '#' COLOUR VALUE? -----
	p_buffer = find_string_in_string(style_attribute_value, STYLE_PARAMETER_VALUE_HASH);
	if (p_buffer)
	{
		//Work forwards through the hex digits after the '#' character
		p_buffer++;
		
		colour_value = 0;
		
		while (*p_buffer != 0x00)
		{
			if (*p_buffer > '9')
				data = (*p_buffer - ('A' - 10));
			else
				data = (*p_buffer - '0');
			p_buffer++;
			
			if (data > 15)
				break;
			
			colour_value <<= 4;
			colour_value |= (DWORD)data;
		}
	}


	
	//-----------------------------
	//----- PROCESS ATTRIBUTE -----
	//-----------------------------

	if (find_string_in_string(style_attribute_name, STYLE_PARAMETER_BACKGROUND_COLOR))
	{
		//----- BACKGROUND COLOUR -----
		if (colour_value < 0xffffffff)
		{
			if (doing_html_tag)
				current_style_background_colour = current_style_background_colour_copy = colour_value;
			else
				html_styles[style_index].background_colour = colour_value;
		}
	}
	else if (find_string_in_string(style_attribute_name, STYLE_PARAMETER_COLOR))
	{
		//----- FOREGROUND COLOUR -----
		if (colour_value < 0xffffffff)
		{
			if (doing_html_tag)
				tags_stack[0].fore_colour = colour_value;
			else
				html_styles[style_index].fore_colour = colour_value;
		}
	}
	else if (find_string_in_string(style_attribute_name, STYLE_PARAMETER_BORDER_DASH))
	{
		//----- BORDER + ELEMENT -----
		if (find_string_in_string(style_attribute_name, STYLE_PARAMETER_DASH_COLOR))
		{
			//----- BORDER COLOUR -----
			if (colour_value < 0xffffffff)
			{
				if (doing_html_tag)
					current_style_border_colour = colour_value;
				else
					html_styles[style_index].border_colour = colour_value;
			}
		}
		else if (find_string_in_string(style_attribute_name, STYLE_PARAMETER_DASH_WIDTH))
		{
			//----- BORDER WIDTH -----
			if (pixels_value < 0xffff)
			{
				if (doing_html_tag)
					tags_stack[0].border_width = (BYTE)pixels_value;
				else
					html_styles[style_index].border_width = (BYTE)pixels_value;
			}
		}
	}
	else if (find_string_in_string(style_attribute_name, STYLE_PARAMETER_BORDER))
	{
		//----- BORDER IN 1 TAG -----

		//COLOUR
		if (colour_value < 0xffffffff)
		{
			if (doing_html_tag)
				current_style_border_colour = colour_value;
			else
				html_styles[style_index].border_colour = colour_value;
		}
		//WIDTH
		if (pixels_value < 0xffff)
		{
			if (doing_html_tag)
				tags_stack[0].border_width = (BYTE)pixels_value;
			else
				html_styles[style_index].border_width = (BYTE)pixels_value;
		}
	}
	else if (find_string_in_string(style_attribute_name, STYLE_PARAMETER_TEXT_ALIGN))
	{
		//----- TEXT ALIGN -----
		if (find_string_in_string(style_attribute_value, STYLE_PARAMETER_LEFT))
		{
			if (doing_html_tag)
				tags_stack[0].align = 0;
			else
				html_styles[style_index].align = 0;
		}
		else if (find_string_in_string(style_attribute_value, STYLE_PARAMETER_CENT))
		{
			if (doing_html_tag)
				tags_stack[0].align = 1;
			else
				html_styles[style_index].align = 1;
		}
		else if (find_string_in_string(style_attribute_value, STYLE_PARAMETER_RIGHT))
		{
			if (doing_html_tag)
				tags_stack[0].align = 2;
			else
				html_styles[style_index].align = 2;
		}

	}	
	else if (find_string_in_string(style_attribute_name, STYLE_PARAMETER_FONT_SIZE))
	{
		//----- FONT SIZE -----
		if (pixels_value < 0xffff)
		{
			if (doing_html_tag)
				tags_stack[0].font_size = (BYTE)pixels_value;
			else
				html_styles[style_index].font_size = (BYTE)pixels_value;
		}
	}
	else if (find_string_in_string(style_attribute_name, STYLE_PARAMETER_PADDING))
	{
		//----- PADDING -----
		if (pixels_value < 0xffff)
		{
			if (
			(find_string_in_string(style_attribute_name, STYLE_PARAMETER_DASH_TOP)) ||
			(find_string_in_string(style_attribute_name, STYLE_PARAMETER_DASH_BOTTOM))
			)
			{
				if (doing_html_tag)
					tags_stack[0].padding_vert = (BYTE)pixels_value;
				else
					html_styles[style_index].padding_vert = (BYTE)pixels_value;
			}
			else if (
			(find_string_in_string(style_attribute_name, STYLE_PARAMETER_DASH_LEFT)) ||
			(find_string_in_string(style_attribute_name, STYLE_PARAMETER_DASH_RIGHT))
			)
			{
				if (doing_html_tag)
					tags_stack[0].padding_hoz = (BYTE)pixels_value;
				else
					html_styles[style_index].padding_hoz = (BYTE)pixels_value;
			}
			else
			{
				if (doing_html_tag)
				{
					tags_stack[0].padding_hoz = (BYTE)pixels_value;
					tags_stack[0].padding_vert = (BYTE)pixels_value;
				}
				else
				{
					html_styles[style_index].padding_hoz = (BYTE)pixels_value;
					html_styles[style_index].padding_vert = (BYTE)pixels_value;
				}
			}
		}
	}
	else if (find_string_in_string(style_attribute_name, STYLE_PARAMETER_TOP))				//After other parameters that might contain this
	{
		//----- TOP -----
		if (pixels_value < 0xffff)
		{
			if (doing_html_tag)
				tags_stack[0].top = pixels_value;
			else
				html_styles[style_index].top = pixels_value;
		}
	}
	else if (find_string_in_string(style_attribute_name, STYLE_PARAMETER_LEFT))				//After other parameters that might contain this
	{
		//----- LEFT -----
		if (pixels_value < 0xffff)
		{
			if (doing_html_tag)
				tags_stack[0].left = pixels_value;
			else
				html_styles[style_index].left = pixels_value;
		}
	}
	else if (find_string_in_string(style_attribute_name, STYLE_PARAMETER_HEIGHT))				//After other parameters that might contain this
	{
		//----- HEIGHT -----
		if (pixels_value < 0xffff)
		{
			if (doing_html_tag)
				tags_stack[0].height = pixels_value;
			else
				html_styles[style_index].height = pixels_value;
		}
	}
	else if (find_string_in_string(style_attribute_name, STYLE_PARAMETER_WIDTH))				//After other parameters that might contain this
	{
		//----- WIDTH -----
		if (pixels_value < 0xffff)
		{
			if (doing_html_tag)
				tags_stack[0].width = pixels_value;
			else
				html_styles[style_index].width = pixels_value;
		}
	}
}




//*********************************************
//*********************************************
//********** LOOK FOR MATCHING STYLE **********
//*********************************************
//*********************************************
//This is a sub function used by the display_html_file() function to look to see if a style has been stored with a matching name
void display_html_look_for_matching_style (DISPLAY_HTML_TAGS_STACK *tags_stack, BYTE total_styles_count, BYTE *style_name)
{
	BYTE count;
	BYTE count1;
	
	for (count = 0; count < total_styles_count; count++)
	{
		for (count1 = 0; count1 < SYTLE_NAME_MAX_LENGTH; count1++)
		{
			if (style_name[count1] != html_styles[count].style_name[count1])
				break;
			
			if ((style_name[count1] == 0x00) && (html_styles[count].style_name[count1] == 0x00))
			{
				//-------------------------------------------
				//----- THIS CLASS NAME MATCHES A STYLE -----
				//-------------------------------------------
				if (html_styles[count].top != 0xffff)
					tags_stack[0].top = html_styles[count].top;
				if (html_styles[count].left != 0xffff)
					tags_stack[0].left = html_styles[count].left;
				if (html_styles[count].height != 0xffff)
					tags_stack[0].height = html_styles[count].height;
				if (html_styles[count].width != 0xffff)
					tags_stack[0].width = html_styles[count].width;
				if (html_styles[count].fore_colour != 0xffffffff)
					tags_stack[0].fore_colour = html_styles[count].fore_colour;
				if (html_styles[count].background_colour != 0xffffffff)
					current_style_background_colour = current_style_background_colour_copy = html_styles[count].background_colour;
				if (html_styles[count].border_colour != 0xffffffff)
					current_style_border_colour = html_styles[count].border_colour;
				if (html_styles[count].border_width != 0xff)
					tags_stack[0].border_width = html_styles[count].border_width;
				if (html_styles[count].align != 0xff)
					tags_stack[0].align = html_styles[count].align;
				if (html_styles[count].font_size != 0xff)
					tags_stack[0].font_size = html_styles[count].font_size;
				if (html_styles[count].padding_hoz != 0xff)
					tags_stack[0].padding_hoz = html_styles[count].padding_hoz;
				if (html_styles[count].padding_vert != 0xff)
					tags_stack[0].padding_vert = html_styles[count].padding_vert;

			} //if ((link_name_or_value[count] == 0x00) && ((html_styles[count].style_name[count1] == 0x00))
			
			if ((style_name[count1] == 0x00) || (html_styles[count].style_name[count1] == 0x00))
				break;
			
			
		} //for (count1 = 0; count1 < SYTLE_NAME_MAX_LENGTH; count1++)
	} //for (count = 0; count < (this_doc_styles_count + global_styles_count); count++)
}




//*********************************
//*********************************
//********** READ STYLES **********
//*********************************
//*********************************
//This function is used to read a global styles .css file and called by the display_html_file() function to read styles in the header of an HTML page being displayed.
//doing_global_styles		1 if this file is the global styles file or 0 if this file is an HTML file being displayed
void display_html_read_styles (DWORD *file_size, BYTE *this_doc_styles_count, BYTE doing_global_styles)
{
	BYTE data;
	BYTE previous_characters[6];
	BYTE character;
	WORD count;
	BYTE style_name[SYTLE_NAME_MAX_LENGTH];
	BYTE *p_style_name;
	BYTE style_attribute_name[MAX_STYLE_ATTRIBUTE_NAME_LENGTH];
	BYTE *p_style_attribute_name;
	BYTE style_attribute_value[MAX_STYLE_ATTRIBUTE_VALUE_LENGTH];
	BYTE *p_style_attribute_value;
	BYTE getting_style_state = STYLE_READING_NAME;
	BYTE this_style_index = 0;
	
	

	//This function is called for a global style sheet file, in which case the entire file is styles, and when styles are defined within and HTML
	//page, in which case we exit when we see the "</STYLE" closing tag.
	//We remove all whitespace and capitalise all characters
	//We parse through the file looking for the start of a new style, marked with "{"
	//The characters prior to this are the style name
	//Then we read through looking for the attribute name followed by the attribute value
	//The closing "}" marks the end of a style
	

	//RESET GLOBAL STYLES IF THIS IS THE GLOBAL STYLE SHEET WE ARE LOADING
	if (doing_global_styles)
		global_styles_count = 0;

	previous_characters[1] = 0x00;
	previous_characters[0] = 0x00;
	data = 0x00;
	p_style_name = &style_name[0];

	//Read each byte
	while ((*file_size)--)
	{
		//-------------------------
		//----- GET NEXT BYTE -----
		//-------------------------
		data = DISPLAY_HTML_READ_NEXT_BYTE;

		//Ignore non displayable characters (e.g. line breaks, etc)
		if (data < 0x20)
		{
			p_style_name = &style_name[0];			//Reset style name capture on line break
			continue;
		}

		//Discard all whitespace
		if ((data == ' ') || (data == '\t'))
			continue;

		//Convert all characters to uppercase
		if ((data >= 'a') && (data <= 'z'))
			data -= 0x20;


		//-----------------------------------
		//----- ADD CHARACTER TO BUFFER -----
		//-----------------------------------
		count = 5;
		do
		{
			previous_characters[count] = previous_characters[(count - 1)];
		} while (--count);
		previous_characters[0] = character;
		character = data;

		if ((previous_characters[5] == '<') && (previous_characters[4] == '/') && (previous_characters[3] == 'S') && (previous_characters[2] == 'T') && (previous_characters[1] == 'Y') && (previous_characters[0] == 'L') && (character == 'E'))
		{
			//------------------------------------
			//----- CLOSING STYLE TAG - EXIT -----
			//------------------------------------
			break;					//Next character will be ">"
		}

		if (character == '{')
		{
			//------------------------------------------
			//----- OPENING BRACKET OF A NEW STYLE -----
			//------------------------------------------
			if (
			(p_style_name == &style_name[0]) ||																									//Ignore if no name
			((*this_doc_styles_count + global_styles_count) >= MAX_STYLES_TO_STORE) ||															//Ignore if we have no more space for new styles
			((style_name[0] == 'B') && (style_name[1] == 'O') && (style_name[2] == 'D') && (style_name[3] == 'Y') && (style_name[4] == 0x00))	//Ignore body styles (body can be used to colour the area outside of our screen size with some different colour)
			)
			{
				getting_style_state = STYLE_READING_IGNORE;
			}
			else
			{
				//-------------------------------
				//----- THIS IS A NEW STYLE -----
				//-------------------------------
				if (doing_global_styles)
					global_styles_count++;
				else
					(*this_doc_styles_count)++;
				
				this_style_index = *this_doc_styles_count + global_styles_count - 1;

				//----- SETUP STYLE VALUES -----
				*p_style_name = 0x00;
				for (count = 0; count < SYTLE_NAME_MAX_LENGTH; count++)
					html_styles[this_style_index].style_name[count] = style_name[count];
				
				html_styles[this_style_index].top = 0xffff;
				html_styles[this_style_index].left = 0xffff;
				html_styles[this_style_index].height = 0xffff;
				html_styles[this_style_index].width = 0xffff;
				html_styles[this_style_index].fore_colour = 0xffffffff;
				html_styles[this_style_index].background_colour = 0xffffffff;
				html_styles[this_style_index].border_colour = 0xffffffff;
				html_styles[this_style_index].border_width = 0xff;
				html_styles[this_style_index].align = 0xff;
				html_styles[this_style_index].font_size = 0xff;
				html_styles[this_style_index].padding_hoz = 0xff;
				html_styles[this_style_index].padding_vert = 0xff;

				getting_style_state = STYLE_READING_ATTRIBUTE_NAME;
				p_style_attribute_name = &style_attribute_name[0];			//Reset ready for next name
			}

			continue;
		}

		if (character == ':')
		{
			//---------------------------------------
			//----- END OF STYLE ATTRIBUTE NAME -----
			//---------------------------------------
			if (getting_style_state == STYLE_READING_ATTRIBUTE_NAME)
			{
				*p_style_attribute_name = 0x00;			//Add terminating null
			}
			
			getting_style_state = STYLE_READING_ATTRIBUTE_VALUE;
			
			
			p_style_attribute_value = &style_attribute_value[0];			//Reset ready for next value
			continue;
		}

		if ((character == ';') || (character == '}'))
		{
			//----------------------------------------
			//----- END OF STYLE ATTRIBUTE VALUE -----
			//----------------------------------------
			if (getting_style_state == STYLE_READING_ATTRIBUTE_VALUE)
			{
				*p_style_attribute_value = 0x00;			//Add terminating null

				//Process the new attribute
				if (getting_style_state == STYLE_READING_ATTRIBUTE_VALUE)
					process_attribute_value(0, 0, this_style_index, &style_attribute_name[0], &style_attribute_value[0]);
			}
			
			getting_style_state = STYLE_READING_ATTRIBUTE_NAME;

			p_style_attribute_name = &style_attribute_name[0];			//Reset ready for next name

			//continue;		//Check for "}" below
		}

		if (character == '}')
		{
			//------------------------
			//----- END OF STYLE -----
			//------------------------
			getting_style_state = STYLE_READING_NAME;
			p_style_name = &style_name[0];
			continue;
		}
		
		
		//------------------------
		//------------------------
		//----- READING DATA -----
		//------------------------
		//------------------------
		if (getting_style_state == STYLE_READING_NAME)
		{
			//--------------------------------------------
			//----- GET NEXT CHARACTER OF STYLE NAME -----
			//--------------------------------------------
			if (character == '.')
				continue;
				
			if (p_style_name < (&style_name[0] + SYTLE_NAME_MAX_LENGTH - 1))		//-1 as last byte is null
				*p_style_name++ = character;
		
			continue;
		}

		if (getting_style_state == STYLE_READING_ATTRIBUTE_NAME)
		{
			//----------------------------------
			//----- GETTING ATTRIBUTE NAME -----
			//----------------------------------
			if ((character == ';') || (character == '.'))
				continue;

			if (p_style_attribute_name < &style_attribute_name[(MAX_STYLE_ATTRIBUTE_NAME_LENGTH - 2)])			//Allways leave space for terminating null
				*p_style_attribute_name++ = character;
		
			continue;
		}

		if (getting_style_state == STYLE_READING_ATTRIBUTE_VALUE)
		{
			//-----------------------------------
			//----- GETTING ATTRIBUTE VALUE -----
			//-----------------------------------
			if (character == ':')
				continue;

			if (p_style_attribute_value < &style_attribute_value[(MAX_STYLE_ATTRIBUTE_VALUE_LENGTH - 2)])		//Allways leave space for terminating null
				*p_style_attribute_value++ = character;
		
			continue;
		}





	} //while ((*file_size)--)
	if (doing_global_styles)
	{
		//----- CLOSE THE FILE -----
		#ifdef DISPLAY_HTML_CLOSE_READ
			DISPLAY_HTML_CLOSE_READ;
		#endif
	}
}







//********************************************************************
//********************************************************************
//********** FIND FIRST OCCURANCE OF STRING WITHIN A STRING **********
//********************************************************************
//********************************************************************
//Looks for the first occurance of a constant string within a variable string
//Returns character number of first character when found, or 0 if not found
BYTE *find_string_in_string (BYTE *examine_string, CONSTANT BYTE *looking_for_string)
{
	BYTE number_of_characters_matched = 0;

	while (*examine_string != 0x00)
	{
		//----- CHECK NEXT CHARACTER -----
		if (*examine_string++ == *looking_for_string)
		{
			//----- THIS CHARACTER MATCHES -----
			looking_for_string++;
			number_of_characters_matched++;
			
			if (*looking_for_string == 0x00)
			{
				//----- GOT TO NULL OF STRING TO FIND - SUCCESS - STRING FOUND -----
				return(examine_string - number_of_characters_matched);
			}
		}
		else if (number_of_characters_matched)
		{
			looking_for_string -= number_of_characters_matched;		//Return string being looked for pointer back to start - the match was not compelted
			number_of_characters_matched = 0;
		}	
	}
	
	//----- NOT FOUND -----
	return(0);

}



