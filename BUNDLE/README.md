Tuble BUNDLE performance and scalability benchmark
==================================================

This case represents a cross-flow in a tube bundle similar to that of a steam
generator.

Both a coarse and fine mesh pattern are available, and each of those patterns
may be repeated in a periodic <x, y> pattern and extruded in the <z> direction
using code_saturne's built-in preprocessing operations.

The base 2D pattern of the fine mesh contains 100,040 cells, and that of the
coarse mesh 1,024 cells.

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
id (under that case's `RESU` subdirectory) and associated resource usage.
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

Mesh periodicity
----------------

By default, the case uses a periocity mesh condition in the _y_ direction,
which implies the use of ghost cell value exchanges even with a single MPI rank.
Setting the `CS_BENCH_NO_PERIODICITY` environment variable to 1 forces a switch
to a symmetry condition instead.

Linear solver choice for pressure solution
------------------------------------------

Some environment variables may be used to modify settings for the pressure
resolution, which uses a multigrid preconditioner by default.

To activate those settings the `CS_BENCH_SLES_TYPE` variable must be set.

| Environment variable      | Value            | Description |
|---------------------------|------------------|------------ |
| `CS_BENCH_SLES_TYPE`      |  `native_fcg_mg` | Use flexible conjugate gradient preconditioned by multigrid solver (default). |
|                           |  `hypre` | Use HYPRE solver if available in build. |
| `CS_BENCH_SLES_VERBOSITY` |  integer | Force verbosity level for this solver.

### Settings for multigrid preconditioner.

When using the (default) multigrid preconditioner, many additional settings may be used.

| Environment variable | Value      | Description |
|----------------------|------------|-------------|
| `CS_BENCH_MG_TYPE`   |  `v_cycle` | Use V-cycle for multigid (default). |
|                  `   |  `k_cycle` | Use K-cycle for multigid. |
| `CS_BENCH_MG_DESCENT_SMOOTHER_TYPE` | `pcg`, `fcg`, `jacobi`, `gs`, `sgs`, `tfgs`, `tbgs` | Descent smoother type (default: sgs on CPU, jacobi on GPU) |
| `CS_BENCH_MG_ASCENT_SMOOTHER_TYPE`  | same as above | Ascent smoother type (default: sgs on CPU, jacobi on GPU) |
| `CS_BENCH_MG_COARSE_SOLVER_TYPE`    | Same as above | Coarse solver type (default: pcg) |
| `CS_BENCH_MG_DESCENT_SMOOTHER_N_ITER` | integer | number of iterations for descent smoother (defaut: 1 with sgs, 2 with jacobi) |
| `CS_BENCH_MG_ASCENT_SMOOTHER_N_ITER`  | integer | number of iterations for ascent smoother (defaut: 1 with sgs, 4 with jacobi) |
| `CS_BENCH_MG_COARSE_SOLVER_N_ITER` | integer | max number of iterations for coarse level solver (500 for pcg, 1 for sgs) |
| `CS_BENCH_MG_COARSE_SOLVER_TEST_CVG` | real number | precision multiplier for coarse solver (no test if negative; default 1.0 for pcg, -1.0 for sgs) |
| `CS_BENCH_MG_COARSENING_FINE_LV` | integer | level above which specific fine-level settings are used (default: -1, unused). |
| `CS_BENCH_MG_COARSENING_TYPE` | `dx`, `mx`, `pw`, or `dx_<n>`, `mx_<n>`, `pw_<n>` | Coarsening type (default: dx_3). The optional postfix indicates the max aggregation count per row, and is useful mainly for the recursive pairwise algorithm (where pw == pw_4, and pw_8 is more aggressive).
| `CS_BENCH_MG_COARSENING_TYPE_FG` | same as avove | As above, but specific to fine levels. |
| `CS_BENCH_MG_MAX_LEVELS` | integer | Maximum number of grid levels (default: 25) |
| `CS_BENCH_MG_MERGE_STEP` | integer | Merge grids on successive MPI ranks by given step, so as to use a smaller MPI communicator for coarse grids. |
| `CS_BENCH_MG_MERGE_THRESHOLD` | integer | Mean number of rows under which grids are merged (default: 600). |
| `CS_BENCH_MG_MERGE_THRESHOLD_GLOBAL` | integer | Global number of rows under which grids are merged (default: 600). |
| `CS_BENCH_MG_MAX_LEVEL_DEVICE` | integer | Specify finest level that should be run on the GPU. All coarser levels smoothes and solves are run on the CPU (default: 1). |
| `CS_BENCH_MG_DESCENT_SMOOTHER_TYPE_D` | `jacobi` or `fcg` | Descent smoother type for device. |
| `CS_BENCH_MG_ASCENT_SMOOTHER_TYPE_D` | `jacobi` or `fcg` | Ascent smoother type for device. |
| `CS_BENCH_MG_DESCENT_SMOOTHER_N_ITER_D` | integer | Number of iterations for descent smoother on device. |
| `CS_BENCH_MG_ASCENT_SMOOTHER_N_ITER_D` | integer | Number of iterations for ascent smoother on device. |

### Mitigation options for MPI communication latency.

When running on networks with higher latency, using the default settings, a large
amount of time may be spent solving the coarsest level grid.
Several options can be used to try to mitigate this:

- Setting `CS_BENCH_MG_COARSE_SOLVER_TYPE=sgs` (or `jacobi` on GPU).
  With this setting, unless further modified, a single iteration will be run on the coarsest grid. As this solution is not converged, more iterations will be necessary for the high level solver (flexible conjugate gradient), but less time will be spent on the coarsest solver.

- Setting `CS_BENCH_MG_COARSE_MG_MERGE_STEP` to a value > 1. This may also require adjusting `CS_BENCH_MG_MERGE_THRESHOLD` or `CS_BENCH_MG_MERGE_THRESHOLD` if the coarsest grid exceeds the default threshold. The larger the merge step, the less levels will be required to (ideally) reach a single rank for the coarsest level solver, but the larger (and more costly) the local intermediate systems will be.

  For example, if a rank step of 8 is used, a merged level on 1 of 8 ranks will have 8 time as many rows as it would have otherwise, while the 7 other ranks are idle. So if we use the default coarsening argument, where we can expect a factor of 3 fewer rows per level, merging after 5 levels would lead to rows of mean size approximately n, n/3, n/9, n/27, and approximately 0.1n (n/81*8). This is recursive, so when the threshold is reached again, further merging occurs, until the coarsest grid occupies a single rank, or coarsening does not reduce the grid size anymore (i.e. the last grid is more than 80% the size of the previous one).

  Activating this option may lead to deeper grid hierarchies, which are not always beneficial to the convergence behavior.

Partitioning
------------

By default, this case is set to use the Morton space-filling curve based
partitioning, which is always available and robust. This can also be changed
using an environment variable.

| Environment variable        | Value      | Description |
|-----------------------------|------------|-------------|
| `CS_BENCH_PARTITION_TYPE`   |  `morton_box`, `morton_cube`, `hilbert_box`, `hilbert_cube`, `scotch`, `metis` | Partition algorithm choice (if available in build for Scotch and (Par)Metis). |
| `CS_BENCH_PARTITION_RANK_STEP` | integer | If > 1, distribute partitioning over n_ranks/rank_step MPI ranks. This may help work around robustness issues for graph-based partitioners such as PT-Scotch or ParMetis.
