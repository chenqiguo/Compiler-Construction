OPTIONS=-I../misc

all: compiler_output program1.ll

compiler_output: program1.bc ../misc/CAT.bc
	cat-c $^ 2> $@

program1.bc: program1.c
	clang $(OPTIONS) -O3 -emit-llvm -c $<

program1.ll: program1.bc
	llvm-dis $<

program1: program1.bc
	clang $(OPTIONS) $^ -o $@

oracle: compiler_output
	mv compiler_output output/oracle_output

check: compiler_output
	../misc/check.sh

clean:
	rm -f program1 *.bc *.ll a.out compiler_output diff_output *.gch diff/*;
	if test -d bitcode ; then cp bitcode/* . ; fi ;

.PHONY: run oracle check clean install
