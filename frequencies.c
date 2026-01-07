#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "frequencies.h"

//Open a file at path and either store the character frequencies in freq or set *a_error to strerror(errno)

bool calc_frequencies(Frequencies freqs, const char* path, const char** a_error){
	FILE* file = fopen(path, "r");
	if(file == NULL){  //checks for invalid
		*a_error = strerror(errno); //error message
		return false;
	}
	//else{
	for(uchar charrb = fgetc(file); !feof(file); charrb = fgetc(file)){
		freqs[charrb] += 1; //store the character frequencies in freqs
	}
	//fclose(file);
	//}
	//fclose(file);
	//return true;
	return !fclose(file);
}
