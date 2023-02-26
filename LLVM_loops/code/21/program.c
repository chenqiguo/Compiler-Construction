#include <stdio.h>

int myF (int v){
  return v + 1;
}

int main (int argc, char *argv[]){
  auto r = 0;

  for (auto i=0; i < argc; i++){
    r = myF(r);
    if (r == 50) break ;
  }

  return r;
}
