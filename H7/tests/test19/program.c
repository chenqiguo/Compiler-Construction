#include <stdio.h>
#include <CAT.h>
//test19
int another_execution (int userInput, CATData y){
	printf("H1: 	Y = %ld\n", CAT_get(y)); //8
	CAT_add(y, y, y);
	return CAT_get(y);
}

void CAT_execution (int userInput, CATData d1, int userInput2){ //d1: %3
	CATData d2,d3;

  d2	= CAT_new(8);//%5
  if (userInput > 10){
	  CAT_add(d1, d2, d2); // d1 = 8 + 8 = 16
  }
	printf("H1: 	Value 1 = %ld\n", CAT_get(d1)); //8
  if (userInput2 > 0){
	  printf("H1: 	Value 2 = %ld\n", CAT_get(d2)); //8
  }

	d3	= CAT_new(0); //%14
	CAT_add(d3, d1, d2); // d3 = 8 + 8 | d3 = 16 + 8 <- wrong!
	printf("H1: 	Result = %ld\n", CAT_get(d3)); //16 -> 24 ?

	return ;
}

int main (int argc, char *argv[]){
  CATData x;

  x	= CAT_new(8);//%3
	CAT_execution(argc, x, argc+1);

	printf("Result 2 = %d\n", another_execution(argc, x)); //16

  printf("CAT invocations = %ld\n", CAT_invocations());
	return 0;
}
