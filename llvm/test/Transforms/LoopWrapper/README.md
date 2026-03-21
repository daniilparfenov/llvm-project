# LoopWrapper Pass Testing

This document describes the methodology for testing the `LoopWrapper` pass, which wraps loops with `loop_start()` and `loop_end()` function calls.

## 1. General Testing Concept

Testing is performed using two approaches:
1. **Functional Verification (C code):** Demonstrates that the transformed program compiles correctly and produces the expected execution log.
2. **Lit Tests:** Automated LLVM IR-level verification to ensure correct instruction insertion into the Control Flow Graph (CFG).

---

## 2. Functional Testing (C code)

We use C source code (`non_lit_tests/tests.c`) for visual verification. 
A dedicated script, `non_lit_tests/test_pass.sh`, automates the entire process: it translates C to IR, runs the `LoopWrapper` pass, compiles the resulting IR into an executable, and runs it.

### Expected Execution Output:
With a correctly functioning pass, the output should be as follows:

- **Simple Loop:** `start` is called once before the loop, `body` is called twice, and `end` is called once after completion.
```
TEST: simple_loop
loop_start_call
loop_body
loop_body
loop_end_call
```

- **Nested Loops:** The outer loop calls `start`/`end` once. The inner loop calls its own `start`/`end` markers on every iteration of the outer loop.
*(Example for `nested_loops(2, 1)`)*:
```
TEST: nested_loops
loop_start_call   // Outer Start
loop_start_call   // Inner Iter 1 Start
loop_body
loop_end_call     // Inner Iter 1 End
loop_start_call   // Inner Iter 2 Start
loop_body
loop_end_call     // Inner Iter 2 End
loop_end_call     // Outer End
```

- **Multiple Exits:** `start` is called once before the loop, `body` is called once (based on condition), and `end` is called exactly once upon exiting.
```
TEST: multi_exit
loop_start_call
loop_body
loop_end_call
```

---

## 3. Creating Lit Tests (IR-level)

### Step 1: Generating Reference IR
First, we generate a reference IR from the C source (located in `c_base`) while preserving variable names, and save it to the `ir_base` directory:
```bash
clang -O1 -S -emit-llvm -fno-discard-value-names test.c -o test.ll
```

### Step 2: Adding RUN Directives
A `RUN` command is added to the beginning of the IR files in `ir_base` to specify how the test should be executed:
```llvm
; RUN: opt -passes='loop(loop-wrapper)' %s -S | FileCheck %s
```

### Step 3: Autogenerating Checks via Python
To avoid writing `CHECK` assertions manually, we use the standard LLVM utility script:
```bash
python3 llvm/utils/update_test_checks.py --opt build/bin/opt basic.ll
```

---

## 4. Running Tests

To execute all lit tests, run the following command:
```bash
build/bin/llvm-lit llvm/test/Transforms/LoopWrapper/lit_tests/*
```
