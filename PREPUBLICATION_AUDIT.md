# Prepublication audit - GitHub release candidate

Status date: 2026-09-09.

## Prior-art and source provenance

- Fresh prior-art screen completed before drafting; no newer public upper bound below the candidate was found in that screen.
- Published exact comparison value rechecked directly in the authors' Lean repository at pinned commit `aa04b54db9a48d7a5084e263c147607af7744498`: PASS.
  - `Distributed2Coloring/UpperBound/Recursive3Param.lean` defines `t=5/8`, `t1=3/8`, `t2=17/32`.
  - `Distributed2Coloring/UpperBound/Recursive3Param/Final.lean` proves the exact equality `p_recursive3ParamAlg = 94835/393216`.
  - `Distributed2Coloring/MainResults.lean` packages the public bound `pStar < 0.24118`; it is not the file containing the exact rational equality.

## Exact finite certificates

- `n=497` exact certificate checker: PASS.
- `n=281` no-flip exact checker: PASS.
- Main strict cross-product inequality: PASS.
- Canonical `n497_certificate.json` SHA-256: `41e62c73421e9ca33b097ecedda6d31b398ed8189d74444258c8015e197f193c`.
- Embedded checker SHA-256: `06280be157851220138e8fec38d2852acda1b59543a0838040d57a3e171cf582`.

## Full n=497 search reproduction

The local-search experiment is packaged as `reproduce_n497_search.cpp`.

Deterministic specification:

- start state: midpoint discretization at `n=497`;
- candidate window: `[218,279]^3`, 238,328 vertices;
- move: choose the candidate flip with minimum exact `Delta M < 0`;
- tie break: lexicographically smallest `(a,b,c)`;
- arithmetic: integer only;
- stopping rule: no negative `Delta M` remains.

Reproduced result:

- base monochromatic count `14,715,071,917`;
- exactly `4750` flips;
- cumulative `Delta M = -77,600`;
- final monochromatic count `14,714,994,317`;
- next-best `Delta M = 0`;
- strict exact improvement: TRUE.

## Independent GitHub Actions reproduction

GitHub Actions run #1 (`Build reproducible artifacts`) completed successfully on Ubuntu 24.04. The clean runner:

1. compiled `reproduce_n497_search.cpp`;
2. regenerated `n497_certificate.json` from the base rule;
3. verified its SHA-256 against the canonical certificate;
4. regenerated `check_n497_embedded_certificate.cpp` from the certificate and verified its canonical SHA-256;
5. compiled and ran both exact checkers and diffed their output against the frozen logs;
6. rebuilt the manuscript with two `pdflatex` passes;
7. committed the generated certificate, embedded checker, and PDF back to `main`.

All seven steps passed. The GitHub-runner PDF has 8 pages and SHA-256

`058d1b0aaef1ae36809cc22c30240c879de3033880d2e2527a851e2fc479c5f7`.

The earlier local v0.5 PDF used a different TeX environment and has a different binary hash. The final publication PDF will be rebuilt after permanent identifiers are inserted, so neither draft-build hash is treated as the final frozen paper hash.

## Current permanent locator status

Public GitHub repository: `yonezaemon1-hub/finite-de-bruijn-certificate-one-round-2-coloring`.

The repository now exists and contains the source, deterministic search reproduction, canonical finite certificate, exact checkers, frozen logs, licenses, metadata, and build workflow. A Zenodo Software DOI and Paper DOI have not yet been assigned.

The current manuscript still contains a draft-local artifact locator in Section 6. Before public release, that text must be replaced by the permanent GitHub repository and Zenodo Software DOI, followed by a fresh PDF build/hash and final prior-art / claim / proof / citation / publication preflight.

## Publication status

**NOT YET PUBLICATION PASS.**

Remaining blocker: reserve/publish the archival identifiers, insert them into the manuscript and metadata, rebuild, rerun the exact reproduction/audits, and only then freeze `v1.0.0`.
