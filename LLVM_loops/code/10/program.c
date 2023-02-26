#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]){
  int z=argc;

  int i=0;
  do {
    z += rand();
L1:
    i++;
  } while (i < argc);

  if (z > 10) {
    goto L1 ;
  }

	return z;
}
