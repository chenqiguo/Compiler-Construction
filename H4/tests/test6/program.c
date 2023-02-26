#include <stdio.h>
#include "CAT.h"

void CAT_execution (int userInput){
	CATData	d1;

    d1	= CAT_new(5); //inst_arg0; inInst_arg0; val
    CAT_set(d1, 3); // inInst; in_callInst; (in_callee); reachDef
    printf("H1: 	Value 1 = %ld\n", CAT_get(d1)); //i; callInst

  return ;
}

int main (int argc, char *argv[]){
	CAT_execution(argc);

  printf("CAT invocations = %ld\n", CAT_invocations());
	return 0;
}


%2 = tail call i8* @CAT_new(i64 5) #3
tail call void @CAT_set(i8* %2, i64 3) #3
%3 = tail call i64 @CAT_get(i8* %2) #3
%4 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str, i64 0, i64 0), i64 %3)
ret void
