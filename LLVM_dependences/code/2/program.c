#include<stdio.h>
#include <stdint.h>

int32_t* getPointer0 (int32_t *p1, int32_t *p2){
  return p1;
}

int32_t* getPointer1 (int32_t *p1, int32_t *p2){
  return p2;
}

int main (int32_t argc, char *argv[]){
  int32_t e1, e2;

  e1 = e2 = 0;
  int32_t *p0 = getPointer0(&e1, &e2);
  int32_t *p1 = getPointer1(&e1, &e2);

  printf("Result: %d\n", *p0 + *p1);

  return 0;
}

