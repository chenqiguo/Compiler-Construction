#include <stdio.h>
#include <stdlib.h>
#include <CAT.h>
#include <math.h>

CATData myF (CATData p1){
  return p1;
}

void computeF (void){
  int counter1;
  int result;

  void **ref1 = malloc(sizeof(void *));

  CATData x	= CAT_new(4);
  CATData w	= CAT_new(42);

  (*ref1) = x; // probelm??? ref1 and x are aliased! (point to the same object!!!)

  counter1 = 0;
  do {
    CATData y,z; 
    y = NULL; z = NULL;
    int x_value = CAT_get(x); //4
    x_value++; //5

    if (x_value > 10){
      y = CAT_new(CAT_get(x));
    } else {
      y = myF(CAT_new(5)); 
      printf("L1: %ld\n", CAT_get(y)); //5
    }

    CAT_add(*ref1, x, y);
    result = CAT_get(w);

    do {
      int x_value;
      CATData y,z; y = NULL; z = NULL;
      x_value = CAT_get(x); //4
      x_value++; //5
      if (counter1 >= 0){ //0 1
        if (x_value > 10){
          y = CAT_new(CAT_get(x)); //4
        } else {
         y = myF(CAT_new(5)); 
         printf("L2: %ld\n", CAT_get(y)); // 5 has more than needed!!!
        }
      }
      
     CAT_add(*ref1, x, y);
     result = CAT_get(w);

      if (counter1 == 0){//0
        int z_value;
        if (z != NULL){
          z_value = CAT_get(z);
        } else {
          z_value = 125; //
        }
        z = CAT_new(z_value);//125
      }
      counter1++;//1 2
      } while (counter1 < 10);

    if (counter1 == 0){
      z = CAT_new(125);
    }
    counter1++;
  } while (counter1 < 10);

  free(ref1);

  printf("Result = %d\n\n\n\n", result);

  return ;
}

int main (int argc, char *argv[]){

  computeF();

  printf("CAT invocations = %ld\n", CAT_invocations());
  return 0;
}





