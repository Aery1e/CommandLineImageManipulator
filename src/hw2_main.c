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
    int iflag = 0; 
    int oflag = 0;
    int cflag = 0; 
    int pflag = 0;
    int rflag = 0; 
    while ((c = getopt(argc, argv, "i:o:")) != -1) {
		switch (c) {
		case 'i':
            if(optarg[0] == '-') return MISSING_ARGUMENT;
            if(iflag == 1){
                return DUPLICATE_ARGUMENT;
            }
            iflag = 1;
            iname = optarg;
            printf("iname = %s\n", iname);
			break;
		case 'o':
            if(optarg[0] == '-') return MISSING_ARGUMENT;
            if(oflag == 1){
                return DUPLICATE_ARGUMENT;
            }
            oflag = 1;
            oname = optarg;
            printf("oname = %s\n", oname);
			break;
		case 'c':
            if(optarg[0] == '-') return MISSING_ARGUMENT;
			break;
            if(cflag == 1){
                return DUPLICATE_ARGUMENT;
            }
            cflag = 1;
            break;
		case 'p':
            if(optarg[0] == '-') return MISSING_ARGUMENT;
            if(pflag == 1){
                return DUPLICATE_ARGUMENT;
            }
            pflag = 1;
			break;
		case 'r':
            if(optarg[0] == '-') return MISSING_ARGUMENT;
            if(rflag == 1){
                return DUPLICATE_ARGUMENT;
            }
            rflag = 1;
			break;
		case '?':
			err = 1;
			break;
		}
        if(err == 1){
            return UNRECOGNIZED_ARGUMENT;
        }
    }
    (void)argc;
    (void)argv;

    return 1000;
}
