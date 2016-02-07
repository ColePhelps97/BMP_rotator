#include <stdio.h>
#include <stdlib.h>
#include "image.h"

int main(int argc, char** argv) {
	if(argc == 1) rotate(argv[1]);
	else rotate("image.bmp");
	return 0;
}
