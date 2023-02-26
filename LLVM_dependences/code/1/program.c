#include<stdio.h>
#include <stdint.h>

int32_t e0 = 0;
int32_t e1 = 1;
int32_t e2 = 2;

int32_t* const getPointer0 (void){
  return &e0;
}

int32_t* const getPointer1 (void){
  return &e1;
}

int32_t* const getPointer2 (void){
  return &e2;
}

int main (int32_t argc, char *argv[]){

  int32_t *p0 = getPointer0();
  int32_t *p1 = getPointer1();
  int32_t *p2 = getPointer2();

  printf("Result: %d\n", *p0 + *p1 + *p2);

  return 0;
}