Function: main
IPA
Dealing with function: main
calleeF: computeF
calleeF: CAT_invocations
calleeF: printf
----------recursiveFunctions:
recur_flag == 0
START inlineFunc: main
callInst:   tail call void @computeF()
inlining callee: computeF
START inlineFunc: main
callInst:   %3 = tail call i8* @CAT_new(i64 4) #4
HOW1
callInst:   %4 = tail call i8* @CAT_new(i64 42) #4
HOW1
callInst:   %5 = tail call i8* @CAT_new(i64 5) #4
HOW1
callInst:   %6 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i64 0, i64 0), i64 5) #4
HOW1
callInst:   tail call void @CAT_add(i8* %3, i8* %3, i8* %5) #4
HOW1
callInst:   %9 = tail call i64 @CAT_get(i8* %3) #4
HOW1
callInst:   %13 = tail call i64 @CAT_get(i8* %3) #4
HOW1
callInst:   %14 = tail call i8* @CAT_new(i64 %13) #4
HOW1
callInst:   %16 = tail call i8* @CAT_new(i64 5) #4
HOW1
callInst:   %17 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.1, i64 0, i64 0), i64 5) #4
HOW1
callInst:   tail call void @CAT_add(i8* %3, i8* %3, i8* %19) #4
HOW1
callInst:   %22 = tail call i8* @CAT_new(i64 125) #4
HOW1
callInst:   %26 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.2, i64 0, i64 0), i32 42) #4
HOW1
callInst:   %27 = tail call i64 @CAT_invocations() #4
HOW1
callInst:   %28 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @.str.3, i64 0, i64 0), i64 %27)
HOW1
END main
END main
loadSrc: i64 4
loadSrc: i64 4
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%CAT_get()!!!
CAT_get() inst:   %9 = tail call i64 @CAT_get(i8* %3) #4
arg0Value:   %3 = tail call i8* @CAT_new(i64 4) #4
Not a phi node!
Try to const propagation arg0Value
constProp: dealing with CAT_get inst:   %9 = tail call i64 @CAT_get(i8* %3) #4
*********inInst:   tail call void @CAT_add(i8* %3, i8* %3, i8* %5) #4 needs constant folding
^^^^^^^^^^^^constantFolding instruction i:   tail call void @CAT_add(i8* %3, i8* %3, i8* %5) #4
callInst:   tail call void @CAT_add(i8* %3, i8* %3, i8* %5) #4 is CAT_add or CAT_sub
inInst (in IN set for this CAT_add or sub):   %3 = tail call i8* @CAT_new(i64 4) #4
inInst is CAT_new
inInst == inst_arg1
reachDef1
v1_exist
result1 = 4
WTFFFFFFFFF!!!!
v1_exist = 1
v2_exist = 0
inInst (in IN set for this CAT_add or sub):   %4 = tail call i8* @CAT_new(i64 42) #4
inInst is CAT_new
reachDef1
v1_exist
result1 = 4
WTFFFFFFFFF!!!!
v1_exist = 1
v2_exist = 0
inInst (in IN set for this CAT_add or sub):   %5 = tail call i8* @CAT_new(i64 5) #4
inInst is CAT_new
inInst == inst_arg2
reachDef1
v1_exist
result1 = 4
reachDef2
v2_exist
result2 = 5
WTFFFFFFFFF!!!!
v1_exist = 1
v2_exist = 1
v1_exist && v2_exist
consts set is: 9
result = 9
consts.size() == 1 : replace with const!
back into constProp
foldConst
foldResult = 9
foldConst
foldResult = 9
foldConst
foldResult = 9
*********inInst:   tail call void @CAT_add(i8* %3, i8* %3, i8* %19) #4 needs constant folding
^^^^^^^^^^^^constantFolding instruction i:   tail call void @CAT_add(i8* %3, i8* %3, i8* %19) #4
callInst:   tail call void @CAT_add(i8* %3, i8* %3, i8* %19) #4 is CAT_add or CAT_sub
inInst (in IN set for this CAT_add or sub):   %4 = tail call i8* @CAT_new(i64 42) #4
inInst is CAT_new
WTFFFFFFFFF!!!!
v1_exist = 0
v2_exist = 0
inInst (in IN set for this CAT_add or sub):   %5 = tail call i8* @CAT_new(i64 5) #4
inInst is CAT_new
WTFFFFFFFFF!!!!
v1_exist = 0
v2_exist = 0
inInst (in IN set for this CAT_add or sub):   tail call void @CAT_add(i8* %3, i8* %3, i8* %5) #4
inInst is CAT_add or CAT_sub
WTFFFFFFFFF!!!!
v1_exist = 0
v2_exist = 0
inInst (in IN set for this CAT_add or sub):   %14 = tail call i8* @CAT_new(i64 %13) #4
inInst is CAT_new
WTFFFFFFFFF!!!!
v1_exist = 0
v2_exist = 0
inInst (in IN set for this CAT_add or sub):   %16 = tail call i8* @CAT_new(i64 5) #4
inInst is CAT_new
WTFFFFFFFFF!!!!
v1_exist = 0
v2_exist = 0
inInst (in IN set for this CAT_add or sub):   tail call void @CAT_add(i8* %3, i8* %3, i8* %19) #4
inInst is CAT_add or CAT_sub
WTFFFFFFFFF!!!!
v1_exist = 0
v2_exist = 0
inInst (in IN set for this CAT_add or sub):   %22 = tail call i8* @CAT_new(i64 125) #4
inInst is CAT_new
WTFFFFFFFFF!!!!
v1_exist = 0
v2_exist = 0
resultConstant = NULL
back into constProp
consts.size() == 1
final result = 9
modified = 1
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%CAT_get()!!!
CAT_get() inst:   %12 = tail call i64 @CAT_get(i8* %3) #4
arg0Value:   %3 = tail call i8* @CAT_new(i64 4) #4
Not a phi node!
Try to const propagation arg0Value
constProp: dealing with CAT_get inst:   %12 = tail call i64 @CAT_get(i8* %3) #4
*********inInst:   tail call void @CAT_add(i8* %3, i8* %3, i8* %5) #4 needs constant folding
^^^^^^^^^^^^constantFolding instruction i:   tail call void @CAT_add(i8* %3, i8* %3, i8* %5) #4
callInst:   tail call void @CAT_add(i8* %3, i8* %3, i8* %5) #4 is CAT_add or CAT_sub
inInst (in IN set for this CAT_add or sub):   %3 = tail call i8* @CAT_new(i64 4) #4
inInst is CAT_new
inInst == inst_arg1
reachDef1
v1_exist
result1 = 4
WTFFFFFFFFF!!!!
v1_exist = 1
v2_exist = 0
inInst (in IN set for this CAT_add or sub):   %4 = tail call i8* @CAT_new(i64 42) #4
inInst is CAT_new
reachDef1
v1_exist
result1 = 4
WTFFFFFFFFF!!!!
v1_exist = 1
v2_exist = 0
inInst (in IN set for this CAT_add or sub):   %5 = tail call i8* @CAT_new(i64 5) #4
inInst is CAT_new
inInst == inst_arg2
reachDef1
v1_exist
result1 = 4
reachDef2
v2_exist
result2 = 5
WTFFFFFFFFF!!!!
v1_exist = 1
v2_exist = 1
v1_exist && v2_exist
consts set is: 9
result = 9
consts.size() == 1 : replace with const!
back into constProp
foldConst
foldResult = 9
foldConst
foldResult = 9
foldConst
foldResult = 9
*********inInst:   tail call void @CAT_add(i8* %3, i8* %3, i8* %18) #4 needs constant folding
^^^^^^^^^^^^constantFolding instruction i:   tail call void @CAT_add(i8* %3, i8* %3, i8* %18) #4
callInst:   tail call void @CAT_add(i8* %3, i8* %3, i8* %18) #4 is CAT_add or CAT_sub
inInst (in IN set for this CAT_add or sub):   %4 = tail call i8* @CAT_new(i64 42) #4
inInst is CAT_new
WTFFFFFFFFF!!!!
v1_exist = 0
v2_exist = 0
inInst (in IN set for this CAT_add or sub):   %5 = tail call i8* @CAT_new(i64 5) #4
inInst is CAT_new
WTFFFFFFFFF!!!!
v1_exist = 0
v2_exist = 0
inInst (in IN set for this CAT_add or sub):   tail call void @CAT_add(i8* %3, i8* %3, i8* %5) #4
inInst is CAT_add or CAT_sub
WTFFFFFFFFF!!!!
v1_exist = 0
v2_exist = 0
inInst (in IN set for this CAT_add or sub):   %13 = tail call i8* @CAT_new(i64 %12) #4
inInst is CAT_new
WTFFFFFFFFF!!!!
v1_exist = 0
v2_exist = 0
inInst (in IN set for this CAT_add or sub):   %15 = tail call i8* @CAT_new(i64 5) #4
inInst is CAT_new
WTFFFFFFFFF!!!!
v1_exist = 0
v2_exist = 0
inInst (in IN set for this CAT_add or sub):   tail call void @CAT_add(i8* %3, i8* %3, i8* %18) #4
inInst is CAT_add or CAT_sub
WTFFFFFFFFF!!!!
v1_exist = 0
v2_exist = 0
inInst (in IN set for this CAT_add or sub):   %21 = tail call i8* @CAT_new(i64 125) #4
inInst is CAT_new
WTFFFFFFFFF!!!!
v1_exist = 0
v2_exist = 0
resultConstant = NULL
back into constProp
consts.size() == 1
final result = 9
modified = 1



