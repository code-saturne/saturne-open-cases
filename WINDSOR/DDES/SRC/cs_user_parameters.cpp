/*============================================================================
 * User-defined parameters for AutoCFD5 Case 1 — SST-DDES case.
 *
 * Activate the Delayed Detached Eddy Simulation (DDES) hybrid model
 * on top of the k-omega SST turbulence model defined in setup.xml.
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

BEGIN_C_DECLS

/*----------------------------------------------------------------------------
 * User function for general parameters.
 * Called before the setup stage.
 *----------------------------------------------------------------------------*/

void
cs_user_model(void)
{
  /* Activate SST-DDES hybrid RANS/LES model.
   *
   * The base turbulence model (k-omega-SST) is set in setup.xml.
   * Here we enable the DDES shielding function which:
   *   - Keeps RANS behaviour in attached boundary layers
   *   - Switches to LES-like resolved mode in separated/wake regions
   *
   * CS_HYBRID_NONE  = 0  (pure RANS)
   * CS_HYBRID_DES   = 1  (DES, no shielding)
   * CS_HYBRID_DDES  = 2  (Delayed DES with shielding function)
   * CS_HYBRID_SAS   = 3  (Scale-Adaptive Simulation)
   * CS_HYBRID_HTLES = 4  (Hybrid Temporal LES)
   */

  cs_turb_model_t *turb_model = cs_get_glob_turb_model();
  turb_model->hybrid_turb = CS_HYBRID_DDES;
}

END_C_DECLS
