# Internal proofreading status - v0.5

- Fresh prior-art screen: completed before drafting; no newer public upper bound below the candidate was found in that screen.
- Published exact comparison value rechecked directly in the authors' Lean repository at pinned commit `aa04b54db9a48d7a5084e263c147607af7744498`: PASS.
  - `Distributed2Coloring/UpperBound/Recursive3Param.lean` defines `t=5/8`, `t1=3/8`, `t2=17/32`.
  - `Distributed2Coloring/UpperBound/Recursive3Param/Final.lean` proves the exact equality `p_recursive3ParamAlg = 94835/393216`.
  - `Distributed2Coloring/MainResults.lean` packages the public bound `pStar < 0.24118`; it is not the file containing the exact rational equality.
- n = 497 fixed certificate exact checker: PASS.
- n = 281 no-flip certificate exact checker: PASS.
- Main cross-product strict inequality: PASS.

## Full n=497 search reproduction

The local-search experiment is packaged as `reproduce_n497_search.cpp`.

Deterministic specification:

- start state: midpoint discretization at `n=497`;
- candidate window: `[218,279]^3`, 238,328 vertices;
- move: choose the candidate flip with minimum exact `Delta M < 0`;
- tie break: lexicographically smallest `(a,b,c)`;
- arithmetic: integer only;
- stopping rule: no negative `Delta M` remains.

Independent reconstruction and compiled C++ rerun both produced:

- base monochromatic count `14,715,071,917`;
- exactly `4750` flips;
- cumulative `Delta M = -77,600`;
- final monochromatic count `14,714,994,317`;
- next-best `Delta M = 0`;
- strict exact improvement: TRUE;
- generated certificate SHA-256 `41e62c73421e9ca33b097ecedda6d31b398ed8189d74444258c8015e197f193c`;
- generated certificate vs canonical `n497_certificate.json`: BYTE-FOR-BYTE MATCH / PASS.

The Linux one-command wrapper additionally reran both exact checkers and ended with `FULL_REPRODUCTION=PASS`.

Thus:

- fixed-certificate reproducibility: PASS;
- full experimental-search reproducibility from the base rule: PASS.

The search is deterministic and does not read the canonical flip list. No random seed, floating-point tolerance, solver, or network access is needed.

## Manuscript preflight

- LaTeX compilation, two passes: PASS.
- Undefined references/citations after final pass: 0.
- Overfull/underfull box warnings: 0.
- PDF pages: 8.
- Final PDF render: PASS in `pdftoppm`-based inspection.
- v0.4 -> v0.5 render diff: pages 1-4 pixel-identical; pages 5-8 changed as expected from the added search-reproducibility text and bundle-version locator.
- Changed pages visually inspected: PASS; no clipping, overlap, broken glyphs, or accidental layout damage observed.
- Final PDF SHA-256: `839e2ad4773a91caa39615d20d880f14c5f6cd571f4d7f3a822cdd8cdebb088a`.

## Reproducibility locator status

The v0.5 PDF uses the proofreading bundle `Yonezu_Paper11_Proofreading_Bundle_v0.5.zip` as the temporary artifact locator. A permanent public GitHub repository and Zenodo software DOI do not yet exist and therefore are not fabricated. Adding those permanent identifiers remains mandatory before public release.

## Publication status

**NOT YET PUBLICATION PASS.** Remaining release blocker: create the public artifact repository / archival record, replace the draft-local locator in Section 6 with permanent GitHub and Zenodo identifiers, and rerun the final prior-art / claim / proof / citation / publication preflight.
