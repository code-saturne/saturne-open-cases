/*============================================================================
 * User-defined extra operations for AutoCFD5 Case 1:
 * Compute and log aerodynamic force and moment coefficients
 * (Cd, Cy, Cl, CMx, CMy, CMz) on the Windsor Squareback body.
 *
 * The vehicle is yawed by -2.5 deg around z in the tunnel frame.
 * Code_saturne computes forces in the tunnel (grid) frame; they must
 * be rotated into the vehicle frame before non-dimensionalisation,
 * as required by the AutoCFD5 specification.
 *
 * Normalisation uses the LOCAL velocity magnitude and static pressure
 * interpolated at the reference probe [-2.0, 0.0, 1.3] m, as required
 * by the specification (not the inlet boundary condition value).
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

#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>

BEGIN_C_DECLS

/*----------------------------------------------------------------------------
 * Reference quantities from the AutoCFD5 Case 1 specification:
 *   - Frontal area Sref = 0.112 m^2
 *   - Wheelbase (moment ref length) Lref = 0.6375 m
 *   - Moment centre: origin (0, 0, 0) = mid-wheelbase, mid-track, ground
 *   - Yaw angle psi = -2.5 deg around z-axis
 *   - Coefficients expressed in the yawed vehicle reference frame
 *   - q = 0.5 * rho * |V_probe|^2   (velocity at [-2, 0, 1.3] m)
 *----------------------------------------------------------------------------*/

