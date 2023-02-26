#include <stdio.h>
#include <CAT.h>

//test3 phi!!!

int another_execution (int userInput, CATData y){
	printf("H1: 	Y = %ld\n", CAT_get(y));
	CAT_add(y, y, y);
	return CAT_get(y);
}

void CAT_execution (int userInput, CATData d, int userInput2){//d1
	CATData d2,d3;
    
  //d2    = CAT_new(8);
    d1	= CAT_new(8); // arg0Value; nodeValue; nodeInst; nodeInst_arg0; opValue; constValue
	printf("H1: 	Value 2 = %ld\n", CAT_get(d2));
  if (userInput > 10){
    d2	= CAT_new(8);
      CAT_add(d1, d2, d2); //nodeValue; nodeInst
  }
	printf("H1: 	Value 2 = %ld\n", CAT_get(d2));
	printf("H1: 	Value 1 = %ld\n", CAT_get(d1)); //phiNode
  if (userInput2 > 0){
	  printf("H1: 	Value 2 = %ld\n", CAT_get(d2));
  }

	return ;
}

int main (int argc, char *argv[]){
  CATData x;

  x	= CAT_new(8);
	CAT_execution(argc, x, argc+1);

	printf("Result 2 = %d\n", another_execution(argc, x)); //funcInst

  printf("CAT invocations = %ld\n", CAT_invocations());
	return 0;
}

//escapeSet = {}
//escapeSetSpecific = {x}
