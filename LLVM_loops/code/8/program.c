#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]){
  int z=argc;
  int i=0;

L1:
    z += rand();
    i++;
    if (i < 10) goto L1;
    goto L2;

L2:
	return z;
}
