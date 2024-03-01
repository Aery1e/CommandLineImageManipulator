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
    if(!(strcmp(iname_file_extension,".sbu") || strcmp(iname_file_extension,".ppm"))){ //This code probably doesn't work, double check this
        return INPUT_FILE_MISSING;
    }
    else if(fopen(iname,"r") == NULL){
        return INPUT_FILE_MISSING;
    }
    else if(!(strcmp(oname_file_extension,".sbu") || strcmp(oname_file_extension,".ppm"))){ //This code probably doesn't work, double check this. strcmp returns 0 when equals.
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
    
    //Part 2
    //Open file to be read.
    FILE *working_file = fopen(iname, "r");
    //Read line 2 (same for both ppm and sbu) for dimensions. 
    unsigned int row = 0, column = 0;
    char buffer[100];
    fgets(buffer, sizeof(buffer), working_file); //Skip line 1
    fscanf(working_file, "%u %u", &column, &row);
    
    printf("Dimensions : %d %d\n", column, row);
    //Read line 3 if the file is .sbu (for the color table), and store it in an array
    unsigned int working_content_ppm_format[row*3][column*3];
    unsigned int working_content_current_row = 0;
    unsigned int working_content_current_column = 0; //Rows and column indices for the content array.
    if(!strcmp(iname_file_extension,".sbu")){ //Handling SBU Files
        //Export the contents of the 3rd line into a single line array. That'll be the color table.
        unsigned int color_table_length = 0;
        fscanf(working_file, "%u", &color_table_length);
        // printf("Color table length: %i\n", color_table_length);
        unsigned int color_table[color_table_length*3];
        unsigned int current_value = 69; 
        for(unsigned int current_color_table_index = 0; current_color_table_index < color_table_length*3 && (fscanf(working_file, "%u", &current_value) == 1); current_color_table_index ++ ){
            color_table[current_color_table_index] = current_value;
        }//Exports the color table into a 1D array.
            // for(unsigned int array_print_index; array_print_index < (color_table_length*3); array_print_index++){
            //     printf("%u ", color_table[array_print_index]);
            // }
            //Array Printer
        char content_sbu_star_checker[100]; //Used for reading each content element as a string, and looking for *
        // printf("\n\n");
        for(int content_array_index = 0; fscanf(working_file,"%s", content_sbu_star_checker) == 1; content_array_index++){
            int star_run_index = 0;
            unsigned int color_table_reference_value = 0;
            // printf("content array index: %i", content_array_index);
            if(*content_sbu_star_checker == '*'){
                // printf("Content sbu star checker: %s \n ", content_sbu_star_checker);
                int star_content = 0; //The number of repeats of the color table reference value.
                star_content = strtoul(content_sbu_star_checker+1, NULL, 10);
                // printf("Star content: %u \n", star_content);
                fscanf(working_file, "%u", &color_table_reference_value); //pulls the value right after the star value
                // printf("*%i ", star_content);
                // printf("%u ", color_table_reference_value);
                while(star_run_index < star_content){
                    working_content_ppm_format[working_content_current_row][working_content_current_column] = color_table[color_table_reference_value*3];
                    working_content_ppm_format[working_content_current_row][working_content_current_column+1] = color_table[color_table_reference_value*3 + 1];
                    working_content_ppm_format[working_content_current_row][working_content_current_column+2] = color_table[color_table_reference_value*3 + 2]; //writes the triple of RGB values from the color table.
                    star_run_index++;
                    working_content_current_column += 3; //moves over 3 spaces for the next triple
                    if(working_content_current_column >= column*3){ //Resets to the start of the next row of the 2D content array when reaching the end of a row.
                        working_content_current_column = 0;
                        working_content_current_row++;
                    }
                }
            }
            else{
                color_table_reference_value = strtoul(content_sbu_star_checker, NULL, 10); //pulls the value
                // printf("%u ", color_table_reference_value);
                working_content_ppm_format[working_content_current_row][working_content_current_column] = color_table[color_table_reference_value*3];
                working_content_ppm_format[working_content_current_row][working_content_current_column+1] = color_table[color_table_reference_value*3 + 1];
                working_content_ppm_format[working_content_current_row][working_content_current_column+2] = color_table[color_table_reference_value*3 + 2];
                working_content_current_column += 3;
                if(working_content_current_column > column*3){ //Resets to the start of the next row of the 2D content array when reaching the end of a row.
                    working_content_current_column = 0;
                    working_content_current_row++;
                }
            }
        }
    }
    else{ //working with .ppm file, just export all content from line 4 down to the end of the file into a double array
        fgets(buffer, sizeof(buffer), working_file); //Skip line 3
        unsigned int current_value = 69; 
        fscanf(working_file, "%u", &current_value); //Skips the 255 i guess?
        while(fscanf(working_file, "%u", &current_value) == 1){
            working_content_ppm_format[working_content_current_row][working_content_current_column] = current_value;
            working_content_current_column++;
            if(working_content_current_column > column*3){
                working_content_current_column = 0;
                working_content_current_row++;
            }
        }
    }
    //Create an 2d array with the dimensions from the previous step
    //Loop through the content starting from line 4 to the end of the file, store each line's content into an array, and that array into the column/row (2d array stuff)
    //Printing Stuff:
    // printf("\n\n CURRENT CONTENT:\n");
    // for(unsigned int ro = 0; ro < row; ro++){
    //     for(unsigned int col = 0; col < column*3; col++){
    //         printf("%u ", working_content_ppm_format[ro][col]);
    //     }
    // }
    // for(unsigned int col = 0; col < column*3; col++){
    //     printf("%u ", working_content_ppm_format[0][col]);
    // }
    // for(unsigned int col = 0; col < column*3; col++){
    //     printf("%u ", working_content_ppm_format[1][col]);
    // }
    //End of Printing Stuff 

    //For the rest of the functions, convert from .sbu to .ppm format, do the function, and convert back from .ppm to .sbu at the end for an output. 
    //This is to avoid having to manipulate values while using counters.
    
    fclose(working_file); //End of reading the file.

    //Exporting/Saving the file:
    FILE *output_file = fopen(oname,"w");
    working_content_current_row = 0;
    working_content_current_column = 0;
    if(!strcmp(iname_file_extension,".sbu")){//.SBU Output

    }
    else{ //.PPM Output
        fprintf(output_file,"P3\n");
        fprintf(output_file,"%u %u\n", column, row);
        fprintf(output_file,"255\n");
        while(working_content_current_row < row*3){
            fprintf(output_file,"%u ", working_content_ppm_format[working_content_current_row][working_content_current_column]);
            working_content_current_column++;
            if(working_content_current_column > column*3){
                working_content_current_column = 0;
                working_content_current_row++ ;
            }
        }
    }
    fclose(output_file);
    return 0;
    (void)argc;
    (void)argv;

    return 1000;
}
