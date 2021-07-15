// https://gitlab.com/greggink/youtube_episode_cgi.git
//
// Copyright (c) 2020 Gregg Ink
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef WEB_H
#define WEB_H

#include "stdio.h"
#include "stdlib.h"

#define WEB_MAX_QUERY_STRING 1024
//maximum permissible size of post data is 10 MB;
//TODO(GI): What is the right maximum size to use?
#define WEB_MAX_POST_DATA 10485760

#define TEC_MOST_SIG_BIT 128
#define TEC_2ND_MOST_SIG_BIT 64

#define web_print_header() printf("Content-Type: text/html; charset=utf-8\r\n\r\n")

#define web_print_relocate_header(X) printf("Status:308 Permanent Redirect\nLocation: %s\r\n\r\n", X)

/*web_get_query_string:
	a query string typically looks something like this:
	var1=abc&var2=def&var3=1%2B1
	this function will retrieve the query string
	it will split the query string where there is a '&'
	it will convert the escaped symbols e.g. %2B -> '+'
	this does modify the original string, make a copy if needed
*/
char** web_get_query_string();

/*web_get_post_string:
	this does the same thing as web_get_query_string
	but gets data sent with POST method
*/
char** web_get_post_string();

/*web_get_from_query_string:
	returns a string which contains the value of variable var_name
	this requires you first used web_get_query_string()
	returns NULL if var_name was not found
	char *qss: this is the output of web_get_query_string
	char *var_name: name of variable you are seeking the value of
*/
char* web_get_from_query_string(char **qss, char *var_name);

/*tec_buf_begins:
	returns 1 if the buffer begins with string str
	returns 0 in all other cases, including errors and str being longer than buffer
*/
int tec_buf_begins(char *buffer, char *str);

/*tec_string_length:
	returns the length of a string in bytes
	check tec_string_utf8_length to know the number of codepoints
	unlike strlen, this function does not segfault when you give it a NULL pointer
	instead it returns zero because, well, you gave it an empty string ;-)
*/
int tec_string_length(char *str);

/*tec_string_find_char:
	returns index of first instance of a character in a string
	returns -1 if not found
*/
int tec_string_find_char(char *s, char to_find);

/*tec_string_shift:
	removes an ascii char or unicode codepoint at front of string
	assumes a valid utf8 string
*/
void tec_string_shift(char *str);

/*tec_string_split:

	splits a string where char c does occur
	num will give you the number of times char c did occur in the string
	num is also index of last string in returned char**
	CAUTION: there is one more string than occurences of char c, this might include empty strings

	the string str will be modified so make sure to make a copy if needed

	note that some character pointers may be NULL pointers
	if 2 or more characters c are right next to each other

	free the returned char ** when done

	returns NULL in case of error
*/
char** tec_string_split(char *str, char c, int *num);

/*tec_string_to_int:
	converts a string to an integer
	string may not contain anything in front of number except '-' or '+'
	does not safeguard against overflow
*/
int tec_string_to_int(char *s);

/*tec_char_is_white_space:
	returns 1 if c is a white space character (e.g. space)
	returns 0 otherwise
	assumes 7 bit ascii character
	there are more white space characters within unicode
	they are not so commonly used and could not all be considered in just an 8 bit character
*/
int			tec_char_is_white_space(char c);

void w_process_query_string(char *qs);

#endif