# tomasulo-simulator

## About
A simulator that implements the Tomasulo algorithm to execute floating-point instructions and track execution status across clock cycles.

## Features
- Supports floating-point instructions: `L.D`, `S.D`, `ADD.D`, `SUB.D`, `MUL.D`, `DIV.D`.
- Optional integer instructions: `DADDIU`, `BNEZ`.
- Simulates with 3 adders, 2 multipliers, 2 load buffers, 2 store buffers.
- 16 floating-point (`F0`~`F30`) and 16 integer registers (R1=16, others 0).
- Memory: 8 double-precision spaces (64 bytes), initial value 1.
- Tracks instruction, buffers, reservation stations, and registers at each clock cycle.
- Outputs results to `output.txt`.

## Input
- Reads instructions from text files, e.g., `test1.txt`:

```text
L.D F6, 8(R2)
L.D F2, 40(R3)
MUL.D F0, F2, F4 
SUB.D F8, F6, F2
DIV.D F10, F0, F6
ADD.D F6, F8, F2
```

## Output

Example output at a given clock cycle:

```text
Clock Cycle: 1
Instruction Status:
L.D F6, 8(R2)  Issue: 1  Exec: -  Write: -
L.D F2, 40(R3) Issue: 1  Exec: -  Write: -

Load Buffer:
LB1: F6, Busy
LB2: F2, Busy

Store Buffer:
SB1: -, Free
SB2: -, Free

Reservation Stations:
Add1: -, Free
Add2: -, Free
Mul1: -, Free

Register Result Status:
F0: -, Free
F2: -, Waiting
```

## References

- [Tomasulo algorithm - Wikipedia](https://en.wikipedia.org/wiki/Tomasulo_algorithm) 
