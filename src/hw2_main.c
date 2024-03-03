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
    unsigned int working_content_ppm_format[row][column*3];
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
                    if(working_content_current_column > column*3 - 1){ //Resets to the start of the next row of the 2D content array when reaching the end of a row.
                        working_content_current_column = 0;
                        working_content_current_row++;
                    }
                }
            }
            else{ //no star value saving
                color_table_reference_value = strtoul(content_sbu_star_checker, NULL, 10); //pulls the value
                // printf("%u ", color_table_reference_value);
                working_content_ppm_format[working_content_current_row][working_content_current_column] = color_table[color_table_reference_value*3];
                working_content_ppm_format[working_content_current_row][working_content_current_column+1] = color_table[color_table_reference_value*3 + 1];
                working_content_ppm_format[working_content_current_row][working_content_current_column+2] = color_table[color_table_reference_value*3 + 2];
                working_content_current_column += 3;
                if(working_content_current_column > column*3 - 1){ //Resets to the start of the next row of the 2D content array when reaching the end of a row.
                    working_content_current_column = 0;
                    working_content_current_row++;
                }
            }
        }
    }
    else{ //working with .ppm file, just export all content from line 4 down to the end of the file into a double array
        //fgets(buffer, sizeof(buffer), working_file); //Skip line 3
        unsigned int current_value = 69; 
        fscanf(working_file, "%u", &current_value); //Skips the 255 i guess?
        while((fscanf(working_file, "%u", &current_value) == 1) && working_content_current_row < row){
            working_content_ppm_format[working_content_current_row][working_content_current_column] = current_value;
            // printf("wrote %u ", current_value);
            // printf("Writing %u at Row %u, Column %u \n", current_value, working_content_current_row,working_content_current_column);
            working_content_current_column++;
            if(working_content_current_column > column*3 - 1){
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
    //     printf("Current Row: %u\n", ro);
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
    
    //Copying:
    if(cflag == 1){
        //copy stuff from the thing to a new 2d array, store it
        char *ctoken;
        ctoken = strtok(cname, ",");
        int c_row = atoi(ctoken);
        ctoken = strtok(NULL, ",");
        int c_col = atoi(ctoken);
        ctoken = strtok(NULL, ",");
        int c_width = atoi(ctoken);
        ctoken = strtok(NULL, ",");
        int c_height = atoi(ctoken);
        int c_width_max = 0;
        if((int)column >= c_col + c_width){
            c_width_max = c_width*3; //*3 bc RGB triplets
        }
        else{
            c_width_max = ((int)column - c_col)*3;
        }
        int c_height_max = 0; //Max number of row entries to save
        if((int) row >= c_row + c_height){
            c_height_max = c_height;
        }
        else{
            c_height_max = (int)row - c_row;
        }

        //printf("%i %i %i %i ", c_row, c_col, c_width, c_height);
        unsigned int c_array[c_height_max][c_width_max];

        printf("\nc_array Dimensions: %i %i\n",  c_width_max,c_height_max);
        // exit(1);
        int working_c_row_index = c_row; //starting row to copy from
        int working_c_col_index = c_col*3; //starting col to copy from
        int c_array_row_index = 0; //indices of the c array.
        int c_array_col_index = 0;
        while(c_array_row_index < c_height_max){
            while(c_array_col_index < c_width_max){
                c_array[c_array_row_index][c_array_col_index] = working_content_ppm_format[working_c_row_index][working_c_col_index];
                //printf("%u ", working_content_ppm_format[working_c_row_index][working_c_col_index]);
                working_c_col_index++;
                c_array_col_index++;
            }
            c_array_col_index = 0; //reset to the start of the next row of c_array
            working_c_col_index = c_col*3; //reset to the start of the next row of the working array
            c_array_row_index++; //move to the next row of c_array
            working_c_row_index++; //move to the next row of the working array
        }
        if(pflag == 1){
            char *ptoken;
            ptoken = strtok(pname, ",");
            int p_row = atoi(ptoken);
            ptoken = strtok(NULL, ",");
            int p_col = atoi(ptoken);
            int p_col_index = p_col*3; //index in triples col to start from
            int p_row_index = p_row; //index in row to start from
            int c_array_pasting_col_index = 0;
            int c_array_pasting_row_index = 0;
            int p_height_max = 0;
            if((int)row - p_row > c_height_max){
                p_height_max = c_height_max;
            }
            else{
                p_height_max = (int)row - p_row;
            }
            int p_width_max = 0;
            if(((int)column - p_col)*3 > c_width_max){
                p_width_max = c_width_max; //c_width_max is already in triples, no need for *3
            }
            else{
                p_width_max = ((int)column - p_col)*3;
            }
            // printf("p_height_max = %i", p_height_max);
            while(p_row_index < p_row + p_height_max){
                while(p_col_index < p_col*3 + p_width_max){
                    working_content_ppm_format[p_row_index][p_col_index] = c_array[c_array_pasting_row_index][c_array_pasting_col_index];
                    p_col_index++;
                    c_array_pasting_col_index++;
                }
                p_col_index = p_col*3;
                c_array_pasting_col_index = 0;
                p_row_index++;
                c_array_pasting_row_index++;
                // printf("p_row index: %i \n", p_row_index);
            }
        }
        // if(c_col + c_width - (int) column > 0){
        //     c_width = (int)column - c_col + 1;
        // }
        // if(c_row + c_height - (int) row > 0){
        //     c_height = (int)row - c_row + 1;
        // }

        // int **c_array = (int **)malloc(c_height * sizeof(int *));
        // for (int i = 0; i < c_height; i++) {
        //     c_array[i] = (int *)malloc(c_width * sizeof(int));
        // }
        // int c_current_row = 0;
        // int c_current_col = 0;
        
        // for(int c_row_index = c_row; c_row_index < c_row + c_height; c_row_index++){
        //     for(int c_col_index = c_col*3; c_col_index < (c_col + c_width)*3; c_col_index++){
        //         c_array[c_current_row][c_current_col] = working_content_ppm_format[c_row_index][c_col_index];
        //         // printf("%u ", c_array[c_current_row][c_current_col]);
        //         c_current_col++;
        //     }
        //     //printf("\n\n");
        //     c_current_row++;
        // }
        

        //Pasting:
        //printf("\n\n");
        // if(pflag == 1){
        //     char *ptoken;
        //     ptoken = strtok(pname, ",");
        //     int p_row = atoi(ptoken);
        //     ptoken = strtok(NULL, ",");
        //     int p_col = atoi(ptoken);
        //     int p_current_row = 0;
        //     int p_current_column = 0;
        //     for(int p_row_index = p_row; p_row_index < p_row + c_height && p_row_index < (int)row ; p_row_index++){
        //         for(int p_col_index = p_col*3; p_col_index < (p_col + c_width)*3 && p_col_index < (int)column*3; p_col_index++){ //This might throw seg fault by like a single pixel. do check <= vs <.
        //             working_content_ppm_format[p_row_index][p_col_index] = c_array[p_current_row][p_current_column];
        //             // printf("%u ", working_content_ppm_format[p_row_index][p_col_index]);
        //             p_current_column++;
        //         }
        //         // printf("\n\n");
        //         p_current_row++;
        //     }
        // }
    }
    
    //Writing with font:

    fclose(working_file); //End of reading the file.

    //Exporting/Saving the file:
    FILE *output_file = fopen(oname,"w");
    working_content_current_row = 0;
    working_content_current_column = 0;
    if(!strcmp(oname_file_extension,".sbu")){//.SBU Output
        fprintf(output_file,"SBU\n");
        fprintf(output_file,"%u %u\n", column, row);
        //You are basically going to build a color table while iterating through the content array. Read each triple as 3 seperate variables,
        //and compare that to each triple in the color array. If there is a match, then move to the next. If there isn't then add the triple at the color array and move on.
        unsigned int color_table_array[column*row*3]; //Safest allocation of memory is probably to assume each pixel has a unique color.
        unsigned int rgb_value1 = 333;
        unsigned int rgb_value2 = 333;
        unsigned int rgb_value3 = 333;
        int matching_rgb = 0; //Sets to 1 only when the RGB triplet finds a match in the color table
        int matching_rgb_zero_case = 0; //Sets to 1 when first encountering 0 0 0.
        int color_table_appending_index = 0;
        int number_of_color_table_elements = 0;
        while(working_content_current_row < row){ //This loop handles extracting all unique colors to the color table 1D array.
            rgb_value1 = working_content_ppm_format[working_content_current_row][working_content_current_column];
            rgb_value2 = working_content_ppm_format[working_content_current_row][working_content_current_column + 1];
            rgb_value3 = working_content_ppm_format[working_content_current_row][working_content_current_column + 2];
            // printf("Reading %u %u %u ", rgb_value1,rgb_value2,rgb_value3);
            for(unsigned int color_table_array_index = 0; color_table_array_index < column*row*3 - 1; color_table_array_index += 3){ //Checks every triple!
                if(rgb_value1 == 0 && rgb_value2 == 0 && rgb_value3 == 0 && matching_rgb_zero_case == 0){
                    matching_rgb_zero_case = 1;
                    break;
                }
                if(rgb_value1 == color_table_array[color_table_array_index] 
                && rgb_value2 == color_table_array[color_table_array_index + 1] 
                && rgb_value3 == color_table_array[color_table_array_index + 2]){
                    matching_rgb = 1;
                    // if(color_table_appending_index > 83571 && color_table_appending_index < 83580) {
                    //     printf("CTa: %u %u %u\n", color_table_array[color_table_array_index], color_table_array[color_table_array_index+1], color_table_array[color_table_array_index+2]);
                    //     printf("RGB: %u %u %u\n", rgb_value1,rgb_value2,rgb_value3);
                    //     printf("Found at index: %i\n", color_table_array_index);
                    // }
                    // printf(" | Skipped\n");
                    break; //if there's a match of the colors, don't do anything and stop checking the table
                }
                if(0 == color_table_array[color_table_array_index] 
                && 0 == color_table_array[color_table_array_index + 1] 
                && 0 == color_table_array[color_table_array_index + 2]
                && 0 == color_table_array[color_table_array_index + 3] 
                && 0 == color_table_array[color_table_array_index + 4] 
                && 0 == color_table_array[color_table_array_index + 5]) break; //End of the currently written color table. Break to save on time reading a bunch of 0 triplets and such.
            }//reads through the current color table built and checks for matches
            if(matching_rgb_zero_case == 1){
                color_table_appending_index += 3; //if it's the first encounter of 0 0 0, 'skip' to the next triple index to store the 0s.
                matching_rgb_zero_case = 2; //Set it to 2 and never check for it again. This set of 0 0 0 is only to be stored once.
                // printf("Added 0 0 0. to index %u\n",working_content_current_column);
                number_of_color_table_elements++;
            }
            else if(matching_rgb == 0){
                color_table_array[color_table_appending_index] = rgb_value1;
                color_table_array[color_table_appending_index + 1] = rgb_value2;
                color_table_array[color_table_appending_index + 2] = rgb_value3;
                //printf("Wrote %u %u %u to Color Table index %i\n", rgb_value1,rgb_value2,rgb_value3,color_table_appending_index);
                // if(color_table_appending_index > 83571 && color_table_appending_index < 83580) printf("Wrote %u %u %u to Color Table index %i\n", rgb_value1,rgb_value2,rgb_value3,color_table_appending_index);
                color_table_appending_index += 3;
                number_of_color_table_elements++;
            }
            matching_rgb = 0;
            working_content_current_column += 3;
            if(working_content_current_column > column*3 - 1){ //Remember to sync up the save and load checks!
                working_content_current_column = 0;
                working_content_current_row++ ;
            }
            
        }
        fprintf(output_file, "%i\n", number_of_color_table_elements);
        for(int color_table_printing_index = 0; color_table_printing_index < number_of_color_table_elements*3; color_table_printing_index++){
            // printf("Color table printing index: %i\n", color_table_printing_index);
            // if(color_table_printing_index == 179960) printf("Color table data at index 179961+: %u \n", color_table_array[color_table_printing_index]);
            fprintf(output_file, "%u ", color_table_array[color_table_printing_index]);
        }
        // for(unsigned int i = 0; i < row*column*3 ; i++){
        //     printf("%u ", color_table_array[i]);
        // }
        // printf("numb: %i",number_of_color_table_elements*3);
        // printf("Color table data at index 179961,2,3: %u %u %u", color_table_array[179961], color_table_array[179962], color_table_array[179963]);
        //To construct the content from the color table, compare each triple to the color table, search for the corresponding index, and insert that index. 
        //The star value can either be handled along with this, or another pass can be done to handle the repeats to create star values afterwards.
        //Count the number of times the triplet appears until you hit a unique one (prob need a leading and a lagging pointer for this), add 1 to a counter until you hit the differing one
        //then paste '*'counter, and the index of the colortable right after. In the case that counter == 1 (as in, the pixel only appears once), just fprintf the index of colortable.
        working_content_current_column = 0;
        working_content_current_row = 0;
        int current_color_table_index = 0;
        int star_repeat_value_counter = 1; 
        fprintf(output_file, "\n"); //New line for the content
        while(working_content_current_row < row){
            rgb_value1 = working_content_ppm_format[working_content_current_row][working_content_current_column];
            rgb_value2 = working_content_ppm_format[working_content_current_row][working_content_current_column + 1];
            rgb_value3 = working_content_ppm_format[working_content_current_row][working_content_current_column + 2];
            // printf("Reading %u %u %u ", rgb_value1,rgb_value2,rgb_value3);
            for(unsigned int color_table_array_index = 0; color_table_array_index < column*row*3 - 1; color_table_array_index += 3){ //Checks every triple!
                if(rgb_value1 == color_table_array[color_table_array_index] 
                && rgb_value2 == color_table_array[color_table_array_index + 1] 
                && rgb_value3 == color_table_array[color_table_array_index + 2]){
                    current_color_table_index = color_table_array_index;
                    break; //Find the correspond triplet index, then save it at that variable.
                }
                if(0 == color_table_array[color_table_array_index] 
                && 0 == color_table_array[color_table_array_index + 1] 
                && 0 == color_table_array[color_table_array_index + 2]
                && 0 == color_table_array[color_table_array_index + 3] 
                && 0 == color_table_array[color_table_array_index + 4] 
                && 0 == color_table_array[color_table_array_index + 5]) break; //End of the currently written color table. Break to save on time reading a bunch of 0 triplets and such.
            }
            if(working_content_current_column + 3 > column*3 - 1 && working_content_current_row + 1 < row){ //Checks for duplicate triplet in the next row (if we have to wrap around)
                if(rgb_value1 == working_content_ppm_format[working_content_current_row + 1][0] 
                && rgb_value2 == working_content_ppm_format[working_content_current_row + 1][1] 
                && rgb_value3 == working_content_ppm_format[working_content_current_row + 1][2]){
                    star_repeat_value_counter++;
                }
                else{
                    if(star_repeat_value_counter == 1) fprintf(output_file,"%i ", current_color_table_index/3);
                    else{
                        fprintf(output_file,"*%i ", star_repeat_value_counter);
                        fprintf(output_file,"%i ", current_color_table_index/3);
                    }
                    star_repeat_value_counter = 1;
                }
            }
            else{//else, checks for the duplicates in the next row
                if(rgb_value1 == working_content_ppm_format[working_content_current_row][working_content_current_column + 3] 
                && rgb_value2 == working_content_ppm_format[working_content_current_row][working_content_current_column + 4] 
                && rgb_value3 == working_content_ppm_format[working_content_current_row][working_content_current_column + 5]){
                    star_repeat_value_counter++;
                }
                else{
                    if(star_repeat_value_counter == 1) fprintf(output_file,"%i ", current_color_table_index/3);
                    else{
                        fprintf(output_file,"*%i ", star_repeat_value_counter);
                        fprintf(output_file,"%i ", current_color_table_index/3);
                    }
                    star_repeat_value_counter = 1;
                }
            }
            //Handling interation:
            working_content_current_column += 3;
            if(working_content_current_column > column*3 - 1){ //Remember to sync up the save and load checks!
                working_content_current_column = 0;
                working_content_current_row++ ;
            }
        }
    }
    else{ //.PPM Output
        fprintf(output_file,"P3\n");
        fprintf(output_file,"%u %u\n", column, row);
        fprintf(output_file,"255\n");
        while(working_content_current_row < row){
            fprintf(output_file,"%u ", working_content_ppm_format[working_content_current_row][working_content_current_column]);
            // if(working_content_current_row%2 == 0) fprintf(output_file,"255 ");
            // else fprintf(output_file,"0 ");
            // printf("Wrote %u", working_content_ppm_format[working_content_current_row][working_content_current_column]);
            working_content_current_column++;
            if(working_content_current_column > column*3 - 1){ //Remember to sync up the save and load checks!
                working_content_current_column = 0;
                fprintf(output_file, "\n");
                working_content_current_row++ ;
            }
        }
       // printf("%u", working_content_ppm_format[0][0]);
    }
    fclose(output_file);
    return 0;
    (void)argc;
    (void)argv;

    return 1000;
}
