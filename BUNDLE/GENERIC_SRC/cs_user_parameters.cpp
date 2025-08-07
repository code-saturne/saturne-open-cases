/*============================================================================
 * User functions for input of calculation parameters.
 *============================================================================*/

/* VERS */

/*
  This file is part of code_saturne, a general-purpose CFD tool.

  Copyright (C) 1998-2025 EDF S.A.

  This program is free software; you can redistribute it and/or modify it under
  the terms of the GNU General Public License as published by the Free Software
  Foundation; either version 2 of the License, or (at your option) any later
  version.

  This program is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
  details.

  You should have received a copy of the GNU General Public License along with
  this program; if not, write to the Free Software Foundation, Inc., 51 Franklin
  Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

/*----------------------------------------------------------------------------*/

#include "cs_defs.h"

/*----------------------------------------------------------------------------
 * Standard C library headers
 *----------------------------------------------------------------------------*/

#include <assert.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

#if defined(HAVE_MPI)
#include <mpi.h>
#endif

/* Avoid warnings due to previous values */
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_URL
#undef PACKAGE_VERSION

/*----------------------------------------------------------------------------
 * PLE library headers
 *----------------------------------------------------------------------------*/

#include <ple_coupling.h>

/*----------------------------------------------------------------------------
 * Local headers
 *----------------------------------------------------------------------------*/

#include "cs_headers.h"

/*----------------------------------------------------------------------------*/

BEGIN_C_DECLS

/*----------------------------------------------------------------------------*/
/*!
 * \file cs_user_parameters.cpp
 *
 * \brief User functions for input of calculation parameters.
 *
 * See \ref parameters for examples.
 */
/*----------------------------------------------------------------------------*/

/*============================================================================
 * User function definitions
 *============================================================================*/

/*----------------------------------------------------------------------------*/
/*!
 * \brief Define linear solver options.
 *
 * This function is called at the setup stage, once user and most model-based
 * fields are defined.
 *
 * Available native iterative linear solvers include conjugate gradient,
 * Jacobi, BiCGStab, BiCGStab2, and GMRES. For symmetric linear systems,
 * an algebraic multigrid solver is available (and recommended).
 *
 * External solvers may also be setup using this function, the cs_sles_t
 * mechanism allowing such through user-define functions.
 */
/*----------------------------------------------------------------------------*/

