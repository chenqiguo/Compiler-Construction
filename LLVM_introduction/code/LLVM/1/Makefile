CC_SUFFIX=""

all: hello_world

hello_world: hello_world.c
	clang$(CC_SUFFIX) $^ -O0 -o $@

hello_world_optimized: hello_world.c
	clang$(CC_SUFFIX) -O3 $^ -o hello_world

hello_world.bc: hello_world.c
	clang$(CC_SUFFIX) -O0 -emit-llvm $^ -c -o $@

hello_world_optimized.bc: hello_world.c
	clang$(CC_SUFFIX) -O3 -emit-llvm $^ -c -o $@

hello_world.ll: hello_world.bc
	llvm-dis$(CC_SUFFIX) < $^ > $@

run_bitcode: hello_world.bc
	lli$(CC_SUFFIX) $^

hello_world_from_bitcode: hello_world.bc
	clang$(CC_SUFFIX) $^ -o $@

hello_world_cat: hello_world.c
	cat-c $^ -o hello_world

invoke_template_opt_1: hello_world.bc
	opt$(CC_SUFFIX) -load ~/CAT/lib/CAT.* -CAT -disable-output $^

invoke_template_opt_2: hello_world.bc
	opt$(CC_SUFFIX) -load ~/CAT/lib/CAT.* -O3 -CAT -disable-output $^

invoke_template_opt_3: hello_world.bc
	opt$(CC_SUFFIX) -load ~/CAT/lib/CAT.* -O3 -disable-output $^

hello_world_opt.bc: hello_world.bc
	opt$(CC_SUFFIX) -load ~/CAT/lib/CAT.* -CAT -time-passes $^ -o $@

invoke_template_opt_4: hello_world.bc
	opt$(CC_SUFFIX) -load ~/CAT/lib/CAT.* -O3 -time-passes -disable-output $^

invoke_template_opt_5: hello_world.bc
	opt$(CC_SUFFIX) -load ~/CAT/lib/CAT.* -CAT --debug-pass=Structure -disable-output $^

cpp_API: hello_world.bc
	llc$(CC_SUFFIX) -march=cpp $^ -o CPP_APIs.cpp

clean:
	rm -f hello_world *.bc *.ll *.s hello_world_from_assembly a.out *.cpp hello_world_from_bitcode
