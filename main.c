#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include "image.h"

struct command_args_t {
	int side;				/* "-l" - left, "-r" - right */ 
	unsigned long number_of_rotations;	/* "-n number" - number of rotations */
	char **input_files;
	int num_input_files;
	char *output_file;			/* "-o file" - output file */
} arguments;
	
static const char *opt_string = "lrn:o:h?";
	
int usage(void) {
	printf("Check man page : \"man rotate\"\n");
	return 0;
}
int main(int argc, char** argv) {
	size_t iter;
	int option = 0;
	char* stdo = "result.bmp";
 
	if(argc == 1) return usage();
	arguments.side = 0;
	arguments.number_of_rotations = 1;
	arguments.input_files = NULL;
	arguments.num_input_files = 0;
	arguments.output_file = stdo;
	
	while((option = getopt(argc, argv, opt_string)) != -1) {
		switch(option) {
			case 'r': 
				arguments.side = RIGHT;
				break;
			
			case 'l': 
				arguments.side = LEFT;
				break;
			
			case 'n':
				arguments.number_of_rotations = strtoul(optarg, NULL, 10);
				break;

			case 'o':
				arguments.output_file = optarg;
				break;
			
			case 'h':
			case '?':
				return usage();
				break;	
				
		}

	}
	
	arguments.input_files = argv + optind;
	arguments.num_input_files = argc - optind;
	

	for(iter = 0; iter < arguments.num_input_files; iter++) {
			rotate(arguments.input_files[iter], arguments.side, arguments.number_of_rotations, arguments.output_file);
	}

	return 0;
}
