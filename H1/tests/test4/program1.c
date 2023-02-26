#include <stdio.h>
#include "CAT.h"

int CAT_execution1 (void){
  CATData d1;
  CATData d2;
  CATData d3;
  d1  = CAT_new(5);
  d2  = CAT_new(8);
  d3  = CAT_new(0);
  CAT_add(d3, d1, d2);
  CAT_set(d1, 3);
  return CAT_get(d3);
}

int CAT_execution2 (void){
  CATData d;
  d  = CAT_new(5);
  return CAT_get(d);
}

int main (int argc, char *argv[]){
  return CAT_execution1() + CAT_execution2();
}
