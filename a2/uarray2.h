/***********************************************
Unboxed 2-D Array (UArray2_T)
Spencer Meldrum and Tim Alander

This data structure is an array capable of storing unboxed elements
(meaning actual data elements, not simply pointers to data elements).
Each element stored in the array is given an x and y coordinate within
the interface. (these x and y coordinates are refered to as columns and rows
in this implementation)

For example, an element stored in xy coordinate 3,2 would be stored in the
third column, two rows down.

Our interface is an abstraction built upon UArray_T which is provided in
Hanson's C Libraries.
***********************************************/

#ifndef UARRAY2_T_INCLUDED
#define UARRAY2_T_INCLUDED

#include "uarray.h"

typedef struct UArray2_T *UArray2_T;

/***********************************************
Function: UArray2_new
Arguments: 3 integers: columns and rows (when multiplied) represent the total
number of elements that can be stored in the created array. The size integer
represents the size in bytes that a data element will occupy.
Purpose: This function creates a UArray2_T  of the user specified number of
columns and rows, and returns a pointer to the array.
***********************************************/
UArray2_T UArray2_new(int columns, int rows, int size);


/***********************************************
Function: UArray2_at
Arguments: -A pointer to the array on which the function will act.
- column and row indices that correspond to a data element within the array.
Purpose: This function returns a pointer to a data element that is stored at the
column and row indices supplied, even if there is no data stored there at the
time of the function call.
***********************************************/
void* UArray2_at(UArray2_T t, int column, int row);


/***********************************************
Function: UArray2_Rows
Arguments: A pointer to a UArray2_T
Purpose: This function returns an integer corresponding to the number of rows
in the given array. It is a c.r.e for the pointer to be NULL.
***********************************************/
int UArray2_Rows(UArray2_T t);


/***********************************************
Function: UArray2_Columns
Arguments: A pointer to a UArray2_T
Purpose: This function returns an integer corresponding to the number of columns
in the given array. It is a c.r.e for the pointer to be NULL.
***********************************************/
int UArray2_Columns(UArray2_T t);


/***********************************************
Function: UArray2_length
Arguments: A pointer to a UArray2_T
Purpose: This function returns an integer corresponding to total number of 
elements in the given array. It is a c.r.e for the pointer to be NULL.
***********************************************/
int UArray2_length(UArray2_T t);


/***********************************************
Function: UArray2_map_column_major
Arguments: -A pointer to a UArray_T
-An apply function
-A pointer to a closure element
Purpose: This function calls the apply function provided on every element in the
array provided. The row indices will vary more quickly than the column indices.
***********************************************/
void UArray2_map_column_major(UArray2_T t, void apply(void* element, void* cl),
void *cl);


/***********************************************
Function: UArray2_map_row_major
Arguments: -A pointer to a UArray_T
-An apply function
-A pointer to a closure element
Purpose: This function calls the apply function provided on every element in the
array provided. The column indices will vary more quickly than the row indices.
***********************************************/
void UArray2_map_row_major(UArray2_T t, void apply(void* element, void* cl),
void *cl);

/***********************************************
Function: UArray2_free
Arguments: -A pointer to a UArray_T
Purpose: This function frees all memory allocated by UArray2_new.
***********************************************/
void UArray2_free(UArray2_T t);

#endif
