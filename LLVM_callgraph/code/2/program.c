#include <stdio.h>
#include <stdlib.h>

int q (void){
  return rand() % 10;
}

int p1 (void){
  return q();
}

int p2 (void){
  return p1();
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
