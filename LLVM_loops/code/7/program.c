#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]){
  int z=argc;

  int i=0;
  do {
    z += rand();
    i++;
  } while (i < argc);

	return z;
}
