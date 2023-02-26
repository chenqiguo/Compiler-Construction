#include <stdio.h>
#include <CAT.h>
// test21
void CAT_execution (CATData	d1){
	CATData d2,d3;

    d2	= CAT_new(8); //%4     inst_arg2; inInst
    CAT_add(d1, d2, d2); //%3           modify it to CAT_set(d1, 16);  d2 = 8;
    printf("H1: 	Value 2 = %ld\n", CAT_get(d2));

    d3	= CAT_new(0); //     arg0Value; inst_arg0; arg0_callInst
    CAT_add(d3, d1, d2); //    inInst(i); callInst; callee   1)d2=8; d1 = 5 -> d3 = 13
    printf("H1: 	Result = %ld\n", CAT_get(d3)); //%6      wrong constant folding; i; idx

	return ;
}

int main (int argc, char *argv[]){
  CATData d1;
    d1	= CAT_new(5);//%3        inInst_arg0; inst_arg1
	CAT_execution(d1);

  printf("CAT invocations = %ld\n", CAT_invocations());
	return 0;
}
