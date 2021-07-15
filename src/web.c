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

#include "web.h"

/////////////////////////////////////////////////////////////////////////////////////
//Implementation below
/////////////////////////////////////////////////////////////////////////////////////

char** web_get_query_string(){

	char *qs = getenv("QUERY_STRING");
	if(!qs)
		return NULL;
	if(tec_string_length(qs) > WEB_MAX_QUERY_STRING){
		return NULL;
	}

	int i = 0;
	int num = 0;
	char **qss = tec_string_split(qs, '&', &num);

	while(i <= num){
		w_process_query_string(qss[i]);
		i += 1;
	}

	return qss;

}//web_get_query_string*/

char** web_get_post_string(){

	char *cont_len = getenv("CONTENT_LENGTH");
	if(!cont_len){
		return NULL;
	}
	int len = tec_string_to_int(cont_len);
	if(len > WEB_MAX_POST_DATA){
		return NULL;
	}
	len += 1;

	char *buffer = (char *) malloc(sizeof(char) * len );
	fread(buffer, sizeof(char), len, stdin);

	int i = 0;
	int num = 0;
	char **qss = tec_string_split(buffer, '&', &num);
	while(i <= num){
		w_process_query_string(qss[i]);
		i += 1;
	}

	return qss;

}//web_get_post_string*/

char* web_get_from_query_string(char **qss, char *var_name){

	if(!qss)
		return NULL;

	int i = 0;
	while(qss[i]){
		if(tec_buf_begins(qss[i], var_name)){
			char *str = qss[i];
			str += tec_string_length(var_name);
			str += 1;       //skip over '='
			return str;
		}
		i += 1;
	}

	return NULL;

}//web_get_from_query_string*/

int tec_buf_begins(char *buffer, char *str){

	if(!buffer)
		return 0;
	if(!str)
		return 0;

	while(*str && *buffer == *str){
		str += 1;
		buffer += 1;
	}

	if(*str){
		return 0;
	}

	return 1;

}//tec_buf_begins*/

int tec_string_length(char *str){

	//TODO(GI): go through string one WORD at a time and check for empty characters in the word

	if(!str)
		return 0;
	if(!*str)
		return 0;

	int len = 0;
	while(*str){
		len += 1;
		str += 1;
	}

	return len;

}//tec_string_length*/

int tec_string_find_char(char *s, char to_find){

	if(!s)
		return -1;

	int i = 0;
	while(s[i] && to_find != s[i]){
		i += 1;
	}

	if(s[i]){
		return i;
	}else{
		return -1;
	}

}//tec_string_find_char*/

void tec_string_shift(char *str){

	if(!str)
		return;

	int len = tec_string_length(str);
	int i = 1;
	int j = 1;

	if( (str[i-j] & TEC_MOST_SIG_BIT) && (str[i-j] & TEC_2ND_MOST_SIG_BIT) ){
		j += 1;
		i += 1;
		while( (str[i] & TEC_MOST_SIG_BIT) && !(str[i] & TEC_2ND_MOST_SIG_BIT) ){
			j += 1;
			i += 1;
		}
	}

	while(i < len){
		str[ i - j ] = str[i];
		i += 1;
	}
	str[i - j] = '\0';

}//tec_string_shift*/

char** tec_string_split(char *str, char c, int *num){

	if(!str)
		return NULL;
	if(!c)
		return NULL;

	// we start by assuming that there will be no more than 32 instances of c
	int original_limit = 32;
	int limit = original_limit;
	char **table[26];
	int table_index = 0;
	int tmp_num = 0;
	*num = 0;

	char **res = (char **) malloc(sizeof(char *) * limit);
	if(!res){
		return NULL;
	}
	table[table_index] = res;

	res[0] = str;
	while(*str){
		if(*str == c){
			tmp_num += 1;
			*num += 1;
			if(tmp_num == limit){
				limit *= 2;
				table_index += 1;
				res = (char **) malloc(sizeof(char *) * limit);
				if(!res){
					return NULL;
				}
				table[table_index] = res;
				tmp_num = 0;
			}
			*str = '\0';
			str += 1;
			if(*str){
				res[tmp_num] = str;
			}else{
				//Note(GI) this deals with case where char c is last character in the string
				res[tmp_num] = NULL;
			}
		}else{
			str += 1;
		}
	}

	if(*num < original_limit){
		res[(*num)+1] = NULL;
		return res;
	}

	char **real_res = (char **) malloc(sizeof(char *) * (*num + 2));
	int ti = 0;
	int n = 0;
	int n2 = 0;
	limit = original_limit;
	while(ti <= table_index){

		res = table[ti];
		n2 = 0;
		while(n2 < limit && n <= *num){
			real_res[n] = res[n2];
			n2 += 1;
			n += 1;
		}

		free(res);
		limit *= 2;
		ti += 1;

	}
	real_res[(*num) + 1] = NULL;

	return real_res;

}//tec_string_split*/

int tec_string_to_int(char *s){

	if(!s)
		return 0;

	int sign = 1;
	int result = 0;

	while(tec_char_is_white_space(*s)){
		s += 1;
	}
	if(*s == '-'){
		sign = -1;
		s += 1;
	}else{
		if(*s == '+'){
			s += 1;
		}
	}

	while(*s){
		if(*s > '9' || *s < '0'){
			return result * sign;
		}
		result *= 10;
		result += *s - '0';
		s += 1;
	}

	return result * sign;

}//tec_string_to_int*/

int tec_char_is_white_space(char c){

	if(c == 32 || c == 9 || c == 10 || c == 11 || c == 12 || c == 13)
		return 1;
	return 0;

}//tec_char_is_white_space*/

void w_process_query_string(char *qs){

	//first replacing a '+' with a space
	char *tmp = qs;
	int n = tec_string_find_char(tmp, '+');
	while( n != -1 ){
		tmp += n;
		*tmp = ' ';
		n = tec_string_find_char(tmp, '+');
	}

	int i = 0;
	tmp = qs;
	while(qs[i]){
		while(qs[i] && qs[i] != '%'){
			i += 1;
		}
		if(qs[i] == '%'){
			tmp = &(qs[i]);
			int hex = 0;
			i += 1;
			int j = 1;
			while(j >= 0){
				if(qs[i] >= 'A' && qs[i] <= 'F'){
					hex += (qs[i] - 'A') + 10;
				}
				if(qs[i] >= '0' && qs[i] <= '9'){
					hex += (qs[i] - '0');
				}
				if(j){
					hex *= 16;
				}
				j -= 1;
				i += 1;
			}
			tmp[0] = hex;
			tmp += 1;
			tec_string_shift(tmp);
			tec_string_shift(tmp);
			i -= 2;
		}
	}

}//w_process_query_string*/
