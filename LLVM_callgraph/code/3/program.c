#include <stdio.h>
#include <stdlib.h>

int q (void){
  return rand() % 10;
}


int p1 (void){
  return q();
}

int z1 (void){
  return 5;
}

int z2 (void){
  return z1();
}

int main (int argc, char *argv[]){
  int t1 = p1();

  int t2 = z2();

  printf("%d\n", t1 + t2);

	return 0;
}
