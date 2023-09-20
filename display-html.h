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
//HTML DRIVER C CODE HEADER FILE




//*****************************
//*****************************
//********** DEFINES **********
//*****************************
//*****************************
#ifndef DISPLAY_HTML_C_INIT		//(Do only once)
#define	DISPLAY_HTML_C_INIT


//------------------------------------
//----- RAM MEMORY USAGE DEFINES -----				//<<<<< CHECK FOR A NEW APPLICATION <<<<<
//------------------------------------
//Make these big enough for your requriements, but small enough so you have enough memory
#define	MAX_TEXT_BUFFER_LENGTH					1024	//Max length of a text string to be displayed in a single action (i.e. continuous paragraph of text).  Additional characters will be ignored

#define	MAX_TAGS_NESTING_LEVELS					5		//Max depth of style tags nesting we can accept
//#define	MAX_PARAMETER_NAME_LENGTH				10		//Max string length of a style parameter name
#define	MAX_STYLE_ATTRIBUTE_NAME_LENGTH			18		//Max string length of a style attribute name (including termianting null)
#define	MAX_STYLE_ATTRIBUTE_VALUE_LENGTH		18		//Max string length of a style attribute text value (including termianting null)
#define	SYTLE_NAME_MAX_LENGTH					11		//Max string length of a style name(Inc terminating null)
#define	MAX_STYLES_TO_STORE						40		//Maximum number of styles to store (global styles loaded from a CSS file + styles in an individual page).  Additional styles will be ignored.



//-------------------------------
//----- FILE ACCESS DEFINES -----				//<<<<< CHECK FOR A NEW APPLICATION <<<<<
//-------------------------------
//Set as required for your application
#ifdef DISPLAY_FILES_ARE_EXTERNAL		//(Set by the Display Files Bitmap Converter Application for Binary Output option)
	//----- YOU ARE STORING YOUR DISPLAY FILES BITMAP CONVERTER OUTPUT FILE IN EXTERNAL FLASH MEMORY -----
	//You need to provide access to the data
	//Reading HTML source files:
	#define	DISPLAY_HTML_SET_READ_ADDRESS(address)		p_display_html_current_address = address		//address is the address starting from 0 within the "display_files_bin.bin" output file.  Only used by the display_html_setup_read_file() function (not requried if you won't use this function)
	#define	DISPLAY_HTML_READ_NEXT_BYTE					some_get_byte_function()						//Called repeatedly after DISPLAY_HTML_SET_READ_ADDRESS
	//#define	DISPLAY_FILE_CLOSE_READ					some_function()									//Optional close file access define.  Comment out if not requried

	//Reading image files:
	#define	DISPLAY_HTML_OPEN_IMAGE_FILE(filename)		ffs_fopen((const char*)filename, (const char*)"r");	//Open requested file for reading using the DISPLAY_FILE_READ_NEXT_BYTE defined call.  Return 1 if found, 0 if not.
#else
	//----- YOU ARE STORING YOUR DISPLAY FILES BITMAP CONVERTER OUTPUT FILE WITHIN PROGRAM MEMORY -----
	//These defines can be left as is
	//Reading HTML source files:
	#define	DISPLAY_HTML_SET_READ_ADDRESS(address)		p_display_html_current_address = (uint8_t*)address		
	#define	DISPLAY_HTML_READ_NEXT_BYTE					(*p_display_html_current_address++)

	//Reading image files:
	#define	DISPLAY_HTML_OPEN_IMAGE_FILE(filename)		display_find_file(filename);
#endif



//-----------------
//----- FONTS -----				//<<<<< CHECK FOR A NEW APPLICATION <<<<<
//-----------------
//Fonts are selected from the font-size attribute.  The font used will be the nearest font available that is >= to the pixel size specified, starting from the smallest.
#ifdef DISPLAY_HTML_C
	//Add additional fonts to these defines as requried and update the total number define below
	const uint8_t html_font_sizes[] = {7, 11, 15, 22, 42};
	const uint8_t *html_fonts[] = {disp_font_5w_7h, disp_font_5w_11h, disp_font_15h, disp_font_22h, disp_font_42h};
#else
	extern const uint8_t html_font_sizes[];
	extern const uint8_t *html_fonts[];
#endif
#define HTML_TOTAL_NUMBER_OF_FONTS		5



