#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]){
  int z=argc;
  int i=0;

  if (argc > 1){
    goto L1;
  }
  goto L2;

L1:
    z += rand();
    goto L2;

L2:
    i++;
    goto L1;


	return z;
}
