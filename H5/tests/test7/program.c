#include <stdio.h>
#include <CAT.h>

//test7

int main (int argc, char *argv[]){
  CATData d1;
  
  if (argc > 10){
    d1	= CAT_new(8);
  } else {
    d1	= CAT_new(8);
  }

	printf("H5: 	Value of d1 = %ld\n", CAT_get(d1)); //i

  printf("CAT invocations = %ld\n", CAT_invocations());
	return 0;
}
HERE3!!! CAT_get
HERE4!!! arg0Value is:   %3 = tail call i8* @CAT_new(i64 8) #3
