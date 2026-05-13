WINDSOR mesh
============

This directory holds the computational mesh `c1g1.cgns` used by both
`RANS/` and `DDES/`. The mesh is referenced through code_saturne's
study-level `MESH/` convention (no explicit path in `setup.xml`).

Mesh characteristics
--------------------

| Quantity              | Value                          |
|-----------------------|--------------------------------|
| File                  | `c1g1.cgns`                    |
| Format                | CGNS, single block, polyhedral |
| Cell count            | ~6.3 M                         |
| Wall y+ on the body   | < 1 (resolved)                 |
| Domain                | Loughborough wind tunnel test section, full-scale |
| Vehicle               | Windsor squareback, yawed by -2.5° around z       |

Boundary zones defined in the mesh and referenced in `setup.xml`:

| XML label      | CGNS zone family                  | BC type   |
|----------------|-----------------------------------|-----------|
| Inlet          | `Flow.CFDWT.In`                   | inlet     |
| Outlet         | `Flow.CFDWT.Out`                  | outlet    |
| Floor          | `Flow.CFDWT.Floor`                | wall      |
| Roof           | `Flow.CFDWT.Roof`                 | symmetry  |
| Left / Right   | `Flow.CFDWT.{Left,Right}`         | symmetry  |
| Windsor_Body   | `Flow.Windsor_Square_nW.Body`     | wall      |
| Windsor_Base   | `Flow.Windsor_Square_nW.Base`     | wall      |
| Windsor_Pins   | `Flow.Windsor_Pins.Surface`       | wall      |

Obtaining the mesh
------------------

The mesh file (~1.2 GB) is not committed to the repository. Download it
with the top-level script:

```bash
cd ..
./fetch_data.sh
```

This places `c1g1.cgns` at `WINDSOR/MESH/c1g1.cgns`, where code_saturne
will pick it up automatically.

Geometry source
---------------

The geometry follows the AutoCFD5 Case 1 specification (Windsor squareback,
floor pins, mid-wheelbase moment reference). The original CAD/EngSketchPad
`.csm` source (~8 GB) is not redistributed here; the workshop spec is
self-contained for anyone wishing to regenerate the geometry. See the
AutoCFD5 workshop description for dimensions and tunnel layout.
