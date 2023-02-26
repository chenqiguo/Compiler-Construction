#include<stdio.h>
#include <stdint.h>

int32_t e0 = 0;

int main (int32_t argc, char *argv[]){
  int32_t *p0 = &e0;

  if (argc > 10){
    *p0 = 1;
  } else {
    *p0 = 2;
  }
  
  return *p0;
}

