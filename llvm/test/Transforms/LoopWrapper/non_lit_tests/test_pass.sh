PATH_TO_LLVM_PROJECT=<PATH_TO_LLVM_PROJECT>

${PATH_TO_LLVM_PROJECT}/build/bin/clang -O0 -Xclang -disable-O0-optnone -emit-llvm -S tests.c -o /tmp/test.ll
${PATH_TO_LLVM_PROJECT}/build/bin/opt -passes="loop(loop-wrapper)" /tmp/test.ll -S -o /tmp/output.ll
${PATH_TO_LLVM_PROJECT}/build/bin/clang /tmp/output.ll -o /tmp/test
/tmp/test