void
cs_user_linear_solvers(void)
{
  const char *s_sles_type = getenv("CS_BENCH_SLES_TYPE");
  if (s_sles_type == nullptr)
    s_sles_type = "native_fcg_mg";

  int verbosity = -1;
  const char *s_sles_verbosity = getenv("CS_BENCH_SLES_VERBOSITY");
  if (s_sles_verbosity != nullptr) {
    verbosity = atoi(s_sles_verbosity);
  }

  /* FCG with multigrid preconditionner
     ---------------------------------- */

  if (strstr(s_sles_type, "native_fcg_mg") != nullptr) {

    cs_sles_it_t *c = cs_sles_it_define(CS_F_(p)->id,
                                        nullptr,
                                        CS_SLES_FCG,
                                        -1,
                                        2000);

    /* Multigrid type */

    cs_multigrid_type_t mg_type = CS_MULTIGRID_V_CYCLE;
    const char *s_mg_type = getenv("CS_BENCH_MG_TYPE");
    if (s_mg_type != nullptr) {
      if (strcmp(s_mg_type, "k_cycle") == 0)
        mg_type = CS_MULTIGRID_K_CYCLE;
      else if (strcmp(s_mg_type, "v_cycle") != 0)
        bft_error(__FILE__, __LINE__, 0,
                  "%s: Invalid value for CS_BENCH_MG_TYPE: %s\n"
                  " (expected \"v_cycle\" or \"k_cycle\").",
                  __func__, s_mg_type);
    }

    cs_sles_pc_t *pc = cs_multigrid_pc_create(mg_type);
    cs_multigrid_t *mg = (cs_multigrid_t *)cs_sles_pc_get_context(pc);
    cs_sles_it_transfer_pc(c, &pc);

    cs_sles_t *sles_p = cs_sles_find_or_add(CS_F_(p)->id, nullptr);
    if (verbosity > -1)
      cs_sles_set_verbosity(sles_p, verbosity);

    assert(strcmp(cs_sles_pc_get_type(cs_sles_it_get_pc(c)), "multigrid") == 0);

    const char *s_mgt[] = {
      getenv("CS_BENCH_MG_DESCENT_SMOOTHER_TYPE"),
      getenv("CS_BENCH_MG_ASCENT_SMOOTHER_TYPE"),
      getenv("CS_BENCH_MG_COARSE_SOLVER_TYPE")
    };

    const char *s_mg_nit[] = {
      getenv("CS_BENCH_MG_DESCENT_SMOOTHER_N_ITER"),
      getenv("CS_BENCH_MG_ASCENT_SMOOTHER_N_ITER"),
      getenv("CS_BENCH_MG_COARSE_SOLVER_N_ITER")
    };

    const char *s_mg_cspm = getenv("CS_BENCH_MG_COARSE_SOLVER_TEST_CVG");

    cs_sles_it_type_t smoother_type[3] = {
      CS_SLES_P_SYM_GAUSS_SEIDEL,
      CS_SLES_P_SYM_GAUSS_SEIDEL,
      CS_SLES_PCG
    };

    for (int i = 0; i < 3; i++) {
      if (s_mgt[i] != nullptr) {
        if (strcmp(s_mgt[i], "pcg") == 0)
          smoother_type[i] = CS_SLES_PCG;
        else if (strcmp(s_mgt[i], "fcg") == 0)
          smoother_type[i] = CS_SLES_FCG;
        else if (strcmp(s_mgt[i], "jacobi") == 0)
          smoother_type[i] = CS_SLES_JACOBI;
        else if (strcmp(s_mgt[i], "l1_jacobi") == 0)
          smoother_type[i] = CS_SLES_L1_JACOBI;
        else if (strcmp(s_mgt[i], "relaxed_jacobi") == 0)
          smoother_type[i] = CS_SLES_R_JACOBI;
        else if (strcmp(s_mgt[i], "sr_jacobi") == 0)
          smoother_type[i] = CS_SLES_SR_JACOBI;
        else if (strcmp(s_mgt[i], "gs") == 0)
          smoother_type[i] = CS_SLES_P_GAUSS_SEIDEL;
        else if (strcmp(s_mgt[i], "sgs") == 0)
          smoother_type[i] = CS_SLES_P_SYM_GAUSS_SEIDEL;
        else if (strcmp(s_mgt[i], "tfgs") == 0)
          smoother_type[i] = CS_SLES_TS_F_GAUSS_SEIDEL;
        else if (strcmp(s_mgt[i], "tbgs") == 0)
          smoother_type[i] = CS_SLES_TS_B_GAUSS_SEIDEL;
      }
    }

    int n_max_iter[3] = {1, 1, 1};
    double precision_mult_coarse = 1.;

    for (int i = 0; i < 3; i++) {
      int nit_d = 1, nit_a = 1, nit_c = 1;
      if (smoother_type[i] == CS_SLES_PCG) {
        nit_d = 2, nit_a = 4, nit_c = 500;
        precision_mult_coarse = 1.;
      }
      else if (smoother_type[i] == CS_SLES_FCG) {
        nit_d = 2, nit_a = 4, nit_c = 500;
        precision_mult_coarse = 1.;
      }
      else if (smoother_type[i] == CS_SLES_JACOBI) {
        nit_d = 2, nit_a = 4, nit_c = 4;
      }
      else if (smoother_type[i] == CS_SLES_P_GAUSS_SEIDEL) {
        nit_d = 2, nit_a = 4, nit_c = 500;
      }

      if (i == 0)
        n_max_iter[i] = nit_d;
      else if (i == 1)
        n_max_iter[i] = nit_a;
      else
        n_max_iter[i] = nit_c;

      if (s_mg_nit[i] != nullptr)
        n_max_iter[i] = atoi(s_mg_nit[i]);
    }

    if (   smoother_type[2] == CS_SLES_JACOBI
        || smoother_type[2] == CS_SLES_P_GAUSS_SEIDEL
        || smoother_type[2] == CS_SLES_P_SYM_GAUSS_SEIDEL)
      precision_mult_coarse = -1.;

    if (s_mg_cspm != nullptr)
      precision_mult_coarse = atof(s_mg_cspm);

    cs_multigrid_set_solver_options
      (mg,
       smoother_type[0],  // descent_smoother_type
       smoother_type[1],  // ascent_smoother_type
       smoother_type[2],  // coarse_solver_type,
       1,                 // (1 cycle for preconditioning)
       n_max_iter[0],     // n_max_iter_descent
       n_max_iter[1],     // n_max_iter_ascent
       n_max_iter[2],     // n_max_iter_coarse
       0,                 // polynomial precond. degree descent
       0,                 // polynomial precond. degree ascent
       0,                 // polynomial precond. degree coarse
       -1.0,              // precision multiplier descent (< 0 for max iters)
       -1.0,              // precision multiplier ascent (< 0 for max iters)
       precision_mult_coarse);

    const char *s_coarsen_type[] = {
      getenv("CS_BENCH_MG_COARSENING_TYPE"),
      getenv("CS_BENCH_MG_COARSENING_TYPE_FG")
    };

    const char *s_coarsen_fg_level = getenv("CS_BENCH_MG_COARSENING_FINE_LV");

    cs_grid_coarsening_t  coarsen_type[] = {
      CS_GRID_COARSENING_DEFAULT,
      CS_GRID_COARSENING_DEFAULT
    };
    int aggregation_limit[] = {3, -1};
    if (mg_type == CS_MULTIGRID_K_CYCLE) {
      aggregation_limit[0] = 4;
    }

    for (int i = 0; i < 2; i++) {
      if (s_coarsen_type[i] != nullptr) {
        if (strstr(s_coarsen_type[i], "dx") != nullptr) {
          coarsen_type[i] = CS_GRID_COARSENING_SPD_DX;
        }
        else if (strstr(s_coarsen_type[i], "mx") != nullptr) {
          coarsen_type[i] = CS_GRID_COARSENING_SPD_DX;
        }
        else if (strstr(s_coarsen_type[i], "pw") != nullptr) {
          coarsen_type[i] = CS_GRID_COARSENING_SPD_PW;
          aggregation_limit[i] = 4;
        }
        const char *s_agl = strstr(s_coarsen_type[i], "_");
        if (s_agl != nullptr) {
          int l = atoi(s_agl+1);
          if (l > 1)
            aggregation_limit[i] = l;
        }
      }
    }

    double p0p1_relaxation = 0.95;
    if (mg_type == CS_MULTIGRID_K_CYCLE)
      p0p1_relaxation = -1;

    int n_max_levels = 25;
    const char *s_max_lv = getenv("CS_BENCH_MG_MAX_LEVELS");
    if (s_max_lv != nullptr)
      n_max_levels = atoi(s_max_lv);

    cs_multigrid_set_coarsening_options(mg,
                                        aggregation_limit[0],
                                        coarsen_type[0],
                                        n_max_levels,
                                        30,   // min_g_cells (default 30) */
                                        p0p1_relaxation,
                                        0);   // postprocessing (default 0)

    if (s_coarsen_fg_level != nullptr) {
      int fg_level = atoi(s_coarsen_fg_level);
      if (fg_level > 0) {
        if (coarsen_type[1] == CS_GRID_COARSENING_DEFAULT)
          (coarsen_type[1] = coarsen_type[0]);
        if (aggregation_limit[1] < 1)
          aggregation_limit[1] = aggregation_limit[0];

        cs_multigrid_set_coarsening_options_fine_grid(mg,
                                                      fg_level,
                                                      aggregation_limit[1],
                                                      coarsen_type[1]);
      }
    }

    /* Try increasing merge_stride; tested at 1 (default), 2, 4 mostly */

    const char *s_merge = getenv("CS_BENCH_MG_MERGE_STEP");
    if (s_merge != nullptr) {
      int merge_step = atoi(s_merge);
      if (merge_step > 0) {
        int merge_threshold = 600;
        int merge_threshold_g = 600;
        const char *s_merge_threshold_l = getenv("CS_BENCH_MG_MERGE_THRESHOLD");
        const char *s_merge_threshold_g
          = getenv("CS_BENCH_MG_MERGE_THRESHOLD_GLOBAL");
        if (s_merge_threshold_l != nullptr)
          merge_threshold = atoi(s_merge_threshold_l);
        if (s_merge_threshold_g != nullptr)
          merge_threshold_g = atoi(s_merge_threshold_g);

        cs_multigrid_set_merge_options(mg,
                                       merge_step,
                                       merge_threshold,
                                       merge_threshold_g);
      }
    }

    s_merge = getenv("CS_BENCH_MG_MERGE_BOTTOM_MAX_RANKS");
    if (s_merge != nullptr) {
      int n_max_ranks = atoi(s_merge);
      float max_row_factor = 1.0;
      const char *s_row_factor = getenv("CS_BENCH_MG_MERGE_BOTTOM_MAX_ROW_FACTOR");
      if (s_row_factor != nullptr)
        max_row_factor = atof(s_row_factor);
      cs_multigrid_set_merge_bottom_options(mg,
                                            n_max_ranks,
                                            max_row_factor);
    }

    /* Additional settings when running on GPU
       --------------------------------------- */

#if defined(HAVE_ACCEL)

    const char *s_lv_device = getenv("CS_BENCH_MG_MAX_LEVEL_DEVICE");
    if (s_lv_device != nullptr)
      cs_multigrid_set_max_grid_level_for_device(atoi(s_lv_device));

    /* Adjust defaults */

    for (int i = 0; i < 2; i++) {
      switch(smoother_type[i]) {
      case CS_SLES_P_GAUSS_SEIDEL:
        smoother_type[i] = CS_SLES_JACOBI;
        n_max_iter[i] *= 2;
        break;
      case CS_SLES_P_SYM_GAUSS_SEIDEL:
        smoother_type[i] = CS_SLES_JACOBI;
        if (n_max_iter[i] == 1) {
          if (i == 0)
            n_max_iter[i] = 2;
          else
            n_max_iter[i] = 3;
        }
        else
          n_max_iter[i] *= 3;
        break;
      case CS_SLES_TS_B_GAUSS_SEIDEL:
        [[fallthrough]];
      case CS_SLES_TS_F_GAUSS_SEIDEL:
        smoother_type[i] = CS_SLES_JACOBI;
        n_max_iter[i] = 2;
        break;
      default:
        smoother_type[i] = CS_SLES_FCG;
      }
    }

    /* User-defined settings */

    s_mgt[0] = getenv("CS_BENCH_MG_DESCENT_SMOOTHER_TYPE_D");
    s_mgt[1] = getenv("CS_BENCH_MG_ASCENT_SMOOTHER_TYPE_D");

    s_mg_nit[0] = getenv("CS_BENCH_MG_DESCENT_SMOOTHER_N_ITER_D");
    s_mg_nit[1] = getenv("CS_BENCH_MG_ASCENT_SMOOTHER_N_ITER_D");

    for (int i = 0; i < 2; i++) {
      int nit_d = n_max_iter[0], nit_a = n_max_iter[1];
      if (s_mgt[i] != nullptr) {
        if (strcmp(s_mgt[i], "fcg") == 0) {
          smoother_type[i] = CS_SLES_FCG;
          nit_d = 2, nit_a = 4;
        }
        else if (strcmp(s_mgt[i], "jacobi") == 0) {
          smoother_type[i] = CS_SLES_JACOBI;
          nit_d = 2, nit_a = 4;
        }
        else
          bft_error(__FILE__, __LINE__, 0,
                    "%s: Invalid value for CS_BENCH_MG_*_SMOOTHER_TYPE_D: %s\n"
                    " (expected \"fcg\" or \"jacobi\".)",
                    __func__, s_mgt[0]);
      }
      if (i == 0)
        n_max_iter[i] = nit_d;
      else if (i == 1)
        n_max_iter[i] = nit_a;
      if (s_mg_nit[i] != nullptr)
        n_max_iter[i] = atoi(s_mg_nit[i]);
    }

    cs_multigrid_set_solver_options_d
      (mg,
       smoother_type[0],
       smoother_type[1],
       smoother_type[2],
       n_max_iter[0],    // n max iter for descent (default 1)
       n_max_iter[1],    // n max iter for ascent (default 1)
       n_max_iter[2],    // n max iter coarse solver (default 1)
       0,                // polynomial precond. degree descent (default)
       0,                // polynomial precond. degree ascent (default)
       0);               // polynomial precond. degree coarse (default 0)

#endif // defined(HAVE_ACCEL)

    return;

  }

  /* FCG with polynomial preconditioner
     ---------------------------------- */

  if (strstr(s_sles_type, "native_fcg_poly") != nullptr) {

    cs_sles_it_define(CS_F_(p)->id,
                      nullptr,
                      CS_SLES_FCG,
                      1,
                      10000);

    return;

  }

  /* FCG with jacobi preconditioner
     ------------------------------ */

  if (strstr(s_sles_type, "native_fcg_jacobi") != nullptr) {

    cs_sles_it_define(CS_F_(p)->id,
                      nullptr,
                      CS_SLES_FCG,
                      0,
                      10000);

    return;

  }

  /* Setting pressure solver with hypre with Default PCG+BoomerAMG options */
  /*-----------------------------------------------------------------------*/

#if defined(HAVE_HYPRE)

  if (strstr(s_sles_type, "hypre") != nullptr) {

    cs_sles_hypre_define(CS_F_(p)->id,
                         NULL,
                         CS_SLES_HYPRE_PCG,        /* solver type */
                         CS_SLES_HYPRE_BOOMERAMG,  /* preconditioner type */
                         NULL,
                         NULL);

    return;

  }

#endif /* defined(HAVE_HYPRE) */

#if defined(HAVE_AMGX)

  if (strstr(s_sles_type, "amgx") != nullptr) {

    cs_sles_amgx_t *c = cs_sles_amgx_define(CS_F_(p)->id, nullptr);

    cs_sles_amgx_set_config_file(c, "PCG_CLASSICAL_V_JACOBI.json");
    // cs_sles_amgx_set_config_file(c, "amgx.json");

    return;

  }

#endif /* defined(HAVE_AMGX) */

#if defined(HAVE_CUDSS)

  if (strstr(s_sles_type, "cudss") != nullptr) {

    cs_sles_cudss_t *c = cs_sles_cudss_define(CS_F_(p)->id, nullptr);

    return;
  }

#endif /* defined(HAVE_CUDSS) */

  bft_error(__FILE__, __LINE__, 0,
            "%s: Invalid value for CS_BENCH_SLES_TYPE:\n"
            "   %s requested but not available in this build.",
            __func__, s_sles_type);

}

/*----------------------------------------------------------------------------*/

END_C_DECLS
