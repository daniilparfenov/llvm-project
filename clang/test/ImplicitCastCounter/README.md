
### Test Cases Description
The test suite covers three main scenarios:
1) **test-functions.cpp**: checks that implicit castes (e.g., int to float) are correctly counted and grouped by function name. Also verifies that output is sorted alphabetically;
2) **test-not-functions.cpp**: verifies that implicit castes are detected not only inside functions (this test case uses global variable initializations) and are grouped into a special 'Not a Function' category;
3) **test-sys-headers.cpp**: checks that implicit castes are not counted in system headers;

### How to run tests
Ensure llvm-lit is in your PATH and ImplicitCastCounter plugin is built. To run all LIT tests for ImplicitCastCounter clang AST plugin, run the following command in the current directory:
```bash
llvm-lit ./test-*.cpp
```
