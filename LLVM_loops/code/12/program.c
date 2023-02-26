#include <stdio.h>

int __attribute__ ((noinline)) myF (int v){
  printf("Ciao\n");
  return v + 1;
}

int main (int argc, char *argv[]){
  auto r = 0;

  for (auto i=0; i < 10; i++){
    r = myF(r);
  }

  return r;
}
