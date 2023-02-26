#include<stdio.h>
#include <stdint.h>

int32_t e0 = 0;
int32_t e1 = 1;
int32_t e2 = 2;

int main (int32_t argc, char *argv[]){

  int32_t *p0 = &e0;
  int32_t *p1 = &e1;
  int32_t *p2 = &e2;
  int32_t *p3 = p2;

  printf("Result: %d\n", *p0 + *p1 + *p2 + *p3);

  return 0;
}

