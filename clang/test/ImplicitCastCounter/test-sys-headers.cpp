// Use the -isystem %S/SysHeaderDummy flag to tell Clang to treat everything in the SysHeaderDummy directory as system headers.
// RUN: %clang -fsyntax-only -fplugin=%llvmshlibdir/ImplicitCastCounter.so -isystem %S/SysHeaderDummy %s 2>&1 | FileCheck %s

#include <sys_header_dummy.h>

// The test verifies that the plugin ignores implicit castes in the system headers

int user_math(float b) {
    // User implicit cast: float -> int
    int x = b; 

    // Sys implicit cast: int -> float
    float y = do_sys_math(x);
    
    return x;
}

// CHECK-NOT ensures that the plugin does not output the 'do_sys_math' function before our block
// CHECK-NOT: Function `do_sys_math`

// CHECK: Function `user_math`
// CHECK-NEXT: float (int) -> float (*)(int): 1
// CHECK-NEXT: float -> int: 1

// Just in case, double-check that the system function is not printed after our block either
// CHECK-NOT: Function `do_sys_math`
