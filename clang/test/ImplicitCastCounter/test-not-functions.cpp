// RUN: %clang -fplugin=%llvmshlibdir/ImplicitCastCounter.so -fsyntax-only %s 2>&1 | FileCheck %s

int double_to_int = 5.5;
int float_to_int = 3.f;
int another_double_to_int = 4.4;

double sum(int a, float b, float c, int d) {
	return a + b + c + d;
}

// CHECK: Function `sum`
// CHECK-NEXT: float -> double: 1
// CHECK-NEXT: int -> float: 2

// CHECK: Not a Function
// CHECK-NEXT: double -> int: 2
// CHECK-NEXT: float -> int: 1
