# Deterministic search reproduction for the n=497 certificate

This document describes the experiment implemented by `reproduce_n497_search.cpp`.
It is separate from the proof obligation: the fixed certificate and exact checker are sufficient to verify the theorem. The reproducer is included so that the *search that found the certificate* can also be rerun from the base rule.

## Initial coloring

Use `n=497` and midpoint representatives

`x_i=(2i+1)/(2n)`.

The base color is exactly the midpoint discretization of the three-parameter `Recursive3Param` rule with

- `t1=3/8`
- `t2=17/32`
- `t=5/8`
- color `1` iff `z < z0(x,y)`.

All threshold comparisons are integer cross-products.

## Search window

Only vertices in

`W^3`, with `W={218,219,...,279}`,

are candidates for local flips. The window contains `62^3 = 238,328` vertices.

## Exact one-flip objective change

For a coloring `h`, define

`L1(b,c)=sum_a h(a,b,c)` and `R1(b,c)=sum_d h(b,c,d)`.

For a candidate vertex `v=(a,b,c)` with current color `x=h(v)`, the incident monochromatic counts are determined by `L1(a,b)` on incoming edges and `R1(b,c)` on outgoing edges.

Let

`same_in  = x ? L1(a,b) : n-L1(a,b)`

`same_out = x ? R1(b,c) : n-R1(b,c)`.

Then the exact change in the global monochromatic-edge count after toggling `v` is

`DeltaM = 2n - 2(same_in + same_out)`

except when `a=b=c`, where `+2` is added. The correction is required because the self-loop `v -> v` belongs to both incidence sums but remains monochromatic after toggling both copies of the same vertex.

No floating-point arithmetic is used.

## Deterministic move rule

At each step:

1. Evaluate the current exact `DeltaM` for all candidates in `W^3`.
2. Choose the candidate with the smallest `DeltaM` subject to `DeltaM < 0`.
3. Break ties lexicographically by `(a,b,c)`.
4. Toggle that vertex.
5. Stop when the minimum `DeltaM` is nonnegative.

A flip at `(a,b,c)` changes only `L1(b,c)` and `R1(a,b)`. Hence only candidates of the forms `(b,c,k)` and `(i,a,b)`, plus the flipped vertex itself, can have changed move values. The C++ implementation maintains these values with a priority queue and generation counters.

## Reproduced result

The deterministic run gives:

```text
n=497
window=[218,279]
candidate_vertices=238328
base_monochromatic=14715071917
flip_count=4750
cumulative_delta=-77600
next_best_delta=0
final_monochromatic=14714994317
exact_strict_improvement=TRUE
RESULT=PASS
```

The generated ordered list contains exactly `4750` flips and its JSON serialization is byte-for-byte identical to the canonical `n497_certificate.json`:

`SHA-256 = 41e62c73421e9ca33b097ecedda6d31b398ed8189d74444258c8015e197f193c`.

The next-best move has `DeltaM=0`, so the run stops at a local optimum for this exact candidate window and move rule; it is not stopped by an arbitrary flip budget.

## One-command audit

On Linux/GCC:

```bash
./reproduce_all.sh
```

On Windows PowerShell with `g++` on `PATH`:

```powershell
.\reproduce_all.ps1
```

The wrapper regenerates the certificate, requires a byte match with the canonical JSON, and then reruns both exact certificate checkers.
