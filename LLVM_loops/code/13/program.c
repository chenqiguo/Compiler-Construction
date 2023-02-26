#include <stdio.h>

int myF (int v, int *trip){
  if (v < 0){
    (*trip)--;
  }
  return v + 1;
}

int main (int argc, char *argv[]){
  auto r = 0;
  auto trip = argc;

  for (auto i=0; i < trip; i++){
    r = myF(r, &trip);
  }

  return r;
}
