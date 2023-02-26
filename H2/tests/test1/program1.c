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

	d3	= CAT_new(0); //callInst_arg0 <- kill[I] <- callInst_u_def
    CAT_add(d3, d1, d2); //callInst <- i ; u <- callInst_u
	CAT_add(d3, d1, d3); // u <- callInst_u <- inst_u
	CAT_add(d3, d3, d3); // u <- callInst_u <- inst_u
	CAT_add(d3, d1, d1); // u <- callInst_u <- inst_u
	printf("H1: 	Result = %ld\n", CAT_get(d3));

	return ;
}

int main (int argc, char *argv[]){
	CAT_execution(argc);

	return 0;
}
