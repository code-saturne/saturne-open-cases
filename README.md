# saturne-open-cases

Open application and benchmark cases for
[code_saturne](https://code-saturne.org), EDF R&D's general-purpose
finite-volume CFD solver.

The intent of this repository is to make a small set of fully runnable
cases available outside the main code_saturne distribution: configured
`DATA/`, user-defined `SRC/`, and the documentation needed to launch
them with the standard `code_saturne run` workflow.

Studies
-------

| Study                | Description                                                                                                                                   |
|----------------------|-----------------------------------------------------------------------------------------------------------------------------------------------|
| [BUNDLE](BUNDLE/)    | Cross-flow in a tube bundle (steam-generator-like geometry). Used as a weak/strong scaling benchmark; mesh sizes from ~1 M to multi-billion cells. |
| [WINDSOR](WINDSOR/)  | External aerodynamics around the Windsor squareback at 2.5° yaw (AutoCFD5 Case 1). RANS and DDES configurations on a ~6.3 M-cell mesh.        |

Each study directory contains its own `README.md` with the physical
description, numerical setup and run instructions. Cases follow the
standard code_saturne layout (`DATA/`, `SRC/`, `RESU/`).

Requirements
------------

- code_saturne (development built recommended for the largest meshes);
  the cases here are tested with the 9.x series.
- For WINDSOR, ~1.2 GB of disk space for the mesh downloaded by
  `WINDSOR/fetch_data.sh`. For BUNDLE, mesh sizes scale with the
  preprocess step.
