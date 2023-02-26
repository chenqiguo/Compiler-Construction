#include <stdio.h>
#include "CAT.h"
//test9
CATData *g;

void CAT_execution (int userInput){
	CATData	d1,d2,d3;

	d1	= CAT_new(5); //%4
  g = &d1;
	printf("H1: 	Value 1 = %ld\n", CAT_get(d1));
	printf("H1: 	G Value = %ld\n", CAT_get(*g));

  CAT_set(*g, 42);
	printf("H1: 	G Value = %ld\n", CAT_get(*g));

  d2	= CAT_new(8); //%17
  if (userInput > 10){
	  CAT_add(d2, d2, d2);
  }
	printf("H1: 	Value 2 = %ld\n", CAT_get(d2));

	d3	= CAT_new(0);
	CAT_add(d3, d1, d2);
	printf("H1: 	Result = %ld\n", CAT_get(d3));

	return ;
}

int main (int argc, char *argv[]){
	CAT_execution(argc);

  printf("CAT invocations = %ld\n", CAT_invocations());
	return 0;
}