; ModuleID = 'program_optimized.bc'
source_filename = "program.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [9 x i8] c"L1: %ld\0A\00", align 1
@.str.1 = private unnamed_addr constant [9 x i8] c"L2: %ld\0A\00", align 1
@.str.2 = private unnamed_addr constant [16 x i8] c"Result = %d\0A\0A\0A\0A\00", align 1
@.str.3 = private unnamed_addr constant [23 x i8] c"CAT invocations = %ld\0A\00", align 1

; Function Attrs: norecurse nounwind readnone uwtable
define dso_local i8* @myF(i8* readnone returned) local_unnamed_addr #0 {
    ret i8* %0
}

; Function Attrs: nounwind uwtable
define dso_local void @computeF() local_unnamed_addr #1 {
    %1 = tail call i8* @CAT_new(i64 4) #4
    %2 = tail call i8* @CAT_new(i64 42) #4
    %3 = trunc i64 4 to i32
    %4 = icmp sgt i32 %3, 9
    br i1 %4, label %5, label %7
    
    ; <label>:5:                                      ; preds = %0
    %6 = tail call i8* @CAT_new(i64 4) #4
    br label %10
    
    ; <label>:7:                                      ; preds = %0
    %8 = tail call i8* @CAT_new(i64 5) #4
    %9 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i64 0, i64 0), i64 5)
    br label %10
    
    ; <label>:10:                                     ; preds = %7, %5
    %11 = phi i8* [ %6, %5 ], [ %8, %7 ]
    tail call void @CAT_add(i8* %1, i8* %1, i8* %11) #4
    br label %12
    
    ; <label>:12:                                     ; preds = %10, %28
    %13 = phi i32 [ 0, %10 ], [ %29, %28 ]
    %14 = tail call i64 @CAT_get(i8* %1) #4
    %15 = trunc i64 %14 to i32
    %16 = icmp sgt i32 %15, 9
    br i1 %16, label %17, label %20
    
    ; <label>:17:                                     ; preds = %12
    %18 = tail call i64 @CAT_get(i8* %1) #4
    %19 = tail call i8* @CAT_new(i64 %18) #4
    br label %23
    
    ; <label>:20:                                     ; preds = %12
    %21 = tail call i8* @CAT_new(i64 5) #4
    %22 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.1, i64 0, i64 0), i64 5)
    br label %23
    
    ; <label>:23:                                     ; preds = %17, %20
    %24 = phi i8* [ %19, %17 ], [ %21, %20 ]
    tail call void @CAT_add(i8* %1, i8* %1, i8* %24) #4
    %25 = icmp eq i32 %13, 0
    br i1 %25, label %26, label %28
    
    ; <label>:26:                                     ; preds = %23
    %27 = tail call i8* @CAT_new(i64 125) #4
    br label %28
    
    ; <label>:28:                                     ; preds = %26, %23
    %29 = add nuw nsw i32 %13, 1
    %30 = icmp eq i32 %29, 10
    br i1 %30, label %31, label %12
    
    ; <label>:31:                                     ; preds = %28
    %.lcssa = phi i64 [ 42, %28 ]
    %32 = trunc i64 %.lcssa to i32
    %33 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.2, i64 0, i64 0), i32 %32)
    ret void
    }
    
    declare dso_local i8* @CAT_new(i64) local_unnamed_addr #2
    
    declare dso_local i64 @CAT_get(i8*) local_unnamed_addr #2
    
    ; Function Attrs: nounwind
    declare dso_local i32 @printf(i8* nocapture readonly, ...) local_unnamed_addr #3
    
    declare dso_local void @CAT_add(i8*, i8*, i8*) local_unnamed_addr #2
    
    ; Function Attrs: nounwind uwtable
    define dso_local i32 @main(i32, i8** nocapture readnone) local_unnamed_addr #1 {
        %3 = tail call i8* @CAT_new(i64 4) #4
        %4 = tail call i8* @CAT_new(i64 42) #4
        %5 = tail call i8* @CAT_new(i64 5) #4
        %6 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i64 0, i64 0), i64 5) #4
        tail call void @CAT_add(i8* %3, i8* %3, i8* %5) #4
        br label %7
        
        ; <label>:7:                                      ; preds = %21, %2
        %8 = phi i32 [ 0, %2 ], [ %22, %21 ]
        %9 = trunc i64 9 to i32
        %10 = icmp sgt i32 %9, 9
        br i1 %10, label %11, label %13
        
        ; <label>:11:                                     ; preds = %7
        %12 = tail call i8* @CAT_new(i64 9) #4
        br label %16
        
        ; <label>:13:                                     ; preds = %7
        %14 = tail call i8* @CAT_new(i64 5) #4
        %15 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.1, i64 0, i64 0), i64 5) #4
        br label %16
        
        ; <label>:16:                                     ; preds = %13, %11
        %17 = phi i8* [ %12, %11 ], [ %14, %13 ]
        tail call void @CAT_add(i8* %3, i8* %3, i8* %17) #4
        %18 = icmp eq i32 %8, 0
        br i1 %18, label %19, label %21
        
        ; <label>:19:                                     ; preds = %16
        %20 = tail call i8* @CAT_new(i64 125) #4
        br label %21
        
        ; <label>:21:                                     ; preds = %19, %16
        %22 = add nuw nsw i32 %8, 1
        %23 = icmp eq i32 %22, 10
        br i1 %23, label %computeF.exit, label %7
        
        computeF.exit:                                    ; preds = %21
        %24 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.2, i64 0, i64 0), i32 42) #4
        %25 = tail call i64 @CAT_invocations() #4
        %26 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @.str.3, i64 0, i64 0), i64 %25)
        ret i32 0
    }
    
    declare dso_local i64 @CAT_invocations() local_unnamed_addr #2
