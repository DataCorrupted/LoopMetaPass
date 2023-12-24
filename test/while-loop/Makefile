all:
	clang++ -g -Xclang -load -Xclang ../../build/src/libLoopMetaPass.so while-loop.cpp -S -emit-llvm -o with-pass.ll
	clang++ -g a.cpp -S -emit-llvm -o origin.ll 