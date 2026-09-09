# A Finite De Bruijn Certificate Improving the Upper Bound for One-Round Randomized 2-Coloring of Cycles

Ryutaro Yonezu, Independent Researcher

Status: preprint release candidate / not peer reviewed.

Archival identifiers for the planned `v1.0.0` release:

- Paper DOI: `10.5281/zenodo.22670656`
- Software DOI: `10.5281/zenodo.22669456`

These identifiers are embedded before the `v1.0.0` freeze so that the tagged source, manuscript PDF, citation metadata, and Zenodo metadata agree.

## Main result

For the one-round randomized 2-coloring problem on cycles in the De Bruijn formulation of Flin, Raevskaya, Stimpert, Suomela, and Yang, this repository provides an explicit finite coloring certificate at `n=497` with

`p* <= 14714994317 / 61013446081 = 0.2411762531404098...`

which is strictly below the exact value of the previously published Lean-certified construction,

`94835 / 393216 = 0.2411778767903646...`.

The exact cross-product check is

`14714994317*393216 - 94835*61013446081 = -38953738163 < 0`.

A supporting no-flip midpoint certificate at `n=281` also gives a strict improvement.

## Full experimental reproduction

The `n=497` refinement can be reproduced from the base rule without reading the canonical flip list.

The deterministic search:

1. constructs the midpoint discretization at `n=497`;
2. restricts candidate flips to `[218,279]^3`;
3. chooses the flip with minimum exact integer `Delta M < 0`;
4. breaks ties lexicographically by `(a,b,c)`;
5. stops when no negative move remains.

It terminates naturally after exactly `4750` flips:

- base monochromatic edges: `14,715,071,917`
- cumulative change: `-77,600`
- final monochromatic edges: `14,714,994,317`
- next-best `Delta M`: `0`

The regenerated JSON is byte-for-byte identical to `n497_certificate.json`.

## One-command reproduction

Linux / GCC:

```bash
chmod +x reproduce_all.sh
./reproduce_all.sh
```

Windows / PowerShell with `g++` on `PATH`:

```powershell
.\reproduce_all.ps1
```

A successful full run ends with:

`FULL_REPRODUCTION=PASS`

No random seed, floating-point tolerance, external solver, or network access is required.

## Files

- `paper.tex` — manuscript source
- `Yonezu_2026_Finite_De_Bruijn_Certificate_One_Round_2_Coloring.pdf` — compiled manuscript release candidate
- `reproduce_n497_search.cpp` — deterministic search reproducer
- `n497_certificate.json` — canonical `n=497` certificate
- `check_n497_embedded_certificate.cpp` — exact certificate checker
- `check_n281_base.cpp` — exact no-flip `n=281` checker
- `reproduce_all.sh` / `reproduce_all.ps1` — full reproduction wrappers
- `SEARCH_REPRODUCTION.md` — exact search specification
- `SOURCE_AUDIT.md` — pinned-source audit of the Lean baseline
- `PREPUBLICATION_AUDIT.md` — current release audit status
- `*_log.txt` — frozen expected outputs
- `SHA256SUMS.txt` — file hashes

## Claim boundary

This work claims only a new finite upper-bound certificate within the De Bruijn framework of Flin et al. It does not claim a new De Bruijn reduction, novelty of heuristic search as a general method, optimality of the `n=497` coloring, an exact value of `p_normal(497)`, an improved lower bound, or optimality of `p*`.

## Licenses

- Manuscript text and PDF: CC BY 4.0.
- Source-package software, search code, and checkers: MIT License unless otherwise noted.
