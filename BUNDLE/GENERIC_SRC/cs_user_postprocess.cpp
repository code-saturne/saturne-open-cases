/*============================================================================
 * Define postprocessing output.
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

#include "stdlib.h"
#include "string.h"

/*----------------------------------------------------------------------------
 * Local headers
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/

BEGIN_C_DECLS

/*============================================================================
 * Local (user defined) function definitions
 *============================================================================*/

static int  snap_id = 0.;
static const int  n_snaps = 1;
static const double  t_snap[] = {750.0};

/*============================================================================
 * User function definitions
 *============================================================================*/

/*----------------------------------------------------------------------------*/
/*
 * \brief Define post-processing writers.
 *
 * The default output format and frequency may be configured, and additional
 * post-processing writers allowing outputs in different formats or with
 * different format options and output frequency than the main writer may
 * be defined.
 */
/*----------------------------------------------------------------------------*/

void
cs_user_postprocess_writers(void)
{
  /* Use Catalyst writer when available */
#if defined (HAVE_CATALYST)
  if (fvm_writer_format_available(fvm_writer_get_format_id("catalyst"))) {
    cs_post_define_writer(1,                            /* writer_id */
                          "catalyst",                   /* writer name */
                          "postprocessing",             /* directory name */
                          "catalyst",                   /* format_name */
                          "",                           /* format_options */
                          FVM_WRITER_TRANSIENT_CONNECT,
                          false,                        /* output_at_start */
                          true,                         /* output_at_end */
                          -1,                           /* frequency_n */
                          -1.0);                        /* frequency_t */
  }
#endif
}

/*----------------------------------------------------------------------------*/
/*
 * \brief Define post-processing meshes.
 *
 * The main post-processing meshes may be configured, and additional
 * post-processing meshes may be defined as a subset of the main mesh's
 * cells or faces (both interior and boundary).
 */
/*----------------------------------------------------------------------------*/

void
cs_user_postprocess_meshes(void)
{
#if defined (HAVE_CATALYST)
  /* Add Catalyst output to volume mesh when available */
  cs_post_mesh_attach_writer(CS_POST_MESH_VOLUME, 1);
#endif
}

/*----------------------------------------------------------------------------*/

END_C_DECLS