//---------------------------------
//----- DYNAMIC TEXT FUNCTION -----				//<<<<< CHECK FOR A NEW APPLICATION <<<<<
//---------------------------------
//HTML pages may contain a special comment marker to include dynamic text.  If used this funtion needs to return each character of the dynamic text each time it is called, terminating
//in a null after the final character to be inserted.
//Comment out the define if not required
#define	HTML_GET_DYNAMIC_TEXT_FUNCTION(dynamic_text_index, dynamic_text_character_index)		html_get_dynamic_text_character(dynamic_text_index, dynamic_text_character_index)

//Define in your application as:
//uint8_t html_get_dynamic_text_character(uint8_t dynamic_text_index, uint8_t dynamic_text_character_index)
#ifdef HTML_GET_DYNAMIC_TEXT_FUNCTION
	extern uint8_t html_get_dynamic_text_character(uint8_t dynamic_text_index, uint8_t dynamic_text_character_index);
#endif


//---------------------------------
//----- ANCHOR LINKS FUNCTION -----				//<<<<< CHECK FOR A NEW APPLICATION <<<<<
//---------------------------------
//If your application uses a touchscreen this function can be implemented to be called for each <a href="somelink.htm> </a> tagged area of an HTML file as it is displayed.  Your
//applications touchscreen processing can then compare touches detected to areas of the html page which have been defined as linked and take the appropriate action, which could
//be as simple as triggering display of the filename passed
#define	HTML_PASS_ANCHOR_LINK_FUNCTION(href_value, start_x, start_y, end_x, end_y)		html_store_anchor_link_function(href_value, start_x, start_y, end_x, end_y)

//Define in your application as:
//void html_store_anchor_link_function(uint8_t *href_value, uint16_t start_x_coord, uint16_t start_y_coord, uint16_t end_x_coord, uint16_t end_y_coord)
//	Note that end_#_coord is guaranteed to be >= start_#_coord
#ifdef HTML_PASS_ANCHOR_LINK_FUNCTION
	extern void html_store_anchor_link_function(uint8_t *href_value, uint16_t start_x_coord, uint16_t start_y_coord, uint16_t end_x_coord, uint16_t end_y_coord);
#endif




//----- DATA TYPE DEFINITIONS -----
typedef struct _DISPLAY_HTML_STYLES
{
	uint16_t top;								//Absolute position value
	uint16_t left;								//Absolute position value
	uint16_t height;							//Absolute position value
	uint16_t width;								//Absolute position value
	uint32_t fore_colour;						//00|RR||GG||BB
	uint32_t background_colour;
	uint32_t border_colour;
	uint8_t style_name[SYTLE_NAME_MAX_LENGTH];	//Null termianted
	uint8_t border_width;
	uint8_t align;								//Text align, 0 = left, 1 = centre, 2 = right
	uint8_t font_size;								//Font numeric reference
	uint8_t padding_hoz;						//Where different left and right padding values are specified the largest value is used for both
	uint8_t padding_vert;						//Where different top and bottom padding values are specified the largest value is used for both
} DISPLAY_HTML_STYLES;
//If name length == 10 then there are 35 bytes user per style (36 in storage)


typedef struct _DISPLAY_HTML_TAGS_STACK
{
	uint16_t top;								//Absolute position value
	uint16_t left;								//Absolute position value
	uint16_t height;							//Absolute position value
	uint16_t width;								//Absolute position value
	uint32_t fore_colour;						//00|RR||GG||BB
	uint8_t border_width;
	uint8_t align;								//Text align, 0 = left, 1 = centre, 2 = right
	uint8_t font_size;							//Font size in pixels
	uint8_t padding_hoz;						//Where different left and right padding values are specified the largest value is used for both
	uint8_t padding_vert;						//Where different top and bottom padding values are specified the largest value is used for both
} DISPLAY_HTML_TAGS_STACK;


