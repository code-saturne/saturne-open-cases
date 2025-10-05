/*============================================================================
 * Definition of advanced options relative to parallelism.
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

/*----------------------------------------------------------------------------*/
/*!
 * \file cs_user_performance_tuning.cpp
 *
 * \brief Definition of advanced options relative to parallelism.
 *
 * See \ref cs_user_performance_tuning for examples.
 */
/*----------------------------------------------------------------------------*/

/*============================================================================
 * User function definitions
 *============================================================================*/

/*----------------------------------------------------------------------------*/
/*
 * \brief Define parallel IO settings.
 */
/*----------------------------------------------------------------------------*/

void
cs_user_parallel_io(void)
{
#if defined(HAVE_MPI_IO)

  /* Fine-tune parallel IO settings.

     Available distributed block access methods
     (subject to build with MPI IO) are:

     CS_FILE_STDIO_SERIAL        Serial standard C IO
                                 (funnelled through rank 0 in parallel)
     CS_FILE_STDIO_PARALLEL      Per-process standard C IO
     CS_FILE_MPI_INDEPENDENT     Non-collective MPI-IO
                                 with independent file open and close
     CS_FILE_MPI_NON_COLLECTIVE  Non-collective MPI-IO
                                 with collective file open and close
     CS_FILE_MPI_COLLECTIVE      Collective MPI-IO
  */

  MPI_Info hints = MPI_INFO_NULL;
  cs_file_access_t  method = CS_FILE_MPI_COLLECTIVE;

  /* Set MPI IO hints

     (see MPI-IO or your filesystem documentation;
     examples here may have no effect, improve, or degrade performance)

     For LUSTRE filesystems, many articles in the literature seem
     to recommend adjusting striping to improve performance.
     If using ROMIO, useful hints for collective buffering and data-sieving
     may take values: "enable", "disable", "automatic".
  */

  MPI_Info_create(&hints);

  MPI_Info_set(hints, "collective_buffering", "true");
  MPI_Info_set(hints, "access_style", "write_once");

  /* Set default file acces methods and communicator stride */

  cs_file_set_default_access(CS_FILE_MODE_WRITE, method, hints);

  MPI_Info_set(hints, "collective_buffering", "true");
  MPI_Info_set(hints, "access_style", "read_once");

  cs_file_set_default_access(CS_FILE_MODE_READ, method, hints);

  MPI_Info_free(&hints);

#endif /* defined(HAVE_MPI_IO) */
}

/*----------------------------------------------------------------------------*/
/*!
 * \brief Define advanced partitioning options.
 */
/*----------------------------------------------------------------------------*/

void
cs_user_partition(void)
{
  const char *s_partition_type = getenv("CS_BENCH_PARTITION_TYPE");
  if (s_partition_type != nullptr) {

    cs_partition_algorithm_t partition_type = CS_PARTITION_SFC_MORTON_BOX;
    int rank_step = 1;

    const char *s_partition_rank_step = getenv("CS_BENCH_PARTITION_RANK_STEP");
    if (s_partition_rank_step != nullptr)
      rank_step = atoi(s_partition_rank_step);

    if (strcmp(s_partition_type, "morton_box") == 0)
      partition_type = CS_PARTITION_SFC_MORTON_BOX;
    else if (strcmp(s_partition_type, "morton_cube") == 0)
      partition_type = CS_PARTITION_SFC_MORTON_CUBE;
    else if (strcmp(s_partition_type, "hilbert_box") == 0)
      partition_type = CS_PARTITION_SFC_HILBERT_BOX;
    else if (strcmp(s_partition_type, "hilbert_cube") == 0)
      partition_type = CS_PARTITION_SFC_HILBERT_CUBE;
    else if (strcmp(s_partition_type, "scotch") == 0)
      partition_type = CS_PARTITION_SCOTCH;
    else if (strcmp(s_partition_type, "metis") == 0)
      partition_type = CS_PARTITION_METIS;

    cs_partition_set_algorithm(CS_PARTITION_MAIN,
                               partition_type,
                               rank_step,
                               false);  /* ignore periodicity in graph */

  }
}

/*----------------------------------------------------------------------------*/

END_C_DECLS
