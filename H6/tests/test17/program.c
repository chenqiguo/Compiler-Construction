#include <stdio.h>
#include <stdlib.h>
#include <CAT.h>
//test17 HW6
void function_that_complicates_everything (CATData *par1){
  CAT_add(*par1, *par1, *par1); //@@@
  return ;
}

int main (int argc, char *argv[]){


    CATData d1	= CAT_new(5); //@@@ in memSet; every CAT_new is a pointer!
  
    function_that_complicates_everything(&d1); //is ref, soescape!

  int64_t value = CAT_get(d1); //@@@

  printf("Values: %ld\n", value);

  printf("CAT invocations = %ld\n", CAT_invocations());
  return 0;
}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%CAT_get()!!!
arg0Value:   %18 = load i8*, i8** %7, align 8
constProp: dealing with CAT_get inst:   %19 = call i64 @CAT_get(i8* %18)
constProp: dealing with CAT_get inst:   %19 = call i64 @CAT_get(i8* %18)
constProp: dealing with CAT_get inst:   %19 = call i64 @CAT_get(i8* %18)
constProp: dealing with CAT_get inst:   %19 = call i64 @CAT_get(i8* %18)
constProp: dealing with CAT_get inst:   %19 = call i64 @CAT_get(i8* %18)
constProp: dealing with CAT_get inst:   %19 = call i64 @CAT_get(i8* %18)
constProp: dealing with CAT_get inst:   %19 = call i64 @CAT_get(i8* %18)
constProp: dealing with CAT_get inst:   %19 = call i64 @CAT_get(i8* %18)
constProp: dealing with CAT_get inst:   %19 = call i64 @CAT_get(i8* %18)



define dso_local i32 @main(i32, i8**) #0 {
    %3 = alloca i8**, align 8
    %4 = alloca i32, align 4
    %5 = alloca i32, align 4
    %6 = alloca i8**, align 8
    %7 = alloca i8*, align 8
    %8 = alloca i64, align 8
    store i32 0, i32* %4, align 4
    store i32 %0, i32* %5, align 4
    store i8** %1, i8*** %6, align 8
    %9 = call i8* @CAT_new(i64 5)
    store i8* %9, i8** %7, align 8
    %10 = bitcast i8*** %3 to i8*
    call void @llvm.lifetime.start.p0i8(i64 8, i8* %10)
    store i8** %7, i8*** %3, align 8
    %11 = load i8**, i8*** %3, align 8
    %12 = load i8*, i8** %11, align 8
    %13 = load i8**, i8*** %3, align 8
    %14 = load i8*, i8** %13, align 8
    %15 = load i8**, i8*** %3, align 8
    %16 = load i8*, i8** %15, align 8
    call void @CAT_add(i8* %12, i8* %14, i8* %16) #3
    %17 = bitcast i8*** %3 to i8*
    call void @llvm.lifetime.end.p0i8(i64 8, i8* %17)
    %18 = load i8*, i8** %7, align 8
    %19 = call i64 @CAT_get(i8* %18) //here!!!
    store i64 %19, i64* %8, align 8
    %20 = load i64, i64* %8, align 8
    %21 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @.str, i32 0, i32 0), i64 %20)
    %22 = call i64 @CAT_invocations()
    %23 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @.str.1, i32 0, i32 0), i64 %22)
    ret i32 0
}