typedef enum _DISPLAY_HTML_INSIDE_TAG_STATE
{
    NOT_INSIDE_TAG,
    INSIDE_TAG_NULL,
    INSIDE_TAG_LOOKING_FOR_TYPE,			//Only processed the first "<" character so far
    INSIDE_TAG_IS_CLOSING_TAG,				//Tag starts with "</"
    INSIDE_TAG_IS_HEADING_TAG,				//This tag is an h1 to h6 tag
    INSIDE_TAG_IS_DIV_TAG,					//This tag is a div tag
    INSIDE_TAG_IS_IMG_TAG,					//This tag is an image tag


#ifdef HTML_INCLUDE_OPTION_SELECT				//>>>>>>>>>>>> REMOVE THIS SECTION! #####################################
    INSIDE_TAG_IS_SELECT_TAG,				//This tag is a select tag, the start of an option select box
    INSIDE_TAG_IS_OPTION_TAG,				//This tag is an option tag, for one of the options in an option select box
#endif											//<<<<<<<<<<<< REMOVE THIS SECTION! #####################################

#ifdef HTML_INCLUDE_TEXT_INPUT					//>>>>>>>>>>>> REMOVE THIS SECTION! #####################################
    INSIDE_TAG_IS_INPUT_TAG,				//This tag is an input tag
#endif											//<<<<<<<<<<<< REMOVE THIS SECTION! #####################################


    INSIDE_TAG_IS_ANCHOR_LINK_TAG,			//This tag is an anchor (link) tag
} DISPLAY_HTML_INSIDE_TAG_STATE;



typedef enum _DISPLAY_HTML_READING_PARAMETER_STATE
{
    PARAMETER_READING_NAME,
    PARAMETER_READING_SRC,
    PARAMETER_READING_ALT,
    PARAMETER_READING_CLASS,
    PARAMETER_READING_SELECT_SIZE,


#ifdef HTML_INCLUDE_OPTION_SELECT				//>>>>>>>>>>>> REMOVE THIS SECTION! #####################################
    PARAMETER_READING_OPTION_SELECTED,
#endif											//<<<<<<<<<<<< REMOVE THIS SECTION! #####################################

#ifdef HTML_INCLUDE_TEXT_INPUT					//>>>>>>>>>>>> REMOVE THIS SECTION! #####################################
    PARAMETER_READING_INPUT_TYPE,
    PARAMETER_READING_INPUT_VALUE,
    PARAMETER_READING_INPUT_MAX_LENGTH,
#endif											//<<<<<<<<<<<< REMOVE THIS SECTION! #####################################


    PARAMETER_READING_STYLE_ATTRIBUTE_NAME,
    PARAMETER_READING_STYLE_ATTRIBUTE_VALUE,
	PARAMETER_READING_HREF
} DISPLAY_HTML_READING_PARAMETER_STATE;

typedef enum _DISPLAY_HTML_READING_STYLE_STATE
{
    STYLE_READING_NAME,
    STYLE_READING_ATTRIBUTE_NAME,
    STYLE_READING_ATTRIBUTE_VALUE,
    STYLE_READING_IGNORE
} DISPLAY_HTML_READING_STYLE_STATE;


#endif //#ifndef DISPLAY_HTML_C_INIT








//*******************************
//*******************************
//********** FUNCTIONS **********
//*******************************
//*******************************
#ifdef DISPLAY_HTML_C
//-----------------------------------
//----- INTERNAL ONLY FUNCTIONS -----
//-----------------------------------
void display_html_push_stack_for_new_tag (DISPLAY_HTML_TAGS_STACK *tags_stack, uint8_t *current_depth, uint8_t *text_buffer_start, uint8_t *p_text_buffer);
void display_html_pop_stack_for_closing_tag (DISPLAY_HTML_TAGS_STACK *tags_stack, uint8_t *current_depth, uint8_t *text_buffer_start, uint8_t *p_text_buffer);
void display_any_pending_elements (DISPLAY_HTML_TAGS_STACK *tags_stack, uint8_t current_depth, uint8_t *text_buffer_start, uint8_t *p_text_buffer);
void process_attribute_value (uint8_t doing_html_tag, DISPLAY_HTML_TAGS_STACK *tags_stack, uint8_t style_index, uint8_t *style_attribute_name, uint8_t *style_attribute_value);
void display_html_look_for_matching_style (DISPLAY_HTML_TAGS_STACK *tags_stack, uint8_t total_styles_count, uint8_t *style_name);
uint8_t *find_string_in_string (uint8_t *examine_string, const uint8_t *looking_for_string);


