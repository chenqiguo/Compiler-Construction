#include <stdio.h>
#include "CAT.h"

CATData *g;

void CAT_execution (int userInput){
	CATData	d1,d2,d3;

	d1	= CAT_new(5);

  g = &d1;

  CAT_set(*g, 42);

	printf("H1: 	G Value = %ld\n", CAT_get(*g));

	return ;
}

int main (int argc, char *argv[]){
	CAT_execution(argc);

  printf("CAT invocations = %ld\n", CAT_invocations());
	return 0;
}
