CC_SUFFIX=
OPTIONS=-O0
OPT_OPTIONS=-mem2reg -simplifycfg
AA6=-cfl-anders-aa
AA5=-scev-aa
AA4=-tbaa
AA3=-cfl-steens-aa
AA2=-globals-aa
AA1=-basicaa
AA=$(AA1) $(AA2) $(AA3) $(AA4) $(AA5) $(AA6)

all: program

program: program.bc
	opt -load ~/CAT/lib/CAT.so $^ -disable-output $(AA) -CAT

program.bc: program.c
	clang$(CC_SUFFIX) $(OPTIONS) -emit-llvm $^ -c -o $@
	opt$(CC_SUFFIX) $(OPT_OPTIONS) $@ -o $@
	llvm-dis$(CC_SUFFIX) $@
	clang$(CC_SUFFIX) $@ -lm  -o program

clean:
	rm -f program program_transformed *.bc *.ll *.s program_from_assembly a.out *.cpp program_from_bitcode *.o core*
