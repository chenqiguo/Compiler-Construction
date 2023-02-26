all: loop
OPT_LEVEL=-O1

program.bc: program.c
	clang $(OPT_LEVEL) -emit-llvm -c $<
	opt -mem2reg $@ -o $@
	llvm-dis $@

cfg: program.bc
	opt -dot-cfg -disable-output $< 

cfg_lcssa: program_lcssa.bc
	opt -dot-cfg -disable-output $< 

loop: program.bc
	opt -load ~/CAT/lib/CAT.so $^ -loops -loop-simplify -lcssa -domtree -scalar-evolution -assumption-cache-tracker -CAT -o loop.bc
	llvm-dis loop.bc
	opt -dot-cfg -disable-output loop.ll
	rm -f cfg.myF.dot

normalize: program.bc
	opt -loop-simplify program.bc -o program.bc
	llvm-dis $^

simplify: program.bc
	opt -lcssa program.bc -o program.bc
	llvm-dis $^

clean:
	rm -f *.bc *.ll *.dot *.jpg ./.*.dot

.PHONY: clean cfg
