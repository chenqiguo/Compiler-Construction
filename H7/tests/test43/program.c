#include <stdio.h>
#include "CAT.h"

void CAT_execution (int userInput){
	CATData	d1,d2,d3,d4,d5;

	d1	= CAT_new(5); // %2
  d2	= CAT_new(8); // %3
  d3	= CAT_new(8); // %4
  d4	= CAT_new(8); // %5
  if (userInput > 10){
	  CAT_add(d1, d2, d3);
  }
	printf("H1: 	Value 1 = %ld\n", CAT_get(d1));//5
	printf("H1: 	Value 2 = %ld\n", CAT_get(d2));//8
	printf("H1: 	Value 3 = %ld\n", CAT_get(d3));//8
	printf("H1: 	Value 4 = %ld\n", CAT_get(d4));//8
	
  CAT_add(d1, d2, d3);
	printf("H1: 	Value 1 = %ld\n", CAT_get(d1));//16 %13
	printf("H1: 	Value 2 = %ld\n", CAT_get(d2));//8
	printf("H1: 	Value 3 = %ld\n", CAT_get(d3));//8
	printf("H1: 	Value 4 = %ld\n", CAT_get(d4));//8

  if (userInput > 20){
	  d5	= CAT_new(0);
  }
	printf("H1: 	Value 1 = %ld\n", CAT_get(d1)); //16 -> 5? %23
	printf("H1: 	Value 2 = %ld\n", CAT_get(d2));//8
	printf("H1: 	Value 3 = %ld\n", CAT_get(d3));//8
	printf("H1: 	Value 4 = %ld\n", CAT_get(d4));//8

  if (userInput > 25){
	  printf("H1: 	Value 5 = %ld\n", CAT_get(d4));
  }

	return ;
}

int main (int argc, char *argv[]){
	CAT_execution(argc);

  printf("CAT invocations = %ld\n", CAT_invocations()); //8 -> 6?
	return 0;
}
