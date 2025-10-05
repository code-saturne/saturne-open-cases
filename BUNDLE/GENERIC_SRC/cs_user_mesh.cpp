/*============================================================================
 * Definition of the calculation mesh.
 *
 * Mesh-related user functions (called in this order):
 *   1) Manage the exchange of data between code_saturne and the pre-processor
 *   2) Define (conforming or non-conforming) mesh joinings.
 *   3) Define (conforming or non-conforming) periodicity.
 *   4) Define thin walls.
 *   5) Modify the geometry and mesh.
 *   6) Smoothe the mesh.
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

#include "cs_headers.h"

/*----------------------------------------------------------------------------
 * Standard library headers
 *----------------------------------------------------------------------------*/

#include <assert.h>
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*----------------------------------------------------------------------------
 * Local headers
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/

BEGIN_C_DECLS

static int _n_tubes = 1;

/*============================================================================
 * User function definitions
 *============================================================================*/

/*----------------------------------------------------------------------------*/
/*
 * \brief Define mesh files to read and optional associated transformations.
 */
/*----------------------------------------------------------------------------*/

void
cs_user_mesh_input(void)
{
  const char *path = "mesh_input.csm";

  /* Settings based on case name */
  {
    char *run_id = nullptr, *case_name = nullptr;
    cs_base_get_run_identity(&run_id, &case_name, nullptr);

    /* Number of x and y copies is based on last portion of case
       name, after second "_". */

    char *s = strstr(case_name, "_");
    if (s != nullptr)
      s = strstr(s+1, "_");
    if (s != nullptr) {
      if (strlen(s) > 1) {
        int n_tubes = atoi(s+1);
        if (n_tubes > 1)
          _n_tubes = n_tubes;
      }
    }

    CS_FREE(run_id);
    CS_FREE(case_name);
  }

  const int n = _n_tubes, m = _n_tubes;
  char _renames[64][8];
  char *renames[8];

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {

      int n_renames = 0;

      if (i > 0) {
        strcpy(_renames[n_renames*2], "west");
        strcpy(_renames[n_renames*2+1], "join");
        n_renames++;
      }
      if (i < n-1) {
        strcpy(_renames[n_renames*2], "east");
        strcpy(_renames[n_renames*2+1], "join");
        n_renames++;
      }
      if (j > 0) {
        strcpy(_renames[n_renames*2], "south");
        strcpy(_renames[n_renames*2+1], "join");
        n_renames++;
      }
      if (j < m-1) {
        strcpy(_renames[n_renames*2], "north");
        strcpy(_renames[n_renames*2+1], "join");
        n_renames++;
      }

      for (int k = 0; k < n_renames*2; k++)
        renames[k] = _renames[k];

      const double transf_matrix[3][4] = {{1., 0., 0., -0.0225*(n-1) + 0.0450*i},
                                          {0., 1., 0., -0.0225*(m-1) + 0.0450*j},
                                          {0., 0., 1., 0.}};

      cs_preprocessor_data_add_file(path,
                                    n_renames,
                                    const_cast<const char **>(renames),
                                    transf_matrix);
    }
  }
}

/*----------------------------------------------------------------------------*/
/*
 * \brief Define mesh joinings.
 */
/*----------------------------------------------------------------------------*/

void
cs_user_join(void)
{
  /* Add a joining operation */
  /* ----------------------- */

  int    verbosity = 1;     /* per-task dump if > 1, debug level if >= 3 */
  int    visualization = 0; /* debug level if >= 3 */
  float  fraction = 0.10, plane = 25.;

  cs_join_add("join", fraction, plane, verbosity, visualization);
}

/*----------------------------------------------------------------------------*/
/*
 * \brief Define periodic faces.
 */
/*----------------------------------------------------------------------------*/

void
cs_user_periodicity(void)
{
  const char *s_perio = getenv("CS_BENCH_NO_PERIODICITY");
  if (s_perio != nullptr) {
    if (atoi(s_perio) > 0)
      return;
  }

  int    verbosity = 1;     /* per-task dump if > 1, debug level if >= 3 */
  int    visualization = 0; /* debug level if >= 3 */
  float  fraction = 0.10, plane = 25.;

  const double translation[3] = {0.0,
                                 0.0450*_n_tubes,
                                 0.0}; /* Translation vector */

  cs_join_perio_add_translation("south or north",
                                fraction,
                                plane,
                                verbosity,
                                visualization,
                                translation);
}

/*----------------------------------------------------------------------------*/

END_C_DECLS
