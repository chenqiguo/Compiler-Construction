#include<stdio.h>
#include <stdint.h>

int32_t p0G = 0;
int32_t p1G = 1;
int32_t p2G = 2;

int* getPointer(int32_t id){
  if (id == 0){
    return &p0G;
  } else if (id == 1){
    return &p1G;
  } else if (id == 2){
    return &p2G;
  }

  return NULL;
}

int main (int32_t argc, char *argv[]){

  int32_t *p0 = getPointer(0);
  int32_t *p01 = getPointer(0);

  int32_t *p1 = getPointer(1);
  int32_t *p2 = getPointer(2);

  printf("Result: %d\n", *p0 + *p01 + *p1 + *p2);

  return 0;
}



Function "getPointer"
### Pointers
### Memory accesses
Mem inst: "  %6 = load i32*, i32** %5, align 8"
### Mod/ref
Mem inst: "  %6 = load i32*, i32** %5, align 8"
Function "main"
### Pointers
Pointer: "  %3 = tail call i32* @getPointer(i32 0)"
Size: 4 Bytes
Pointer2: "  %4 = tail call i32* @getPointer(i32 1)"
Size: 4 Bytes
May alias
Pointer2: "  %5 = tail call i32* @getPointer(i32 2)"
Size: 4 Bytes
May alias
Pointer: "  %4 = tail call i32* @getPointer(i32 1)"
Size: 4 Bytes
Pointer2: "  %3 = tail call i32* @getPointer(i32 0)"
Size: 4 Bytes
May alias
Pointer2: "  %5 = tail call i32* @getPointer(i32 2)"
Size: 4 Bytes
May alias
Pointer: "  %5 = tail call i32* @getPointer(i32 2)"
Size: 4 Bytes
Pointer2: "  %3 = tail call i32* @getPointer(i32 0)"
Size: 4 Bytes
May alias
Pointer2: "  %4 = tail call i32* @getPointer(i32 1)"
Size: 4 Bytes
May alias
### Memory accesses
Mem inst: "  %6 = load i32, i32* %3, align 4, !tbaa !2"
Mem inst2: "  %8 = load i32, i32* %4, align 4, !tbaa !2"
May alias
Mem inst2: "  %10 = load i32, i32* %5, align 4, !tbaa !2"
May alias
Mem inst: "  %8 = load i32, i32* %4, align 4, !tbaa !2"
Mem inst2: "  %6 = load i32, i32* %3, align 4, !tbaa !2"
May alias
Mem inst2: "  %10 = load i32, i32* %5, align 4, !tbaa !2"
May alias
### Mod/ref
Mem inst: "  %6 = load i32, i32* %3, align 4, !tbaa !2"
Pointer: "  %3 = tail call i32* @getPointer(i32 0)"
Size: 4 Bytes
