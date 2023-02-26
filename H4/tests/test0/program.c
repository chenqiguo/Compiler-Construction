#include <stdio.h>
#include "CAT.h"

void CAT_execution (int userInput){
	CATData	d1,d2,d3;

	d1	= CAT_new(5);
	printf("H1: 	Value 1 = %ld\n", CAT_get(d1));

  d2	= CAT_new(8);
  if (userInput > 10){
	  CAT_add(d2, d2, d2);
  }
	printf("H1: 	Value 2 = %ld\n", CAT_get(d2));

	d3	= CAT_new(0); // inst_arg0
    CAT_add(d3, d1, d2); //inInst // const folding: d3 = 5 + 8 -> d3 = 13 -> CAT_set(d3, 13);
	printf("H1: 	Result = %ld\n", CAT_get(d3)); //i

	return ;
}

int main (int argc, char *argv[]){
	CAT_execution(argc);

  printf("CAT invocations = %ld\n", CAT_invocations());
	return 0;
}
