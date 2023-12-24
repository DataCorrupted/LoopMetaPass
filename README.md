# LoopMetaPass

For all the loops:

- Add metadata to the IR branch instruction representing the back edge for each loop
- Add metadata to the IR instruction that generates the loop induction variable

The metadata includes corresponding filename and line number in the C++ code.

## Usage

### Requirement

- LLVM 14+
- CMake 3.20+

### Build

```sh
cd build && cmake .. -GNinja && ninja
```

### Execution

The pass is written in old Pass Manager, remember to include `-enable-new-pm=0` when using.
```sh
opt -enable-new-pm=0 -load build/src/libLoopMetaPass.so --loop_meta_pass <bitcode>
```

## Corner cases

**`LoopPass` doesn't treat goto as a loop**

**Definition of [induction variable](https://en.wikipedia.org/wiki/Induction_variable) is unclear.**

We take LLVM's definition and only works with simple loop. 
`Loop.getInductionVariable(SE)` only finds plus one induction variable, and assumes that there is only one.
There can be multiple induction variables. E.g. `for (int i = 0, j = 0; i < a && j < b; i += 1, j += 1){}`.

If we take Wikipedia's definition, we may need special analysis to find all variables. 

- We can start by finding the header block, track all the PHI nodes in it.
- To determine if a PHI node is an induction variable, track if it has self increment/decrement.
- If a loop has `continue`, PHI may have more than two incoming, but all should have the same value except for the one from preheader. Otherwise, this PHI is not an induction variable. 

**The behavior of `continue`**

`continue` may directly create a backedge, or it can jump to a backedge block that takes care of all `continue`s.
After `LoopSimplify` there is always only one backedge.