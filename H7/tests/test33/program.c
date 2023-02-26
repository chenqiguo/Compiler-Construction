#include <stdio.h>
#include "CAT.h"
//test33
void CAT_execution (int userInput){
	CATData	d1,d2,d3;

	d1	= CAT_new(5); //%2
	printf("H1: 	Value 1 = %ld\n", CAT_get(d1)); //5

  d2	= CAT_new(8); //%4
  if (userInput > 10){
	  CAT_add(d2, d2, d2); //d2 = 16
  }
	printf("H1: 	Value 2 = %ld\n", CAT_get(d2)); //8 <- this get should keep

	d3	= CAT_new(0); // %10
	CAT_add(d3, d1, d2); // d3 = 5 + 8 = 13!!!
	printf("H1: 	Result = %ld\n", CAT_get(d3)); // 13 -> 21?

	return ;
}

int main (int argc, char *argv[]){
	CAT_execution(argc);

  printf("CAT invocations = %ld\n", CAT_invocations());
	return 0;
}