void
cs_user_extra_operations(cs_domain_t  *domain)
{
  /* Reference quantities */
  const cs_real_t s_ref = 0.112;       /* frontal area [m^2]               */
  const cs_real_t l_ref = 0.6375;      /* wheelbase for moments [m]        */
  const cs_real_t rho   = 1.225;       /* density [kg/m^3]                 */

  /* Yaw angle: vehicle rotated by -2.5 deg around z in tunnel frame.
   * Rotation matrix from tunnel frame to vehicle frame:
   *   [ cos(psi)   sin(psi)  0 ]       psi = -2.5 deg
   *   [-sin(psi)   cos(psi)  0 ]
   *   [    0           0     1 ]        */
  const cs_real_t yaw_deg = -2.5;
  const cs_real_t yaw_rad = yaw_deg * acos(-1.0) / 180.0;
  const cs_real_t cos_psi = cos(yaw_rad);
  const cs_real_t sin_psi = sin(yaw_rad);

  /* Moment centre at origin */
  const cs_real_3_t x_moment = {0., 0., 0.};

  /* Reference probe location (per AutoCFD5 specification, p.7-8) */
  const cs_real_t ref_point[3] = {-2.0, 0.0, 1.3};

  const cs_real_t *b_face_surf
    = domain->mesh_quantities->b_face_surf;
  const cs_real_3_t *b_face_cog
    = (const cs_real_3_t *)domain->mesh_quantities->b_face_cog;

  /* -------------------------------------------------------------------
   * Interpolate velocity and pressure at the reference probe.
   * Use the nearest cell value (parallel-safe).
   * Cell ID is cached after the first call (static).
   * ------------------------------------------------------------------- */

  static cs_lnum_t s_ref_cell = -2; /* -2 = not searched yet */
  static int s_is_owner = 0;        /* 1 if this rank owns the probe cell */

  const cs_real_3_t *cell_cen
    = (const cs_real_3_t *)domain->mesh_quantities->cell_cen;

  if (s_ref_cell == -2) {
    cs_real_t local_min_d2 = HUGE_VAL;
    cs_lnum_t local_best = -1;

    for (cs_lnum_t c_id = 0; c_id < domain->mesh->n_cells; c_id++) {
      cs_real_t d2 = 0.;
      for (int j = 0; j < 3; j++) {
        cs_real_t dj = cell_cen[c_id][j] - ref_point[j];
        d2 += dj * dj;
      }
      if (d2 < local_min_d2) {
        local_min_d2 = d2;
        local_best = c_id;
      }
    }

    /* Find global minimum distance across all MPI ranks */
    cs_real_t global_min_d2 = local_min_d2;
    cs_parall_min(1, CS_REAL_TYPE, &global_min_d2);

    /* Unique-owner election: smallest MPI rank among those at the global
     * minimum. Prevents cs_parall_sum from double-counting when 2+ ranks
     * are equidistant (typical when the probe lies on the symmetry plane). */
    int candidate_rank = INT_MAX;
    if (local_min_d2 <= global_min_d2 * (1. + 1e-12))
      candidate_rank = cs_glob_rank_id;

    cs_parall_min(1, CS_INT_TYPE, &candidate_rank);

    if (cs_glob_rank_id == candidate_rank) {
      s_ref_cell = local_best;
      s_is_owner = 1;
    }
    else {
      s_ref_cell = -1; /* not on this rank */
      s_is_owner = 0;
    }
  }

  /* Read velocity and pressure at the reference cell */
  cs_field_t *vel_field = cs_field_by_name("velocity");
  cs_field_t *p_field   = cs_field_by_name("pressure");

  cs_real_3_t v_probe = {0., 0., 0.};
  cs_real_t   p_probe = 0.;

  if (s_is_owner && s_ref_cell >= 0) {
    const cs_real_3_t *vel_val = (const cs_real_3_t *)vel_field->val;
    v_probe[0] = vel_val[s_ref_cell][0];
    v_probe[1] = vel_val[s_ref_cell][1];
    v_probe[2] = vel_val[s_ref_cell][2];
    p_probe = p_field->val[s_ref_cell];
  }

  /* Broadcast: only the owning rank has non-zero values */
  cs_parall_sum(3, CS_REAL_TYPE, v_probe);
  cs_parall_sum(1, CS_REAL_TYPE, &p_probe);

  /* Reference velocity magnitude and dynamic pressure */
  cs_real_t v_ref = sqrt(  v_probe[0] * v_probe[0]
                         + v_probe[1] * v_probe[1]
                         + v_probe[2] * v_probe[2]);

  /* Safety: at the very first iterations the flow may not be established */
  if (v_ref < 1.0)
    v_ref = 40.0;

  cs_real_t q_ref = 0.5 * rho * v_ref * v_ref;

  /* -------------------------------------------------------------------
   * Names of the three wall zones forming the Windsor vehicle
   * (must match CGNS boundary zone names)
   * ------------------------------------------------------------------- */

  const char *zone_names[] = {
    "Windsor_Body",
    "Windsor_Base",
    "Windsor_Pins"
  };
  const int n_zones = 3;

  /* Accumulate total forces and moments in TUNNEL frame */
  cs_real_3_t f_tun  = {0., 0., 0.};
  cs_real_3_t m_tun  = {0., 0., 0.};

  cs_field_t *b_stress = cs_field_by_name_try("boundary_stress");

  if (b_stress != nullptr) {
    const cs_real_3_t *stress_val = (const cs_real_3_t *)b_stress->val;

    for (int iz = 0; iz < n_zones; iz++) {
      const cs_zone_t *zn = cs_boundary_zone_by_name_try(zone_names[iz]);
      if (zn == nullptr)
        continue;

      for (cs_lnum_t e_id = 0; e_id < zn->n_elts; e_id++) {
        cs_lnum_t face_id = zn->elt_ids[e_id];
        cs_real_t surf = b_face_surf[face_id];

        /* Force on this face (tunnel frame) */
        cs_real_3_t f_face;
        for (cs_lnum_t i = 0; i < 3; i++) {
          f_face[i] = stress_val[face_id][i] * surf;
          f_tun[i] += f_face[i];
        }

        /* Lever arm from moment centre to face centre */
        cs_real_3_t r;
        for (cs_lnum_t i = 0; i < 3; i++)
          r[i] = b_face_cog[face_id][i] - x_moment[i];

        /* Moment M = r x F (tunnel frame) */
        m_tun[0] += r[1] * f_face[2] - r[2] * f_face[1];
        m_tun[1] += r[2] * f_face[0] - r[0] * f_face[2];
        m_tun[2] += r[0] * f_face[1] - r[1] * f_face[0];
      }
    }
  }

  /* Parallel sum across all MPI ranks */
  cs_parall_sum(3, CS_REAL_TYPE, f_tun);
  cs_parall_sum(3, CS_REAL_TYPE, m_tun);

  /* -------------------------------------------------------------------
   * Rotate from tunnel frame to vehicle frame
   *   F_veh_x =  cos(psi) * F_tun_x + sin(psi) * F_tun_y
   *   F_veh_y = -sin(psi) * F_tun_x + cos(psi) * F_tun_y
   *   F_veh_z =  F_tun_z
   * Same rotation for moments.
   * ------------------------------------------------------------------- */

  cs_real_3_t f_veh, m_veh;

  f_veh[0] =  cos_psi * f_tun[0] + sin_psi * f_tun[1];
  f_veh[1] = -sin_psi * f_tun[0] + cos_psi * f_tun[1];
  f_veh[2] =  f_tun[2];

  m_veh[0] =  cos_psi * m_tun[0] + sin_psi * m_tun[1];
  m_veh[1] = -sin_psi * m_tun[0] + cos_psi * m_tun[1];
  m_veh[2] =  m_tun[2];

  /* Force coefficients (vehicle frame) */
  cs_real_t cd  = f_veh[0] / (q_ref * s_ref);
  cs_real_t cy  = f_veh[1] / (q_ref * s_ref);
  cs_real_t cl  = f_veh[2] / (q_ref * s_ref);

  /* Moment coefficients (vehicle frame) */
  cs_real_t cmx = m_veh[0] / (q_ref * s_ref * l_ref);
  cs_real_t cmy = m_veh[1] / (q_ref * s_ref * l_ref);
  cs_real_t cmz = m_veh[2] / (q_ref * s_ref * l_ref);

  /* Log to listing */
  bft_printf("  ---- AutoCFD5 Case 1 - Aerodynamic Coefficients ----\n"
             "  (vehicle frame, yaw = %.1f deg)\n"
             "  V_ref = %8.4f m/s   P_ref = %12.5e Pa  (probe)\n"
             "  q_ref = %10.3f Pa   Sref = %6.4f m^2   Lref = %6.4f m\n"
             "  Tunnel:  Fx = %12.5e   Fy = %12.5e   Fz = %12.5e N\n"
             "  Vehicle: Fd = %12.5e   Fs = %12.5e   Fl = %12.5e N\n"
             "  Cd = %12.5e   Cy = %12.5e   Cl = %12.5e\n"
             "  Mx = %12.5e   My = %12.5e   Mz = %12.5e Nm\n"
             "  CMx= %12.5e   CMy= %12.5e   CMz= %12.5e\n"
             "  ---------------------------------------------------\n",
             yaw_deg,
             v_ref, p_probe,
             q_ref, s_ref, l_ref,
             f_tun[0], f_tun[1], f_tun[2],
             f_veh[0], f_veh[1], f_veh[2],
             cd, cy, cl,
             m_veh[0], m_veh[1], m_veh[2],
             cmx, cmy, cmz);

  /* -------------------------------------------------------------------
   * Write coefficients to a CSV file for convergence monitoring
   * ------------------------------------------------------------------- */

  if (cs_glob_rank_id <= 0) {

    const int nt_cur = domain->time_step->nt_cur;
    FILE *f = nullptr;

    if (nt_cur == 1)
      f = fopen("force_coefficients.csv", "w");
    else
      f = fopen("force_coefficients.csv", "a");

    if (f != nullptr) {
      if (nt_cur == 1)
        fprintf(f,
                "iteration,"
                "V_ref,P_ref,q_ref,"
                "Fx_tun,Fy_tun,Fz_tun,"
                "Fd_veh,Fs_veh,Fl_veh,"
                "Cd,Cy,Cl,"
                "Mx_veh,My_veh,Mz_veh,"
                "CMx,CMy,CMz\n");
      fprintf(f,
              "%d,"
              "%.6e,%.6e,%.6e,"
              "%.6e,%.6e,%.6e,"
              "%.6e,%.6e,%.6e,"
              "%.6e,%.6e,%.6e,"
              "%.6e,%.6e,%.6e,"
              "%.6e,%.6e,%.6e\n",
              nt_cur,
              v_ref, p_probe, q_ref,
              f_tun[0], f_tun[1], f_tun[2],
              f_veh[0], f_veh[1], f_veh[2],
              cd, cy, cl,
              m_veh[0], m_veh[1], m_veh[2],
              cmx, cmy, cmz);
      fclose(f);
    }
  }
}

END_C_DECLS
