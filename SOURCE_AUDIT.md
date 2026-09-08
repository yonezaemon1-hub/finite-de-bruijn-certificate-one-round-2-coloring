# Source audit for the Flin et al. comparison baseline

Pinned repository snapshot:

- repository: `suomela/2-coloring-1-round`
- commit: `aa04b54db9a48d7a5084e263c147607af7744498`

Verified facts:

1. `Distributed2Coloring/UpperBound/Recursive3Param.lean`
   - `t = 5/8`
   - `t1 = 3/8`
   - `t2 = 17/32`

2. `Distributed2Coloring/UpperBound/Recursive3Param/Final.lean`
   - theorem `p_recursive3ParamAlg_eq`
   - exact equality `ClassicalAlgorithm.p recursive3ParamAlg = ENNReal.ofReal (94835 / 393216 : ℝ)`

3. `Distributed2Coloring/MainResults.lean`
   - public-facing theorem `pStar_lt_24118`
   - this file packages the `< 0.24118` result; it does not itself state the exact `94835/393216` equality.

Manuscript v0.5 retains the full repository-relative paths and file-level provenance explicitly.

Additional repository-tree verification (2026-09-09):

- `main` points exactly to commit `aa04b54db9a48d7a5084e263c147607af7744498`.
- The recursive Git tree at that commit explicitly contains:
  - `Distributed2Coloring/UpperBound/Recursive3Param.lean`
  - `Distributed2Coloring/UpperBound/Recursive3Param/Final.lean`
  - `Distributed2Coloring/UpperBound/Recursive3Param/Value.lean`
- The two manuscript-cited files were fetched directly from the repository at the pinned snapshot, confirming that the paths are not inferred or fabricated. An independent proofreading pass additionally verified the same snapshot by downloading and expanding the pinned commit tarball.

The manuscript bibliography now spells out the complete repository-relative paths to remove any ambiguity.
