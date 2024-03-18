A command-line program that reads an image file from disk, performs image manipulation operations given as command-line arguments, and writes the modified image back to disk.
Able to import and export `.ppm` and `.sbu` files (a format which stores pixel colors in a color table, followed by data which are indices to that color table)

Implements File I/O and Image Manipulation.
 
Supported command line arguments:

  - `-i` : Path to input file
    - Ex. `-i "/home/testing/moon.ppm"`
    - Load the `moon.ppm` image file stored in the `/home/testing` directory.
   
  - `-o` : Path to output file
    - Ex. `-o "/home/testing/sun.sbu"`
    - Save the processed image in a file named `sun.sbu` in the local `/home/testing` directory.
   
  - `-c` : Rectangular region to copy pixels from
    - Ex.  `-c 5,8,20,30`
    - Select the region starting at the pixel at row 5, column 8 that is 20 columns wide and 30 rows high. The lower-right corner of this region would be at the pixel at row 34, column 27.
   
    - requires four parameters separated by commas: `row`, `col`, `width` and `height`:
      - `row`: the row number of the upper-left pixel of the region (0-based indexing)
      - `col`: the column number of the upper-left pixel of the region (0-based indexing)
      - `width`: the width of the entire region, in pixels (i.e., the number of columns spanned)
      - `height`: the height of the entire region, in pixels (i.e., the number of rows spanned)
  
  - `-p`: Paste the copied contents into the image
    - Ex. `-p 19,50`
    - Paste the contents of the copied region starting at row 19, column 50, overwriting the pixels at the region covered by the pasted content.
    - requires a `row` and `col` value.
      - `row`: the row number of the upper-left pixel of the image where the copied region’s upper-left corner should be positioned for pasting (0-based indexing)
      - `col`: the column number of the upper-left pixel of the image where the copied region’s upper-left corner should be positioned for pasting (0-based indexing)
  
  - `-r`: Print a message on top of the image
    - Ex. `-r "new YORK state","fonts/font3.txt",1,10,5`
    - Prints a message on top of the image in white (RGB = (255,255,255)), with the upper-left corner of the first letter’s bounding box positioned at the given row and column in the image.
    - requires five parameters separated by commas: `row`, `col`, `width` and `height`
      - `message`: the message to display, given in double quotation marks.
      - `path_to_font`: the path to the file containing the font file, given in double quotation marks.
      - `font_size`: the integer multiplier used to increase the size of the text horizontally and vertically
      - `row`: the row number of the upper-left pixel of the image where the upper-left corner of the first letter’s bounding box should be positioned when printing the message into the image
      - `col`: the column number of the upper-left pixel of the image where the upper-left corner of the first letter’s bounding box should be positioned when printing the message into the image 
