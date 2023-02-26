#include <stdio.h>
#include <stdlib.h>
#include <CAT.h>

int q (int v2){
  return 5;
}

int p (int v, int (*p_to_f)(int v2)){
  int m;
  if (v < 10){
    m = (*p_to_f)(v);
  } else {
    m = 2;
  }
  return m;
}

int main (int argc, char *argv[]){
  int x,y,r,t;
  
  x = 7;
  r = p(x, q);
  
  y = 80;
  t = p(y, q);

  printf("%d %d\n", r, t);

	return 0;
}
