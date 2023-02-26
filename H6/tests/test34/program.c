#include <stdio.h>
#include <stdlib.h>
#include <CAT.h>
//test34: Not phi node?
int my_f (CATData d, int v){
  if (d == NULL){
    return v+1;
  }
  return v - 10;
}

int main (int argc, char *argv[]){
    CATData d = CAT_new(5); //@@@@; arg0Value; inst_arg0; arg0_callInst
  int v = 0;

  if (argc > 5){
      d = CAT_new(5); //@@@;arg0Value;
    v = my_f(d, argc); //not ref, is obj -> not in memSet-> not escape!
  } else {
      d = CAT_new(5); //xxx; remove???
  }
   //???can they merge? yes!
  printf("Value at the end: %ld\n", CAT_get(d) + v); //xxx (phi node) v is phi node, but not d
                                                     // i

  printf("CAT invocations = %ld\n", CAT_invocations());
  return 0;
}
