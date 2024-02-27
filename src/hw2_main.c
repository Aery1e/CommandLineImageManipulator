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
    char *iname, *oname, *cname = NULL, *pname = NULL, *rname = NULL;
    int iflag = 0; 
    int oflag = 0;
    int cflag = 0; 
    int pflag = 0;
    int rflag = 0; 
    while ((c = getopt(argc, argv, "i:o:c:p:r:")) != -1) {
		switch (c) {
		case 'i':
            if(optarg[0] == '-') iflag = -9999;
            iname = optarg;
            iflag += 1;
            printf("iname = %s\n", iname);
			break;
		case 'o':
            if(optarg[0] == '-') oflag = -9999;
            oname = optarg;
            oflag += 1;
            printf("oname = %s\n", oname);
			break;
		case 'c':
            if(optarg[0] == '-') cflag = -9999;
            cname = optarg;
            cflag += 1;
            break;
		case 'p':
            if(optarg[0] == '-') pflag = -9999;
            pname = optarg;
            pflag += 1;
			break;
		case 'r':
            if(optarg[0] == '-') rflag = -9999;
            rname = optarg;
            rflag += 1;
			break;
        case ':':
            err = 1;
            break;    
		case '?':
            // printf("optind = %d\nargc  = %d", optind,argc);
            if(optind >= argc){
                iflag = -9999;
            }
			err = 1;
			break;
		}
        
    }
    if(cflag < 0 || pflag < 0 || rflag < 0 || iflag <= 0 || oflag <= 0) {
        printf("\n\nRETURNING MISSING ARGUMENT\n\n");
        return MISSING_ARGUMENT; }
    else if(err == 1) {
        printf("\n err = %d \n cflag = %d \n pflag = %d \n rflag = %d \n iflag = %d \n oflag = %d", err,cflag,pflag,rflag,iflag,oflag);
        printf("Returning UNRECOGNIZED ARGUMENT");
        return UNRECOGNIZED_ARGUMENT;
    }
    else if(iflag > 1 || oflag > 1 || cflag > 1 || pflag > 1 || rflag > 1) {
        printf("\n\nunrecognized something\n\n");
        return DUPLICATE_ARGUMENT;
    }
    char *iname_file_extension = &iname[strlen(iname) - 4];
    char *oname_file_extension = &oname[strlen(oname) - 4];
    // printf("oname writabiility : %d", fopen(oname,"w") == NULL);
    // printf("iname file extension: %s", iname_file_extension);
    // printf("Existence of file: %d\n", fopen(iname,"r") != NULL);
    // FILE *fpr = fopen(iname,"r");
    // FILE *fpw = fopen(oname,"w");
    if(!(strcmp(iname_file_extension,".sbu") || strcmp(iname_file_extension,".ppm"))){
        return INPUT_FILE_MISSING;
    }
    else if(fopen(iname,"r") == NULL){
        return INPUT_FILE_MISSING;
    }
    else if(!(strcmp(oname_file_extension,".sbu") || strcmp(oname_file_extension,".ppm"))){
        return OUTPUT_FILE_UNWRITABLE;
    }
    else if(fopen(oname,"w") == NULL){
        // fclose(fpw);
        return OUTPUT_FILE_UNWRITABLE;
    }
    else if(pflag > 0 && cflag < 1){
        return C_ARGUMENT_MISSING;
    }
    int cnumberofcommas = 0;
    if(cname != NULL){
        // printf("cname: %s\n",cname);
        // int length_of_cname = strlen(cname);
        // printf("Length of strlen(cname) : %d\n", length_of_cname);
        for(int i = 0; i < (int) strlen(cname); i++){
            if(cname[i] == ',') cnumberofcommas++;
        }
        int cname_end = cname[strlen(cname) - 1];
        if(cnumberofcommas != 3 || cname_end == ','){
            return C_ARGUMENT_INVALID;
        }
    }
    int pnumberofcommas = 0;
    if(pname != NULL){
        // printf("cname: %s\n",cname);
        // int length_of_cname = strlen(cname);
        // printf("Length of strlen(cname) : %d\n", length_of_cname);
        for(int i = 0; i < (int) strlen(pname); i++){
            if(pname[i] == ',') pnumberofcommas++;
        }
        int pname_end = pname[strlen(pname) - 1];
        if(pnumberofcommas != 1 || pname_end == ','){
            return P_ARGUMENT_INVALID;
        }
    }
    int rnumberofcommas = 0;
    if(rname != NULL){
        // printf("cname: %s\n",cname);
        // int length_of_cname = strlen(cname);
        // printf("Length of strlen(cname) : %d\n", length_of_cname);
        for(int i = 0; i < (int) strlen(rname); i++){
            if(rname[i] == ',') rnumberofcommas++;
        }
        int rname_end = rname[strlen(rname) - 1];
        if(rnumberofcommas != 4 || rname_end == ','){
            return R_ARGUMENT_INVALID;
        }
        char *token;
        token = strtok(rname, ",");
        token = strtok(NULL, ",");
        printf("1: %s\n", token);
        if(!fopen(token,"r")){
            return R_ARGUMENT_INVALID;
        } //this is never closed... uh oh
    }
    // fclose(fpr);
    // fclose(fpw);
    // printf("Number of commas: %d", numberofcommas);
    // printf("cname ending: %s",cname_ending_comma_check);
    // printf("strcmp result: %d",strcmp(cname_ending_comma_check,','));
    
    return 0;
    (void)argc;
    (void)argv;

    return 1000;
}
