/*************************************************
Unblack Edges
Spencer Meldrum and Tim Alander

This program takes in as input a pbm file that has "black edges" from copying
errors.  The program then moves this data into a Bit array and cleans up all
black pixels on the edge as well as all "chains" of black pixels that are
connected to the edge.  It will print to stdout the modified pbm file without
any black edges.
*************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "seq.h"
#include "pnm.h"
#include "pnmrdr.h"
#include "assert.h"
#include "bit2.h"
//this struct is how we store the locations of the black bits we need to
//turn white in our stack.  As we pop them off, we turn their respective
//indexes in the bit vector from black to white and then call their
//neighbors
typedef struct {
  int row;
  int column;
} bit_loc;
/*************************************************
Function:load_bitmap
Arguments: A FILE pointer that points to the image that will be parsed.
Purpose: This function will load a pnm reader to parse the pbm, assert that
it is a pbm, and transfer all the bits to a newly allocated bit vector.  It
will return a pointer to that new vector
*************************************************/
Bit2_T load_bitmap(FILE* fp);
/*************************************************
Function: print_bitmap_values
Arguments: A pointer to our bit vector, width/height of the given value to be
printed, and a closure argument
Purpose: This is the apply function that map row major will call when we have
fixed all of the black edges in our bit vector.  It will print out bit by
bit what we have stored in our bit vector
*************************************************/
static void print_bitmap_values(Bit2_T bitmap, int width, int height, void* cl);
/*************************************************
Function: set_bitmap_values
Arguments: pointer to a bit vector, 2 ints: width and height of a given index 
in the bit vector, and a closure argument
Purpose: This apply function will be called by map row major to set all the
values of the pbm to the values in our bit vector.
*************************************************/
static void set_bitmap_values(Bit2_T bitmap, int width, int height, void* cl);
/*************************************************
Function:load_blackedge_sequence
Arguments: pointer to a bit vector
Purpose: This will line up our stack to perform all the necessary procedures
to remove black edges by pushing onto the stack all coordinates of edge bits 
that are black.  Therefore, when we pop off these edge coordinates, we will 
have a chain of events that will lead to the complete elimination of black edges
*************************************************/
Seq_T load_blackedge_sequence(Bit2_T bitmap);
/*************************************************
Function: remove_black
Arguments: pointer to a bit vector, pointer to a sequence
Purpose: This will be the function that pseudo-recursively pushes and pops
black bits from our bit vector to eliminate the edges.  This function will
always turn a black bit white when it is popped off, and then call
push_if_valid_loc to determine if any of its neiboring bits are also black
edges.
*************************************************/
void remove_black(Bit2_T bitmap, Seq_T stack);
/*************************************************
Function:push_if_valid_loc
Arguments:pointer to a bit vector, column/row of a specific index in the array
and a pointer to a sequence
Purpose: This function will first check if a column/row pair is a legitimate
value in the bitmap, and if so, it will check if it is a black edge that will
need to be pushed on the stack via a struct of coordinates representing where
it is held in the bit vector.
*************************************************/
void push_if_valid_loc(Bit2_T bitmap, int temp_col, int temp_row, Seq_T stack);
/*************************************************
Function:print_pbm_formatting
Arguments: none
Purpose: This is a function that prints the necessary pre-bit printing
prefixes of "P1" and the width/height of the pbm file.  If it does not
print this out first, it will not recognize the bits and thus not print out
certain pbms.
*************************************************/
void print_pbm_formatting();

int main(int argc, char *argv[]) {
  if (argc == 1) {
    Bit2_T bit_array=load_bitmap(stdin);
    Seq_T stack=load_blackedge_sequence(bit_array);
    remove_black(bit_array,stack);
    print_pbm_formatting(Bit2_width(bit_array), Bit2_height(bit_array));
    Bit2_map_row_major(bit_array, print_bitmap_values, NULL);
    Seq_free(&stack);
    Bit2_free(bit_array);
  } else {
    for (int i = 1; i < argc; i++) {
      FILE *fp = fopen(argv[i], "r");
      if (fp == NULL) {
        fprintf(stderr, "%s: Could not open file %s for reading\n",
        argv[0], argv[i]);
        exit(1);
      }
      Bit2_T bit_array=load_bitmap(fp);
      Seq_T stack=load_blackedge_sequence(bit_array);
      remove_black(bit_array,stack);
      print_pbm_formatting(Bit2_width(bit_array), Bit2_height(bit_array));
      Bit2_map_row_major(bit_array, print_bitmap_values, NULL);
      Seq_free(&stack);
      Bit2_free(bit_array);
    }
  }
  return 0;
}

