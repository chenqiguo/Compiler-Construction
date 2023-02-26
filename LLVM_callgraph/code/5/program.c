#include <stdio.h>
#include <stdlib.h>

int q (void){
  return rand() % 10;
}

int p1 (void){
  return q();
}

int p2 (void){

  auto v = p1();
  if (v > 100000) v += p2();

  return v;
}

int p3 (void){
  return p2();
}

int main (int argc, char *argv[]){
  int t = p3();

  int r = p1();

  printf("%d %d\n", r, t);

	return 0;
}
