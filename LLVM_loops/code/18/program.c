#include <stdio.h>

int main (int argc, char *argv[]){
  int s = 5;
  for (int i=0; i < 100; i++){
    s = s + argc;
  }
  printf("Value %d\n", s);

	return 0;
}
