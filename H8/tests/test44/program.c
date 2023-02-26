#include <stdio.h>
#include <stdlib.h>
#include "CAT.h"
// test44
int main (int argc, char *argv[]){
  CATData x = CAT_new(40);
  CATData y = CAT_new(2);

  for (int i=1; i <= 5; i++){
    printf("X: %ld\n", CAT_get(x));
    printf("Y: %ld\n", CAT_get(y));

    CATData z = CAT_new(0);
    CAT_add(z, x, y);
    printf("Z: %ld\n", CAT_get(z));

    CAT_add(x, x, x);
  }

  printf("CAT invocations = %ld\n", CAT_invocations());
  return 0;
}


//7 iterations:
#### Running the CAT compiler
Function: main
*******************************************************************
---ICmpInst:
***  %18 = icmp eq i32 %17, 6
false
final result: false
HERE!!!
analyzeLoop
-----------------------------------------------------------
Loop: Loop at depth 1 containing: %8<header><latch><exiting>
Trip count: 5
Trip multiple: 5
Partially unrolled
####  The compiler modified the input bitcode: the fixed point isn't reached.
#### Running the CAT compiler
Function: main
*******************************************************************
---ICmpInst:
***  %11 = icmp eq i32 %10, 6
false
***  %31 = icmp eq i32 %30, 6
false
final result: false
HERE!!!
analyzeLoop
-----------------------------------------------------------
Loop: Loop at depth 1 containing: %15<header><latch><exiting>
Trip count: 2
Trip multiple: 2
Fully unrolled
####  The compiler modified the input bitcode: the fixed point isn't reached.
#### Running the CAT compiler
Function: main
####  The compiler did not modify the input bitcode: the fixed point has been reached.
llvm-dis program_optimized.bc


//7 Iterations
#### Running the CAT compiler
Function: main
*******************************************************************
HERE!!!
analyzeLoop
-----------------------------------------------------------
Loop: Loop at depth 1 containing: %8<header><latch><exiting>
Trip count: 5
Trip multiple: 5
Partially unrolled
####  The compiler modified the input bitcode: the fixed point isn't reached.
#### Running the CAT compiler
Function: main
*******************************************************************
HERE!!!
analyzeLoop
-----------------------------------------------------------
Loop: Loop at depth 1 containing: %15<header><latch><exiting>
Trip count: 2
Trip multiple: 2
Fully unrolled
####  The compiler modified the input bitcode: the fixed point isn't reached.
#### Running the CAT compiler
Function: main
####  The compiler did not modify the input bitcode: the fixed point has been reached.
llvm-dis program_optimized.bc
