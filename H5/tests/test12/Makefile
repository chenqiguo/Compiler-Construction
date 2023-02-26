LLVM_SUFFIX= 
OPTIONS=-I../misc

all: program_optimized.bc

program_optimized.bc: program.bc
	cat-c -emit-llvm -c $< -o $@ &> compiler_output
	llvm-dis$(LLVM_SUFFIX) $@

program.bc: program.c
	clang$(LLVM_SUFFIX) $(OPTIONS) -O1 -emit-llvm -c $<

program.ll: program.bc
	llvm-dis$(LLVM_SUFFIX) $^

program: program.bc ../misc/CAT.bc
	clang$(LLVM_SUFFIX) -O3 $^ -o $@

program_optimized: program_optimized.bc ../misc/CAT.bc
	clang$(LLVM_SUFFIX) -O3 $^ -o $@

program_optimized.ll: program_optimized.bc
	llvm-dis$(LLVM_SUFFIX) $^

program_output: program
	./$< > $@

program_optimized_output: program_optimized
	./$< > $@

check: program_optimized_output
	../misc/check.sh "output/oracle.txt" "program_optimized_output"

install: program.bc program.ll
	cp *.bc *.ll bitcode/

oracle: program_optimized_output
	mv $^ output/oracle.txt

clean:
	rm -rf program program_optimized program_optimized_output *.bc *.ll a.out diff core* program_output compiler_output ;
	cp bitcode/* . ;
