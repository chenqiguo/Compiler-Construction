#include <stdio.h>
#include <stdlib.h>
#include <CAT.h>
//test35
void you_cannot_take_decisions_based_on_function_names (CATData par1){
  CAT_add(par1, par1, par1);
  return ;
}

int main (int argc, char *argv[]){
    CATData d1	= CAT_new(5); //arg0Value; inst_arg0; arg0_callInst <- find its dependence
    // argInst(pointer type?); escapeInst; escape_callee
    
  you_cannot_take_decisions_based_on_function_names(d1); // inst_arg0 is in argSet!

  int64_t value = CAT_get(d1); //i

  printf("Values: %ld\n", value);

  printf("CAT invocations = %ld\n", CAT_invocations());
	return 0;
}
