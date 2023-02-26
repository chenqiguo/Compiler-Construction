#include <stdio.h>
#include "CAT.h"

void CAT_execution (int userInput){
	CATData	d1,d2,d3,d4,d5,d6,d7;

	d1	= CAT_new(5);
	printf("H1: 	Value 1 = %ld\n", CAT_get(d1));

  d2	= CAT_new(8);
  if (userInput > 10){
	  CAT_add(d2, d2, d2);
  }
	printf("H1: 	Value 2 = %ld\n", CAT_get(d2));

	d3	= CAT_new(0);
	CAT_add(d3, d1, d2);
	
  d4	= CAT_new(0);
	CAT_add(d4, d1, d3);
	
  d5	= CAT_new(0);
	CAT_add(d5, d3, d4);
	
  d6	= CAT_new(0);
	d7	= CAT_new(0);

	CAT_add(d6, d1, d1);
	CAT_add(d7, d1, d1);
	printf("H1: 	Result = %ld\n", CAT_get(d7));

	return ;
}

int main (int argc, char *argv[]){
	CAT_execution(argc);

	return 0;
}
