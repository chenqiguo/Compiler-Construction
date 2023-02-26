#include <stdio.h>

int main (){
  int a[2][4];

  printf("0x%p\n", &a[0][0]);
  printf("0x%p\n", &a[0][1]);
  printf("  Distance: %d bytes\n", ((unsigned int)(&a[0][1])) - ((unsigned int)(&a[0][0])));
 
  printf("0x%p\n", &a[0][0]);
  printf("0x%p\n", &a[1][0]);
  printf("  Distance: %d bytes\n", ((unsigned int)(&a[1][0])) - ((unsigned int)(&a[0][0])));
 
  return 0;
}
