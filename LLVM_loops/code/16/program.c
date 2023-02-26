#include <stdio.h>

int main (int argc, char *argv[]){

  for (int i=0; i < argc; i++){
    auto j = i * argc;
    printf("Value %d\n", j);
  }

	return 0;
}
