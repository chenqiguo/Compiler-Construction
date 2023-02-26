#include <stdio.h>

void myF (int *p, int n){
  for (int x=0; x < n; x++){
    p[x] = x*x*x + 2*x*x + 3*x + 7;
  }
}

int main (int argc, char *argv[]){
  int p[100];
  myF(p, 100);
  return 0;
}
