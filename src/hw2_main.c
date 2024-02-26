#include "hw2.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h> 

int main(int argc, char **argv) {
    //Part 1 : Return Exceptions
    extern char *optarg; extern int optind;
    int c, err = 0; 
    char *iname, *oname;
    int iflag, oflag, cflag, pflag, rflag; 
    while ((c = getopt(argc, argv, "i:o:")) != -1) {
		switch (c) {
		case 'i':
            if(optarg == ""){
                printf("missing -i argument");
                return MISSING_ARGUMENT;
            }
            iflag = 1;
            iname = optarg;
			break;
		case 'o':
            if(optarg == ""){
                printf("missing -o argument");
                return MISSING_ARGUMENT;
            }
            oflag = 1;
            oname = optarg;
			break;
		case 'c':
			break;
		case 'p':
			break;
		case 'r':
			break;
		case '?':
			err = 1;
			break;
		}
    }
    (void)argc;
    (void)argv;

    return 1000;
}
