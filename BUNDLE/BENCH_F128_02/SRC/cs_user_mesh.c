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

  Copyright (C) 1998-2022 EDF S.A.

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
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*----------------------------------------------------------------------------
 * Local headers
 *----------------------------------------------------------------------------*/

#include "cs_headers.h"

/*----------------------------------------------------------------------------*/

BEGIN_C_DECLS

static int _n_tubes = 2;

/*============================================================================
 * User function definitions
 *============================================================================*/

/*----------------------------------------------------------------------------*/
/*!
 * \brief Define mesh files to read and optional associated transformations.
 */
/*----------------------------------------------------------------------------*/

void
cs_user_mesh_input(void)
{
  const char *path = "mesh_input.csm";

  int i, j, k;
  const int n = _n_tubes, m = _n_tubes;
  char _renames[64][8];
  char *renames[8];

  for (i = 0; i < n; i++) {
    for (j = 0; j < m; j++) {

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

      for (k = 0; k < n_renames*2; k++)
        renames[k] = _renames[k];

      const double transf_matrix[3][4] = {{1., 0., 0., -0.0225*(n-1) + 0.0450*i},
                                          {0., 1., 0., -0.0225*(m-1) + 0.0450*j},
                                          {0., 0., 1., 0.}};

      cs_preprocessor_data_add_file(path,
                                    n_renames, renames,
                                    transf_matrix);
    }
  }
}

/*----------------------------------------------------------------------------*/
/*!
 * \brief Define mesh joinings.
 */
/*----------------------------------------------------------------------------*/

void
cs_user_join(void)
{
  int    join_num;

  /* Add a joining operation */
  /* ----------------------- */

  int    verbosity = 1;     /* per-task dump if > 1, debug level if >= 3 */
  int    visualization = 1; /* debug level if >= 3 */
  float  fraction = 0.10, plane = 25.;

  join_num = cs_join_add("join",
                         fraction,
                         plane,
                         verbosity,
                         visualization);
}

/*----------------------------------------------------------------------------*/
/*!
 * \brief Define periodic faces.
 */
/*----------------------------------------------------------------------------*/

void
cs_user_periodicity(void)
{
  int    join_num;

  int    verbosity = 1;     /* per-task dump if > 1, debug level if >= 3 */
  int    visualization = 1; /* debug level if >= 3 */
  float  fraction = 0.10, plane = 25.;

  const double translation[3] = {0.0,
                                 0.0450*_n_tubes,
                                 0.0}; /* Translation vector */

  join_num = cs_join_perio_add_translation("south or north",
                                           fraction,
                                           plane,
                                           verbosity,
                                           visualization,
                                           translation);
}

/*----------------------------------------------------------------------------*/
/*!
 * \brief Set options for cutting of warped faces.
 */
/*----------------------------------------------------------------------------*/

void
cs_user_mesh_warping(void)
{

}

/*----------------------------------------------------------------------------*/
/*!
 * \brief Insert boundaries into a mesh.
 *
 * \param[in,out] mesh  pointer to a cs_mesh_t structure
 */
/*----------------------------------------------------------------------------*/

void
cs_user_mesh_boundary(cs_mesh_t  *mesh)
{

}

/*----------------------------------------------------------------------------*/
/*!
 * \brief Modify geometry and mesh.
 *
 * \param[in,out] mesh  pointer to a cs_mesh_t structure
*/
/*----------------------------------------------------------------------------*/

void
cs_user_mesh_modify(cs_mesh_t  *mesh)
{

}

/*----------------------------------------------------------------------------*/
/*!
 * \brief Mesh smoothing.
 *
 * \param[in,out] mesh  pointer to a cs_mesh_t structure
*/
/*----------------------------------------------------------------------------*/

void
cs_user_mesh_smoothe(cs_mesh_t  *mesh)
{
}

/*----------------------------------------------------------------------------*/
/*!
 * \brief Enable or disable mesh saving.
 *
 * By default, mesh is saved when modified.
 *
 * \param[in,out] mesh  pointer to a cs_mesh_t structure
*/
/*----------------------------------------------------------------------------*/

void
cs_user_mesh_save(cs_mesh_t  *mesh)
{

}

/*----------------------------------------------------------------------------*/
/*!
 * \brief Tag bad cells within the mesh based on user-defined geometric criteria.
 *
 * \param[in,out] mesh  pointer to a cs_mesh_t structure
 * \param[in,out] mesh_quantities pointer to a cs_mesh_quantities_t structure
*/
/*----------------------------------------------------------------------------*/

void
cs_user_mesh_bad_cells_tag(cs_mesh_t             *mesh,
                           cs_mesh_quantities_t  *mesh_quantities)
{
}

/*----------------------------------------------------------------------------*/

END_C_DECLS