//-----------------------------------------
//----- INTERNAL & EXTERNAL FUNCTIONS -----
//-----------------------------------------
//(Also defined below as extern)
uint8_t display_html_setup_read_file (const uint8_t *p_file_pointer, uint8_t *search_for_filename, uint32_t *file_size);
void display_html_read_styles (uint32_t *file_size, uint8_t *this_doc_styles_count, uint8_t doing_global_styles);
void display_html_file (uint32_t file_size);


#else
//------------------------------
//----- EXTERNAL FUNCTIONS -----
//------------------------------
extern uint8_t display_html_setup_read_file (const uint8_t *p_file_pointer, uint8_t *search_for_filename, uint32_t *file_size);
extern void display_html_read_styles (uint32_t *file_size, uint8_t *this_doc_styles_count, uint8_t doing_global_styles);
extern void display_html_file (uint32_t file_size);


#endif




//****************************
//****************************
//********** MEMORY **********
//****************************
//****************************
#ifdef DISPLAY_HTML_C
//--------------------------------------------
//----- INTERNAL ONLY MEMORY DEFINITIONS -----
//--------------------------------------------
DISPLAY_HTML_STYLES html_styles[MAX_STYLES_TO_STORE];
uint32_t current_style_background_colour;
uint32_t current_style_background_colour_copy;
uint32_t current_style_border_colour;
uint8_t global_styles_count;

#ifdef HTML_PASS_ANCHOR_LINK_FUNCTION				//Don't do if we're not storing links
	uint8_t anchor_get_next_start_coord;
	uint16_t anchor_link_start_x;
	uint16_t anchor_link_start_y;
	uint16_t anchor_link_end_x;
	uint16_t anchor_link_end_y;
#endif
	
#ifdef DISPLAY_FILES_ARE_EXTERNAL
	uint32_t display_html_current_address;
#else
	const uint8_t *p_display_html_current_address;
#endif

//--------------------------------------------------
//----- INTERNAL & EXTERNAL MEMORY DEFINITIONS -----
//--------------------------------------------------
//(Also defined below as extern)



#else
//---------------------------------------
//----- EXTERNAL MEMORY DEFINITIONS -----
//---------------------------------------




#endif


//----- CONTENT STRINGS -----
#ifdef DISPLAY_HTML_C
const uint8_t STYLE_PARAMETER_VALUE_HASH[] = {"#"};
const uint8_t STYLE_PARAMETER_VALUE_PX[] = {"PX"};
const uint8_t STYLE_PARAMETER_TOP[] = {"TOP"};
const uint8_t STYLE_PARAMETER_LEFT[] = {"LEFT"};
const uint8_t STYLE_PARAMETER_HEIGHT[] = {"HEIGHT"};
const uint8_t STYLE_PARAMETER_WIDTH[] = {"WIDTH"};
const uint8_t STYLE_PARAMETER_BACKGROUND_COLOR[] = {"BACKGROUND-COLOR"};
const uint8_t STYLE_PARAMETER_COLOR[] = {"COLOR"};
const uint8_t STYLE_PARAMETER_BORDER[] = {"BORDER"};
const uint8_t STYLE_PARAMETER_BORDER_DASH[] = {"BORDER-"};
const uint8_t STYLE_PARAMETER_DASH_WIDTH[] = {"-WIDTH"};
const uint8_t STYLE_PARAMETER_DASH_COLOR[] = {"-COLOR"};
const uint8_t STYLE_PARAMETER_TEXT_ALIGN[] = {"TEXT-ALIGN"};
const uint8_t STYLE_PARAMETER_CENT[] = {"CENT"};			//CENTER or CENTRE
const uint8_t STYLE_PARAMETER_RIGHT[] = {"RIGHT"};
const uint8_t STYLE_PARAMETER_FONT_SIZE[] = {"FONT-SIZE"};
const uint8_t STYLE_PARAMETER_PADDING[] = {"PADDING"};
const uint8_t STYLE_PARAMETER_DASH_TOP[] = {"-TOP"};
const uint8_t STYLE_PARAMETER_DASH_BOTTOM[] = {"-BOTTOM"};
const uint8_t STYLE_PARAMETER_DASH_LEFT[] = {"-LEFT"};
const uint8_t STYLE_PARAMETER_DASH_RIGHT[] = {"-RIGHT"};
#endif


