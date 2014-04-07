#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include "bool.h"

/* some math functions.
 * not fully tested. especially atof_sci*/

/*oct string to integer*/
int 
atoi_oct(const char *s){
	assert(s[0] == '0');
	int value = 0;
	s++;
	for (; *s; ++s){
		assert(*s >= '0' && *s <= '7');
		value *= 8;
		value += *s - '0';
	}
	return value;
}

/*hex string to integer*/
int 
atoi_hex(const char *s){
	assert(s[0] == '0');
	assert(s[1] == 'x' || s[1] == 'X');
	int value = 0;
	s += 2;
	for(; *s; ++s){ 
		value *= 16;
		if(*s >= '0' && *s <=  '9')
			value += *s - '0';
		else if(*s >= 'a' && *s <= 'f')
			value += *s - 'a' + 10;
		else if(*s >= 'A' && *s <= 'F')
			value += *s - 'A' + 10;
		else
			assert(0);
	}
	return value;
}

/*scientific notation string to float*/
float 
atof_sci(const char *s){
	int len = strlen(s);
	int e_pos = -1;	//the position of e\E
	int i;
	for(i = 0; s[i]; i++)
		if(s[i] == 'e' || s[i] == 'E')
			e_pos = i;
	assert(e_pos > 0 && e_pos < len - 1);

	/*get base and index substrings*/
	int base_len = e_pos;
	int index_len = len - e_pos - 1;
	char *base = (char *)malloc(base_len + 1);
	char *index = (char *)malloc(index_len + 1);
	strncpy(base, s, base_len);
	strncpy(index, s + e_pos + 1, index_len);
	base[base_len] = '\0';
	index[index_len] = '\0';

	/*get base value and index value*/
	float base_value = atof(base);
	int index_value = atoi(index);
	/*printf("%d %s\n", len, s);
	printf("%d %s\n", base_len, base);
	printf("%d %s\n", index_len, index);
	printf("%f\n", base_value);
	printf("%d\n", index_value);*/
	
	/*calculate*/
	return base_value * pow(10.0, (double)index_value); 
}

/*
int main(){
	printf("%f\n", atof_sci("00e+5"));
}*/