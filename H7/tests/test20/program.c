#include <stdio.h>
#include <stdlib.h>
#include <CAT.h>
// test20 HW7
void function_that_complicates_everything (CATData *par1){
  CAT_add(*par1, *par1, *par1);
  return ;
}

int main (int argc, char *argv[]){
	CATData d1	= CAT_new(5);
  
  function_that_complicates_everything(&d1);

  int64_t value = CAT_get(d1);

  printf("Values: %ld\n", value); //10 -> 5?

  printf("CAT invocations = %ld\n", CAT_invocations());
	return 0;
}


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%CAT_get()!!!
arg0Value:   %5 = tail call i8* @CAT_new(i64 5) #4
constProp: dealing with CAT_get inst:   %7 = tail call i64 @CAT_get(i8* %5) #4
reachDef
reachResult = 5
reachDef
reachResult = 5
consts.size() == 1
final result = 5




define dso_local i32 @main(i32, i8** nocapture readnone) local_unnamed_addr #0 {
    %3 = alloca i8*, align 8 //loadSrc
    %4 = bitcast i8** %3 to i8*
    call void @llvm.lifetime.start.p0i8(i64 8, i8* nonnull %4) #4
    %5 = tail call i8* @CAT_new(i64 5) #4 //srcInst
    store i8* %5, i8** %3, align 8, !tbaa !2
    %6 = load i8*, i8** %3, align 8, !tbaa !2 //argValue; argInst: %3: loadSrc
    call void @CAT_add(i8* %6, i8* %6, i8* %6) #4 //func
    %7 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @.str, i64 0, i64 0), i64 5)
    %8 = tail call i64 @CAT_invocations() #4
    %9 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @.str.1, i64 0, i64 0), i64 %8)
    call void @llvm.lifetime.end.p0i8(i64 8, i8* nonnull %4) #4
    ret i32 0
}



