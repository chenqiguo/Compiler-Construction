OPTIONS=-I../misc

all: compiler_output program1.ll

compiler_output: program1.bc ../misc/CAT.bc
	cat-c $^ 2> $@

install: program1.bc program1.ll
	cp *.bc *.ll bitcode/

oracle: compiler_output
	mv $^ output/oracle_output

program1.bc: program1.c
	clang $(OPTIONS) -O1 -emit-llvm -c $<

program1.ll: program1.bc
	llvm-dis $<

check: compiler_output
	../misc/check.sh

clean:
	rm -f program1 *.bc *.ll a.out compiler_output diff_output *.gch diff/*output ;
	cp bitcode/* . ;
