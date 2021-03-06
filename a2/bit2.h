/*************************************************
Bit 2-D Array
Spencer Meldrum and Tim Alander

This data structure stores a "2-D Array" of bits.
We do this by assigning each coordinate pair (column, row) an index
within a linear array.  The underlying inferface that we have utilized
is the bit vector included in Hanson's C Libraries.
*************************************************/

#ifndef BIT2_T_INCLUDED
#define BIT2_T_INCLUDED

#include "bit.h"

typedef struct Bit2_T *Bit2_T;

/*************************************************
Function: Bit2_new
Arguments: Two integers that represent the width and height 
of the bit array that will be allocated in this function
Purpose:Will allocate memory for a new bit vector, and will
return a pointer to the newly allocated memory block
*************************************************/

Bit2_T Bit2_new(int init_width, int init_height);

/*************************************************
Function: Bit2_put
Arguments: A pointer to the bit vector, the width and height of the
spot on our 2-d plane that we want to change, the bit that we want
to replace the current bit with
Purpose: This function will assign a new bit to a position in our
vector designated by the width/height, and return the value that
it was holding formerly
*************************************************/

int Bit2_put(Bit2_T t, int width, int height, int bit);

/*************************************************
Function: Bit2_get
Arguments: A pointer to the bit vector, the width and height of the
spot on our 2-d plane that we want to receive the value of
Purpose: This function will look into our Bit array at the index that is
designated by the width and height arguments, and will return the bit value
(0 or 1) that is stored in that spot
*************************************************/

int Bit2_get(Bit2_T t, int width, int height);

/*************************************************
Function: Bit2_height
Arguments: A pointer to the bit vector
Purpose: This will return the "height" of the bit vector that we have
created.  The user will give Bit2_new info about how large the vector
will be and new will set this value for easy reference
*************************************************/

int Bit2_height(Bit2_T t);

/*************************************************
Function: Bit2_width
Arguments: A pointer to the bit vector
Purpose: This will return the "width" of the bit vector that we have
created.  The user will give Bit2_new info about how large the vector
will be and new will set this value for easy reference
*************************************************/

int Bit2_width(Bit2_T t);

/*************************************************
Function: Bit2_size
Arguments: A pointer to the bit vector
Purpose: This will return the "width*height" of the bit vector that we have
created.  The user will give Bit2_new info about how large the vector
will be and new will set the values involved in this calculation for easy 
reference
*************************************************/

int Bit2_size(Bit2_T t);

/*************************************************
Function: Bit2_map_column_major
Arguments: A pointer to the bit vector, an apply function, and a closure 
argument
Purpose: this function will call an apply function on every element in the 
provided Bit Vector. Row indices will vary more quickly than column indices.


*************************************************/

void Bit2_map_column_major(Bit2_T t, void apply(Bit2_T t, int width, int height,
 void* cl), void *cl);

/*************************************************
Function: Bit2_map_row_major
Arguments: A pointer to the bit vector, an apply function, and a closure 
argument
Purpose: this function will call an apply function on every element in the 
provided Bit Vector. Row indices will vary more quickly than column indices.
*************************************************/

void Bit2_map_row_major(Bit2_T t,void apply(Bit2_T t, int width, int height, 
void* cl), void *cl);

/*************************************************
Function: Bit2_free
Arguments: A pointer to the bit vector
Purpose: This function deallocates/frees the entire bit vector that this
pointer is pointing to, and also the pointer
*************************************************/

void Bit2_free(Bit2_T t);

#endif
