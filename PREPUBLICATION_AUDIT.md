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

## Permanent identifiers and metadata

Public GitHub repository: `yonezaemon1-hub/finite-de-bruijn-certificate-one-round-2-coloring`.

Reserved archival identifiers for the planned `v1.0.0` release:

- Paper DOI: `10.5281/zenodo.22670656`.
- Software DOI: `10.5281/zenodo.22669456`.

Both DOI values are now inserted into the manuscript source. The paper DOI is recorded in `CITATION.cff`; both identifiers are recorded in `paper.publish.json`; and `.zenodo.json` links the software package to the paper DOI with an `isSupplementTo` related identifier. The manuscript artifact-availability section now points to the public repository and the two reserved archival identifiers rather than to a draft-local bundle.

## Independent GitHub Actions reproduction after DOI insertion

GitHub Actions run #2 (`Build reproducible artifacts`) completed successfully on Ubuntu 24.04 after the DOI and permanent-locator edits. The clean runner:

1. compiled `reproduce_n497_search.cpp`;
2. regenerated `n497_certificate.json` from the base rule and verified its canonical SHA-256;
3. regenerated `check_n497_embedded_certificate.cpp` and verified its canonical SHA-256;
4. compiled and ran both exact checkers and diffed their output against the frozen logs;
5. rebuilt the manuscript with two `pdflatex` passes;
6. produced the DOI-bearing manuscript PDF;
7. committed the generated PDF back to `main`.

All stages passed. The authoritative hash of the final release-candidate PDF and the accompanying text/source files is recorded in `SHA256SUMS.txt` after the final manifest refresh rather than duplicated here.

## Claim-boundary status

- New finite upper-bound certificate: supported.
- Exact integer verification: supported.
- Deterministic search reproduction: supported.
- New De Bruijn reduction: not claimed.
- General novelty of local/heuristic search: not claimed.
- Optimality of the `n=497` coloring or exact value of `p_normal(497)`: not claimed.
- Improved lower bound or optimality of `p*`: not claimed.
- Peer review: not performed.

## Publication status

**PUBLICATION NOT YET PASS.**

The mathematical, source, reproduction, identifier-insertion, and DOI-bearing PDF-build gates have passed. Remaining publication steps are: refresh and verify the final integrity manifest; freeze GitHub `v1.0.0`; publish the Zenodo Software and Paper records; and perform the final DOI / Related works cross-check. Because the GitHub repository is connected to Zenodo while a Software DOI has already been reserved manually, the GitHub-release / Zenodo-draft interaction must be checked before creating the public GitHub release so that a duplicate software record is not created.
