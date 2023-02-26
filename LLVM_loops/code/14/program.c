#include <stdio.h>

int main (int argc, char *argv[]){

  for (int i=0; i < argc; i++){
    for (int j=0; j < argc; j++){
      for (int z=0; z < argc; z++){
        printf("ciao\n");
        if (argc < 100) {
          z += 2;
          continue ;
        }
        if (argc > 50){
          z += 3;
          continue ;
        }
        printf("ciao again\n");
      }
    }
  }
  return 0;
}