Bit2_T load_bitmap(FILE* fp){
  Pnmrdr_T image=Pnmrdr_new(fp);
  Pnmrdr_mapdata data=Pnmrdr_data(image);
  assert(data.type==1);
  int width=data.width;
  int height=data.height;
  Bit2_T bitmap=Bit2_new(width, height);
  Bit2_map_row_major(bitmap, set_bitmap_values, image);
  Pnmrdr_free(&image);
  return bitmap;
}

static void set_bitmap_values(Bit2_T bitmap, int width, int height, void* cl){
  Pnmrdr_T image_file=(Pnmrdr_T)cl;
  unsigned temp=Pnmrdr_get(image_file);
  assert(temp<2);
  Bit2_put(bitmap, width, height, temp);
}

static void print_bitmap_values(Bit2_T bitmap, int width, int height, void *cl){
  (void)cl;
  int temp=Bit2_get(bitmap, width, height);
  printf("%i", temp);
  if(width+1==Bit2_width(bitmap)){
    printf("\n");
  }
}

Seq_T load_blackedge_sequence(Bit2_T bitmap){
  Seq_T temp=Seq_new(Bit2_size(bitmap));
  int width=Bit2_width(bitmap);
  int height=Bit2_height(bitmap);
  //these are the 4 edges of our bit vector.  Placing all of the black bits
  //on the stack makes it very easy to manipulate the stack and the touching
  //inner black edges 
  for(int i=0; i<width-1; i++){
    if(Bit2_get(bitmap, i, 0)==1){
      bit_loc* coords=malloc(sizeof(bit_loc));
      coords->row=0;
      coords->column=i;
      Seq_addhi(temp, coords);
    }
  }
  for(int i=0; i<height-1; i++){
    if(Bit2_get(bitmap, width-1, i)==1){
      bit_loc* coords=malloc(sizeof(bit_loc));
      coords->row=i;
      coords->column=width-1;
      Seq_addhi(temp, coords);
    }
  }
  for(int i=width-1; i>0; i--){
    if(Bit2_get(bitmap, i, height-1)==1){
      bit_loc* coords=malloc(sizeof(bit_loc));
      coords->row=height-1;
      coords->column=i;
      Seq_addhi(temp, coords);
    }
  }
  for(int i=height-1; i>0; i--){
    if(Bit2_get(bitmap, 0, i)==1){
      bit_loc* coords=malloc(sizeof(bit_loc));
      coords->row=i;
      coords->column=0;
      Seq_addhi(temp, coords);
    }
  }
  return temp;
}

void remove_black(Bit2_T bitmap, Seq_T stack){
  while(Seq_length(stack)!=0){
    bit_loc* temp_loc=(bit_loc*)Seq_remhi(stack);
    if(Bit2_get(bitmap, temp_loc->column, temp_loc->row)==0){
      free(temp_loc);
    } else {
      Bit2_put(bitmap, temp_loc->column, temp_loc->row, 0);
      //this now calls all the potential new black edges, tests to see if
      //they are valid, and then will push them onto the stack accordingly
      push_if_valid_loc(bitmap, temp_loc->column+1, temp_loc->row, stack);
      push_if_valid_loc(bitmap, temp_loc->column-1, temp_loc->row, stack);
      push_if_valid_loc(bitmap, temp_loc->column, temp_loc->row+1, stack);
      push_if_valid_loc(bitmap, temp_loc->column, temp_loc->row-1, stack);
      free(temp_loc);
    }
  }
}

void push_if_valid_loc(Bit2_T bitmap, int temp_col, int temp_row, Seq_T stack){
  if(temp_col>=0 && temp_col<Bit2_width(bitmap) && temp_row>=0 && 
    temp_row<Bit2_height(bitmap)){
    if(Bit2_get(bitmap, temp_col, temp_row)==1){
      bit_loc* coords=malloc(sizeof(bit_loc));
      coords->row=temp_row;
      coords->column=temp_col;
      Seq_addhi(stack, coords);
    }
  }
}

void print_pbm_formatting(int width, int height){
  printf("P1\n");
  printf("%i %i\n", width, height);
}
