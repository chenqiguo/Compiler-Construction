#include <stdio.h>

int main (int argc, char *argv[]){
  int c;
  c =0;
  if (argc > 10) {
    for (int i = 0; i < argc; i++){
      c = 5;
    }
  } else {
    c = 5;
  }

	return c;
}
