#include <stdio.h>
#include <stdlib.h>
#include "CAT.h"
//test53
int my_f (CATData x, CATData y, CATData z, int argc){ // all problems are in z2

  int i = 0;
  do {
      printf("Z: %ld\n", CAT_get(z)); //%7: z; %9: CAT_get(z); %10: printf

    CAT_add(z, x, y);
      printf("Z: %ld\n", CAT_get(z)); //%7: z; %11: CAT_get(z);

    CAT_add(x, x, x);
    CAT_add(y, y, y);

    z = CAT_new(1); //%13

    for (int j=0; j < 10; j++){
        printf("Z2: %ld\n", CAT_get(z)); // the first: No problem %16: z; %17: CAT_get(z);

      CAT_add(z, x, y);
      printf("Z2: %ld\n", CAT_get(z)); //problem!!! %19: CAT_get(z);

      CAT_add(x, x, x);
      CAT_add(y, y, y);

      z = CAT_new(1);
    }

    for (int k=0; k < (i + 1); k++){
      printf("Z3: %ld\n", CAT_get(z));

      CAT_add(z, x, y);
      printf("Z3: %ld\n", CAT_get(z));

      CAT_add(x, x, x);
      CAT_add(y, y, y);

      z = CAT_new(1);

      for (int w=0; w < (k * 3); w++){
        printf("Z21: %ld\n", CAT_get(z));

        CAT_add(z, x, y);
        printf("Z21: %ld\n", CAT_get(z));

        CAT_add(x, x, x);
        CAT_add(y, y, y);

        z = CAT_new(1);
        if (CAT_get(x) > 1000000){
          z = CAT_new(my_f(x, y, z, argc));
        }

        for (int w2=0; w2 < (w * 2); w2++){
          printf("Z211: %ld\n", CAT_get(z));

          CAT_add(z, x, y);
          printf("Z211: %ld\n", CAT_get(z));

          CAT_add(x, x, x);
          CAT_add(y, y, y);

          z = CAT_new(1);
        }
      }
    }

    i++;
  } while (i < (argc + 2));

  return CAT_get(z);
}

int main (int argc, char *argv[]){
  CATData x = CAT_new(40);
  CATData y = CAT_new(2);
  CATData z = CAT_new(0);

  int res = my_f(x, y, z, argc);
  printf("Result = %d\n", res);

  printf("CAT invocations = %ld\n", CAT_invocations());
  return 0;
}



Function: my_f
loadSrc:   %.lcssa6 = phi i8* [ %39, %38 ]
loadSrc:   %13 = tail call i8* @CAT_new(i64 1) #3
loadSrc:   %39 = phi i8* [ %35, %28 ], [ %.lcssa5, %.loopexit2 ]
loadSrc:   %61 = phi i8* [ %57, %56 ], [ %.lcssa4, %.loopexit ]
loadSrc:   %71 = tail call i8* @CAT_new(i64 1) #3
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%CAT_get()!!!
CAT_get() inst:   %9 = tail call i64 @CAT_get(i8* %7) #3
arg0Value:   %7 = phi i8* [ %.lcssa6, %24 ], [ %2, %4 ]
Is phi node!
nodeValue is phi node
phiValues.size() != 1
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%CAT_get()!!!
CAT_get() inst:   %11 = tail call i64 @CAT_get(i8* %7) #3
arg0Value:   %7 = phi i8* [ %.lcssa6, %24 ], [ %2, %4 ]
Is phi node!
nodeValue is phi node
phiValues.size() != 1
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%CAT_get()!!!
CAT_get() inst:   %17 = tail call i64 @CAT_get(i8* %16) #3
arg0Value:   %16 = phi i8* [ %13, %5 ], [ %21, %14 ]
Is phi node!
phi node is CAT_new
phi node is CAT_new
phi_flag!
CatType is: 0
replace arg0Value with const
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%CAT_get()!!!
CAT_get() inst:   %18 = tail call i64 @CAT_get(i8* %16) #3
arg0Value:   %16 = phi i8* [ %13, %5 ], [ %20, %14 ]
Is phi node!
phi node is CAT_new
phi node is CAT_new
phi_flag!
CatType is: 0
replace arg0Value with const
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%CAT_get()!!!
CAT_get() inst:   %29 = tail call i64 @CAT_get(i8* %28) #3
arg0Value:   %28 = phi i8* [ %37, %36 ], [ %.lcssa, %.preheader3 ]
Is phi node!
nodeValue is phi node
phiValues.size() != 1
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%CAT_get()!!!
CAT_get() inst:   %31 = tail call i64 @CAT_get(i8* %28) #3
arg0Value:   %28 = phi i8* [ %37, %36 ], [ %.lcssa, %.preheader3 ]
Is phi node!
nodeValue is phi node
phiValues.size() != 1
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%CAT_get()!!!
CAT_get() inst:   %43 = tail call i64 @CAT_get(i8* %42) #3
arg0Value:   %42 = phi i8* [ %59, %58 ], [ %33, %.preheader1 ]
Is phi node!
nodeValue is phi node
phiValues.size() != 1
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%CAT_get()!!!
CAT_get() inst:   %45 = tail call i64 @CAT_get(i8* %42) #3
arg0Value:   %42 = phi i8* [ %59, %58 ], [ %33, %.preheader1 ]
Is phi node!
nodeValue is phi node
phiValues.size() != 1
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%CAT_get()!!!
CAT_get() inst:   %48 = tail call i64 @CAT_get(i8* %0) #3
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%CAT_get()!!!
CAT_get() inst:   %65 = tail call i64 @CAT_get(i8* %64) #3
arg0Value:   %64 = phi i8* [ %69, %62 ], [ %55, %.preheader ]
Is phi node!
phi node is CAT_new
nodeValue is phi node
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%CAT_get()!!!
CAT_get() inst:   %67 = tail call i64 @CAT_get(i8* %64) #3
arg0Value:   %64 = phi i8* [ %69, %62 ], [ %55, %.preheader ]
Is phi node!
phi node is CAT_new
nodeValue is phi node
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%CAT_get()!!!
CAT_get() inst:   %73 = tail call i64 @CAT_get(i8* %.lcssa6.lcssa) #3
arg0Value:   %.lcssa6.lcssa = phi i8* [ %.lcssa6, %22 ]
Is phi node!
nodeValue is phi node
phiValues.size() != 1
