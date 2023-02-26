#include <stdio.h>
#include <stdlib.h>
#include <CAT.h>

int p (int v){
  int m;
  if (v < 10){
    m = 1;
  } else {
    m = 2;
  }
  return m;
}

int main (int argc, char *argv[]){
  int x,y,r,t;
  
  x = 7;
  r = p(x);
  
  y = 80;
  t = p(y);

  printf("%d %d\n", r, t);

	return 0;
}
