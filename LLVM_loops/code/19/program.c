#include <stdio.h>

int myF (int k, int iters){
  int s = 5;
  for (int i=0; i < iters; i++){
    s = s + k;
  }
	return s;
}

int main (int argc, char *argv[]){
  printf("Value %d\n", myF(argc, 100));
  return 0;
}
