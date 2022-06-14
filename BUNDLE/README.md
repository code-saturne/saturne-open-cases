Tuble BUNDLE performance and scalability benchmark
==================================================

This case represents a cros-flow in a tube bundle similar to that of a steam
generator.

Both a coarse and fine mesh pattern are available, and each of those patterns
may be repeated in a periodic <x, y> pattern and extruded in the <z> direction
using code_saturne's built-in preprocessing operations.

The base 2D pattern of the fine mesh contains 100,040 cells, and that of the
coarse maesh 1,024 cells.

This allows building a series of meshes of varying sizes, adapted to weak
scaling measurement.

Case directories are named based on a `BENCH_`TL_N scheme, where:
- T is the type of mesh: `F` (fine) or `C` (coarse).
- L is the number of vertical layers (obtained by extrusion of Thebase layer)
- N is the number of motif copies in the x and y directions.

So `BENCH_F128_01`, with a base motif of 100040 cells, extruded over 128 layers,
contains 12,805,120 cells, where `BENCH_F128_02`, with a 2x2 pattern, contains
51,220,480 cells, and `BENCH_F128_04`, with a 4x4 pattern, contains
204,881,920 cells. More repeats are possible, and this case has been extended
to at least 3.2 billion cells.

Mesh generation
---------------

Scripts allowing generation and modification of the mesh using the SALOME
platform (www.salome-platform.org) are available in `MESH/Generation`, but
for easier use, pre-built coarse and fine single-layer meshes are
placed at `MESH/C/mesh_input.csm` and `MESH/F/mesh_input.csm` respectively.

These meshes can then be extruded to 16 and 128 layers respectively by
running `<install_path>/bin/code_saturne run` in the `BENCH_C016_PREPROCESS` and
`BENCH_F128_PREPROCESS` directories (or their `DATA` subdirectories).
This will produce a `BENCH_C016_PREPROCESS/RESU/extrude_16/mesh_output.csm`
for the coarse mesh, and `BENCH_F128_PREPROCESS/RESU/extrude_128/mesh_output.csm`
for the fine mesh.

The number of pattern repetitions is defined in each case's
`SRC/cs_user_mesh.c` user-defined sources file, through the `_n_tubes`
variable. The files are otherwise identical.

Running the cases
-----------------

Once these base meshes are generated, the predefined setups in the matching
`BENCH_`TL_N directories may be run, using the
`<install_path>/bin/code_saturne run` when no resource manager is used,
or that command is run from within a resource allocation, or
`<install_path>/bin/code_saturne submit` when a batch system is configured
in code_saturne's post-install step, and we are not already running under
a matching allocation.

Selecting a number of processes
-------------------------------

In each case's `DATA` directory, the code_saturne GUI may be run, or the
`run.cfg` file edited directly, so as to modify the resulting output directory
id (under that case's `RESU`subdirectory) and associated resouvce usage.
The `--id <output_id>`, `-n <procs>`, and `-nt <threads>` options may also
be passed directly to the `code_saturne run` or `code_saturne submit` commands.

Performance results are not extracted automatically at this stage:
the user should check the `performance.log`, `run_solver.log`, and
`timer_stats.csv` files for details (with the mosst relevant information
in the former).

Mesh numbering
--------------

Note that by default, this case uses the basic Morton curbe partitioning,
for deterministic results and robustness. Better performance may often
be obtained switching to a graph-based partitioning scheme.

Also, using the current 2-step mesh generation, mesh numbering might be different
in some cases than in older version of this benchmark, where the meshes
were extruded with SALOME's SMESH tool (requiring larger file transfers if
SALOME was not available on the user's platform).
