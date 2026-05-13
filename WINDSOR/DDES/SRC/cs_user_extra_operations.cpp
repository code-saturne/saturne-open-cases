/*============================================================================
 * User-defined extra operations for AutoCFD5 Case 1 — DDES case.
 * Compute and log aerodynamic force and moment coefficients
 * (Cd, Cy, Cl, CMx, CMy, CMz) on the Windsor Squareback body, both
 * instantaneous and time-averaged.
 *
 * The vehicle is yawed by -2.5 deg around z in the tunnel frame.
 * Code_saturne computes forces in the tunnel (grid) frame; they must
 * be rotated into the vehicle frame before non-dimensionalisation,
 * as required by the AutoCFD5 specification.
 *
 * Normalisation uses the LOCAL velocity magnitude and static pressure
 * interpolated at the reference probe [-2.0, 0.0, 1.3] m, as required
 * by the specification (not the inlet boundary condition value).
 *
 * Time averaging of forces/moments starts at t_avg_start, expressed in
 * convective times tau_c = L_body / U_inf (Windsor body length).
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

  /* Time-averaging start time (skip initial transient).
   * Expressed in convective times tau_c = L_body / U_inf based on the
   * Windsor body length. Increase n_tau_transient for production runs
   * (typical bluff-body DDES practice: 10-20 tau_c before averaging,
   * 20+ tau_c of averaging window).                                    */
  const cs_real_t l_body = 1.044;          /* Windsor body length [m]   */
  const cs_real_t u_inf  = 40.0;           /* free-stream velocity [m/s]*/
  const cs_real_t n_tau_transient = 10.0;  /* convective times skipped  */
  const cs_real_t t_avg_start = n_tau_transient * l_body / u_inf;

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

  const cs_real_t t_cur = domain->time_step->t_cur;
  const cs_real_t dt    = domain->time_step->dt[0];

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

  /* Instantaneous force coefficients (vehicle frame) */
  cs_real_t cd  = f_veh[0] / (q_ref * s_ref);
  cs_real_t cy  = f_veh[1] / (q_ref * s_ref);
  cs_real_t cl  = f_veh[2] / (q_ref * s_ref);

  /* Instantaneous moment coefficients (vehicle frame) */
  cs_real_t cmx = m_veh[0] / (q_ref * s_ref * l_ref);
  cs_real_t cmy = m_veh[1] / (q_ref * s_ref * l_ref);
  cs_real_t cmz = m_veh[2] / (q_ref * s_ref * l_ref);

  /* -------------------------------------------------------------------
   * Running time-average of forces and moments (for t >= t_avg_start)
   * ------------------------------------------------------------------- */

  static cs_real_3_t s_f_avg = {0., 0., 0.};
  static cs_real_3_t s_m_avg = {0., 0., 0.};
  static cs_real_t   s_t_avg_elapsed = 0.;

  cs_real_t cd_avg = 0., cy_avg = 0., cl_avg = 0.;
  cs_real_t cmx_avg = 0., cmy_avg = 0., cmz_avg = 0.;

  if (t_cur >= t_avg_start) {

    /* Weighted running average:
     *   f_avg <- (f_avg * t_elapsed + f * dt) / (t_elapsed + dt)        */
    cs_real_t t_new = s_t_avg_elapsed + dt;

    for (int i = 0; i < 3; i++) {
      s_f_avg[i] = (s_f_avg[i] * s_t_avg_elapsed + f_veh[i] * dt) / t_new;
      s_m_avg[i] = (s_m_avg[i] * s_t_avg_elapsed + m_veh[i] * dt) / t_new;
    }
    s_t_avg_elapsed = t_new;

    /* Time-averaged coefficients (use current q_ref for normalisation) */
    cd_avg  = s_f_avg[0] / (q_ref * s_ref);
    cy_avg  = s_f_avg[1] / (q_ref * s_ref);
    cl_avg  = s_f_avg[2] / (q_ref * s_ref);
    cmx_avg = s_m_avg[0] / (q_ref * s_ref * l_ref);
    cmy_avg = s_m_avg[1] / (q_ref * s_ref * l_ref);
    cmz_avg = s_m_avg[2] / (q_ref * s_ref * l_ref);
  }

  /* Log to listing */
  bft_printf("  ---- AutoCFD5 Case 1 DDES - Aerodynamic Coefficients ----\n"
             "  t = %.6f s   (yaw = %.1f deg)\n"
             "  V_ref = %8.4f m/s   P_ref = %12.5e Pa  (probe)\n"
             "  q_ref = %10.3f Pa   Sref = %6.4f m^2   Lref = %6.4f m\n"
             "  -- Instantaneous --\n"
             "  Cd = %12.5e   Cy = %12.5e   Cl = %12.5e\n"
             "  CMx= %12.5e   CMy= %12.5e   CMz= %12.5e\n",
             t_cur, yaw_deg,
             v_ref, p_probe,
             q_ref, s_ref, l_ref,
             cd, cy, cl,
             cmx, cmy, cmz);

  if (t_cur >= t_avg_start) {
    bft_printf("  -- Time-averaged (since t = %.2f s, window = %.4f s) --\n"
               "  Cd = %12.5e   Cy = %12.5e   Cl = %12.5e\n"
               "  CMx= %12.5e   CMy= %12.5e   CMz= %12.5e\n",
               t_avg_start, s_t_avg_elapsed,
               cd_avg, cy_avg, cl_avg,
               cmx_avg, cmy_avg, cmz_avg);
  }

  bft_printf("  ---------------------------------------------------\n");

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
                "iteration,time,"
                "V_ref,P_ref,q_ref,"
                "Cd,Cy,Cl,CMx,CMy,CMz,"
                "Cd_avg,Cy_avg,Cl_avg,CMx_avg,CMy_avg,CMz_avg,"
                "t_avg_window\n");
      fprintf(f,
              "%d,%.6e,"
              "%.6e,%.6e,%.6e,"
              "%.6e,%.6e,%.6e,%.6e,%.6e,%.6e,"
              "%.6e,%.6e,%.6e,%.6e,%.6e,%.6e,"
              "%.6e\n",
              nt_cur, t_cur,
              v_ref, p_probe, q_ref,
              cd, cy, cl, cmx, cmy, cmz,
              cd_avg, cy_avg, cl_avg, cmx_avg, cmy_avg, cmz_avg,
              s_t_avg_elapsed);
      fclose(f);
    }
  }
}

END_C_DECLS
