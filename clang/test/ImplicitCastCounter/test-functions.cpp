// First, define the RUN command. 
// It runs clang with our plugin, processes this file (%s), 
// redirects stderr to stdout (2>&1), and pipes the output to FileCheck.
//
// RUN: %clang -fplugin=%llvmshlibdir/ImplicitCastCounter.so -fsyntax-only %s 2>&1 | FileCheck %s

// Target code for analysis
double sum(int a, float b) {
	return a + b;
}

int mul(float a, float b) {
	return a + sum(a, b);
}

// Now, define the checks (the expected console output).
// FileCheck reads the output line by line and matches these strings.
// Note: Both functions and their internal casts must be listed in alphabetical order

// CHECK: Function `mul`
// CHECK-NEXT: double (int, float) -> double (*)(int, float): 1
// CHECK-NEXT: double -> int: 1
// CHECK-NEXT: float -> double: 1
// CHECK-NEXT: float -> int: 1

// CHECK: Function `sum`
// CHECK-NEXT: float -> double: 1
// CHECK-NEXT: int -> float: 1
