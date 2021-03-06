#include <stdlib.h>
#include <stdio.h>
#include "mem.h"
#include "assert.h"
#include "bit2.h"

struct Bit2_T{
  Bit_T Linear_Array;
  int height; //will be set by Bit2_new
  int width; //will be set by Bit2_new
};

Bit2_T Bit2_new(int init_width, int init_height){
  Bit2_T newBitArray = NEW(newBitArray);
  newBitArray->Linear_Array=Bit_new(init_width*init_height);
  newBitArray->height=init_height;
  newBitArray->width=init_width;
  return newBitArray;
}
int Bit2_get(Bit2_T t, int width, int height){
  int temp_width=t->width;
  return Bit_get(t->Linear_Array, height*temp_width+width);
}

int Bit2_put(Bit2_T t, int width, int height, int bit){
  int temp_width=t->width;
  //this is our formula to reach an index in a 1-d array via a width and height
  int temp_value=Bit_put(t->Linear_Array, height*temp_width+width, bit);
  return temp_value;
}


int Bit2_height(Bit2_T t){
  assert(t!=NULL);
  return t->height;
}

int Bit2_width(Bit2_T t){
  assert(t!=NULL);
  return t->width;
}

int Bit2_size(Bit2_T t){
  assert(t!=NULL);
  return t->height*t->width;
}

void Bit2_map_column_major(Bit2_T t, void apply(Bit2_T t, int width, int height,
   void* cl), void *cl){
  for(int i=0; i<Bit2_width(t); i++){
    for(int k=0;k<Bit2_height(t); k++){
      apply(t, i, k, cl);
    }
  }
}

void Bit2_map_row_major(Bit2_T t,void apply(Bit2_T t, int width, int height, 
  void* cl), void *cl){
  for(int k=0; k<Bit2_height(t); k++) {
    for(int i=0; i<Bit2_width(t); i++){
      apply(t, i, k, cl);
    }
  }
}

void Bit2_free(Bit2_T t){
  Bit_free(&t->Linear_Array);
  free(t);
}

