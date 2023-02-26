all: loop
OPT_LEVEL=-O0

program.bc: program.c
	clang $(OPT_LEVEL) -emit-llvm -Xclang -disable-O0-optnone -c $<
	opt -mem2reg $@ -o $@
	llvm-dis $@

cfg: program.bc
	opt -view-cfg $< -disable-output

cfg_lcssa: program_lcssa.bc
	opt -view-cfg $< -disable-output

loop: program.bc
	opt -load ~/CAT/lib/CAT.so $^ -disable-output -CAT
	llvm-dis $^

normalize: program.bc
	opt -loop-simplify program.bc -o program.bc
	llvm-dis $^

simplify: program.bc
	opt -lcssa program.bc -o program.bc
	llvm-dis $^

clean:
	rm -f *.bc *.ll
