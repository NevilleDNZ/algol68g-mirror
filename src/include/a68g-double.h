//! @file a68g-double.h
//! @author J. Marcel van der Veer
//!
//! @section Copyright
//!
//! This file is part of Algol68G - an Algol 68 compiler-interpreter.
//! Copyright 2001-2023 J. Marcel van der Veer [algol68g@xs4all.nl].
//!
//! @section License
//!
//! This program is free software; you can redistribute it and/or modify it 
//! under the terms of the GNU General Public License as published by the 
//! Free Software Foundation; either version 3 of the License, or 
//! (at your option) any later version.
//!
//! This program is distributed in the hope that it will be useful, but 
//! WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
//! or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for 
//! more details. You should have received a copy of the GNU General Public 
//! License along with this program. If not, see [http://www.gnu.org/licenses/].

#if !defined (__A68G_DOUBLE_H__)
#define __A68G_DOUBLE_H__

#if (A68_LEVEL >= 3)

#define MODCHK(p, m, c) (!(MODULAR_MATH (p) && (m == M_LONG_BITS)) && (c))

#if defined (HAVE_IEEE_754)
#define CHECK_DOUBLE_REAL(p, u) PRELUDE_ERROR (!finite_double (u), p, ERROR_INFINITE, M_LONG_REAL)
#define CHECK_DOUBLE_COMPLEX(p, u, v)\
  PRELUDE_ERROR (isinf_double (u), p, ERROR_INFINITE, M_LONG_REAL);\
  PRELUDE_ERROR (isinf_double (v), p, ERROR_INFINITE, M_LONG_REAL);
#else
#define CHECK_DOUBLE_REAL(p, u) {;}
#define CHECK_DOUBLE_COMPLEX(p, u, v) {;}
#endif

#define LONG_INT_BASE (9223372036854775808.0q)
#define HW(z) ((z).u[1])
#define LW(z) ((z).u[0])
#define D_NEG(d) ((HW(d) & D_SIGN) != 0)
#define D_LT(u, v) (HW (u) < HW (v) ? A68_TRUE : (HW (u) == HW (v) ? LW (u) < LW (v) : A68_FALSE))

#define acos_double acosq
#define acosh_double acoshq
#define asin_double asinq
#define asinh_double asinhq
#define atan2_double atan2q
#define atan_double atanq
#define atanh_double atanhq
#define cacos_double cacosq
#define cacosh_double cacoshq
#define casin_double casinq
#define casinh_double casinhq
#define catan_double catanq
#define catanh_double catanhq
#define cbrt_double cbrtq
#define ccos_double ccosq
#define ccosh_double ccoshq
#define cexp_double cexpq
#define cimag_double cimagq
#define clog_double clogq
#define cos_double cosq
#define cosh_double coshq
#define creal_double crealq
#define csin_double csinq
#define csinh_double csinhq
#define csqrt_double csqrtq
#define ctan_double ctanq
#define ctanh_double ctanhq
#define erfc_double erfcq
#define erf_double erfq
#define exp_double expq
#define fabs_double fabsq
#define finite_double finiteq
#define floor_double floorq
#define fmod_double fmodq
#define isinf_double isinfq
#define lgamma_double lgammaq
#define log10_double log10q
#define log_double logq
#define pow_double powq
#define sin_double sinq
#define sinh_double sinhq
#define sqrt_double sqrtq
#define tan_double tanq
#define tanh_double tanhq
#define tgamma_double tgammaq
#define trunc_double truncq

#define DBLEQ(z) ((dble_double (A68 (f_entry), (z))).f)

#define ABSQ(n) ((n) >= 0.0q ? (n) : -(n))

#define POP_LONG_COMPLEX(p, re, im) {\
  POP_OBJECT (p, im, A68_LONG_REAL);\
  POP_OBJECT (p, re, A68_LONG_REAL);\
  }

#define set_lw(z, k) {LW(z) = k; HW(z) = 0;}
#define set_hw(z, k) {LW(z) = 0; HW(z) = k;}
#define set_hwlw(z, h, l) {LW (z) = l; HW (z) = h;}
#define D_ZERO(z) (HW (z) == 0 && LW (z) == 0)

#define add_double(p, m, w, u, v)\
  {\
    DOUBLE_NUM_T _ww_;\
    LW (_ww_) = LW (u) + LW (v);\
    HW (_ww_) = HW (u) + HW (v);\
    PRELUDE_ERROR (MODCHK (p, m, HW (_ww_) < HW (v)), p, ERROR_MATH, (m));\
    if (LW (_ww_) < LW (v)) {\
      HW (_ww_)++;\
      PRELUDE_ERROR (MODCHK (p, m, HW (_ww_) < 1), p, ERROR_MATH, (m));\
    }\
    w = _ww_;\
  }

#define sub_double(p, m, w, u, v)\
  {\
    DOUBLE_NUM_T _ww_;\
    LW (_ww_) = LW (u) - LW (v);\
    HW (_ww_) = HW (u) - HW (v);\
    PRELUDE_ERROR (MODCHK (p, m, HW (_ww_) > HW (u)), p, ERROR_MATH, (m));\
    if (LW (_ww_) > LW (u)) {\
      PRELUDE_ERROR (MODCHK (p, m, HW (_ww_) == 0), p, ERROR_MATH, (m));\
      HW (_ww_)--;\
    }\
    w = _ww_;\
  }

static inline DOUBLE_NUM_T dble (DOUBLE_T x)
{
  DOUBLE_NUM_T w;
  w.f = x;
  return w;
}

static inline int sign_double_int (DOUBLE_NUM_T w)
{
  if (D_NEG (w)) {
    return -1;
  } else if (D_ZERO (w)) {
    return 0;
  } else {
    return 1;
  }
}

static inline int sign_double (DOUBLE_NUM_T w)
{
  if (w.f < 0.0q) {
    return -1;
  } else if (w.f == 0.0q) {
    return 0;
  } else {
    return 1;
  }
}

static inline DOUBLE_NUM_T abs_double_int (DOUBLE_NUM_T z)
{
  DOUBLE_NUM_T w;
  LW (w) = LW (z);
  HW (w) = HW (z) & (~D_SIGN);
  return w;
}

static inline DOUBLE_NUM_T neg_double_int (DOUBLE_NUM_T z)
{
  DOUBLE_NUM_T w;
  LW (w) = LW (z);
  if (D_NEG (z)) {
    HW (w) = HW (z) & (~D_SIGN);
  } else {
    HW (w) = HW (z) | D_SIGN;
  }
  return w;
}

extern int sign_double_int (DOUBLE_NUM_T);
extern int sign_double (DOUBLE_NUM_T);
extern int string_to_double_int (NODE_T *, A68_LONG_INT *, char *);
extern DOUBLE_T a68_double_hypot (DOUBLE_T, DOUBLE_T);
extern DOUBLE_T string_to_double (char *, char **);
extern DOUBLE_T inverf_double (DOUBLE_T);
extern DOUBLE_NUM_T abs_double_int (DOUBLE_NUM_T);
extern DOUBLE_NUM_T bits_to_double_int (NODE_T *, char *);
extern DOUBLE_NUM_T dble_double (NODE_T *, REAL_T);
extern DOUBLE_NUM_T double_int_to_double (NODE_T *, DOUBLE_NUM_T);
extern DOUBLE_NUM_T double_strtou (NODE_T *, char *);
extern DOUBLE_NUM_T double_udiv (NODE_T *, MOID_T *, DOUBLE_NUM_T, DOUBLE_NUM_T, int);
extern DOUBLE_T a68_dneginf (void);
extern DOUBLE_T a68_dposinf (void);
extern void deltagammainc_double (DOUBLE_T *, DOUBLE_T *, DOUBLE_T, DOUBLE_T, DOUBLE_T, DOUBLE_T);

extern GPROC genie_infinity_double;
extern GPROC genie_minus_infinity_double;
extern GPROC genie_gamma_inc_g_double;
extern GPROC genie_gamma_inc_f_double;
extern GPROC genie_gamma_inc_h_double;
extern GPROC genie_gamma_inc_gf_double;
extern GPROC genie_abs_double_compl;
extern GPROC genie_abs_double_int;
extern GPROC genie_abs_double;
extern GPROC genie_acos_double_compl;
extern GPROC genie_acosdg_double;
extern GPROC genie_acosh_double_compl;
extern GPROC genie_acosh_double;
extern GPROC genie_acos_double;
extern GPROC genie_acotdg_double;
extern GPROC genie_acot_double;
extern GPROC genie_asec_double;
extern GPROC genie_acsc_double;
extern GPROC genie_add_double_compl;
extern GPROC genie_add_double_bits;
extern GPROC genie_add_double_int;
extern GPROC genie_add_double;
extern GPROC genie_add_double;
extern GPROC genie_and_double_bits;
extern GPROC genie_arg_double_compl;
extern GPROC genie_asin_double_compl;
extern GPROC genie_asindg_double;
extern GPROC genie_asindg_double;
extern GPROC genie_asinh_double_compl;
extern GPROC genie_asinh_double;
extern GPROC genie_asin_double;
extern GPROC genie_atan2dg_double;
extern GPROC genie_atan2_double;
extern GPROC genie_atan_double_compl;
extern GPROC genie_atandg_double;
extern GPROC genie_atanh_double_compl;
extern GPROC genie_atanh_double;
extern GPROC genie_atan_double;
extern GPROC genie_bin_double_int;
extern GPROC genie_clear_double_bits;
extern GPROC genie_conj_double_compl;
extern GPROC genie_cos_double_compl;
extern GPROC genie_cosdg_double;
extern GPROC genie_cosdg_double;
extern GPROC genie_cosh_double_compl;
extern GPROC genie_cosh_double;
extern GPROC genie_cospi_double;
extern GPROC genie_cos_double;
extern GPROC genie_cotdg_double;
extern GPROC genie_cotpi_double;
extern GPROC genie_cot_double;
extern GPROC genie_sec_double;
extern GPROC genie_csc_double;
extern GPROC genie_curt_double;
extern GPROC genie_divab_double_compl;
extern GPROC genie_divab_double;
extern GPROC genie_divab_double;
extern GPROC genie_div_double_compl;
extern GPROC genie_div_double_int;
extern GPROC genie_double_bits_pack;
extern GPROC genie_double_max_bits;
extern GPROC genie_double_max_int;
extern GPROC genie_double_max_real;
extern GPROC genie_double_min_real;
extern GPROC genie_double_small_real;
extern GPROC genie_double_zeroin;
extern GPROC genie_elem_double_bits;
extern GPROC genie_entier_double;
extern GPROC genie_eq_double_compl;
extern GPROC genie_eq_double_bits;
extern GPROC genie_eq_double_int;
extern GPROC genie_eq_double_int;
extern GPROC genie_eq_double;
extern GPROC genie_eq_double;
extern GPROC genie_eq_double;
extern GPROC genie_eq_double;
extern GPROC genie_erfc_double;
extern GPROC genie_erf_double;
extern GPROC genie_exp_double_compl;
extern GPROC genie_exp_double;
extern GPROC genie_gamma_double;
extern GPROC genie_ge_double_bits;
extern GPROC genie_ge_double_int;
extern GPROC genie_ge_double_int;
extern GPROC genie_ge_double;
extern GPROC genie_ge_double;
extern GPROC genie_ge_double;
extern GPROC genie_ge_double;
extern GPROC genie_gt_double_bits;
extern GPROC genie_gt_double_int;
extern GPROC genie_gt_double_int;
extern GPROC genie_gt_double;
extern GPROC genie_gt_double;
extern GPROC genie_gt_double;
extern GPROC genie_gt_double;
extern GPROC genie_i_double_compl;
extern GPROC genie_i_int_double_compl;
extern GPROC genie_im_double_compl;
extern GPROC genie_inverfc_double;
extern GPROC genie_inverf_double;
extern GPROC genie_le_double_bits;
extern GPROC genie_le_double_int;
extern GPROC genie_le_double_int;
extern GPROC genie_lengthen_bits_to_double_bits;
extern GPROC genie_lengthen_double_compl_to_long_mp_complex;
extern GPROC genie_lengthen_complex_to_double_compl;
extern GPROC genie_lengthen_double_int_to_mp;
extern GPROC genie_lengthen_int_to_double_int;
extern GPROC genie_lengthen_double_to_mp;
extern GPROC genie_lengthen_real_to_double;
extern GPROC genie_le_double;
extern GPROC genie_le_double;
extern GPROC genie_le_double;
extern GPROC genie_le_double;
extern GPROC genie_ln_double_compl;
extern GPROC genie_lngamma_double;
extern GPROC genie_ln_double;
extern GPROC genie_log_double;
extern GPROC genie_lt_double_bits;
extern GPROC genie_lt_double_int;
extern GPROC genie_lt_double_int;
extern GPROC genie_lt_double;
extern GPROC genie_lt_double;
extern GPROC genie_lt_double;
extern GPROC genie_lt_double;
extern GPROC genie_minusab_double_compl;
extern GPROC genie_minusab_double_int;
extern GPROC genie_minusab_double_int;
extern GPROC genie_minusab_double;
extern GPROC genie_minusab_double;
extern GPROC genie_minus_double_compl;
extern GPROC genie_minus_double_int;
extern GPROC genie_minus_double;
extern GPROC genie_modab_double_int;
extern GPROC genie_modab_double_int;
extern GPROC genie_mod_double_bits;
extern GPROC genie_mod_double_int;
extern GPROC genie_mul_double_compl;
extern GPROC genie_mul_double_int;
extern GPROC genie_mul_double;
extern GPROC genie_mul_double;
extern GPROC genie_ne_double_compl;
extern GPROC genie_ne_double_bits;
extern GPROC genie_ne_double_int;
extern GPROC genie_ne_double_int;
extern GPROC genie_ne_double_int;
extern GPROC genie_ne_double_int;
extern GPROC genie_ne_double;
extern GPROC genie_ne_double;
extern GPROC genie_ne_double;
extern GPROC genie_ne_double;
extern GPROC genie_ne_double;
extern GPROC genie_ne_double;
extern GPROC genie_ne_double;
extern GPROC genie_ne_double;
extern GPROC genie_next_random_double;
extern GPROC genie_not_double_bits;
extern GPROC genie_odd_double_int;
extern GPROC genie_or_double_bits;
extern GPROC genie_overab_double_int;
extern GPROC genie_overab_double_int;
extern GPROC genie_over_double_bits;
extern GPROC genie_over_double_int;
extern GPROC genie_over_double;
extern GPROC genie_over_double;
extern GPROC genie_pi_double;
extern GPROC genie_plusab_double_compl;
extern GPROC genie_plusab_double_int;
extern GPROC genie_plusab_double_int;
extern GPROC genie_plusab_double;
extern GPROC genie_pow_double_compl_int;
extern GPROC genie_pow_double_int_int;
extern GPROC genie_pow_double;
extern GPROC genie_pow_double_int;
extern GPROC genie_re_double_compl;
extern GPROC genie_rol_double_bits;
extern GPROC genie_ror_double_bits;
extern GPROC genie_round_double;
extern GPROC genie_set_double_bits;
extern GPROC genie_shl_double_bits;
extern GPROC genie_shorten_double_compl_to_complex;
extern GPROC genie_shorten_double_bits_to_bits;
extern GPROC genie_shorten_long_int_to_int;
extern GPROC genie_shorten_long_mp_complex_to_double_compl;
extern GPROC genie_shorten_mp_to_double_int;
extern GPROC genie_shorten_mp_to_double;
extern GPROC genie_shorten_double_to_real;
extern GPROC genie_shr_double_bits;
extern GPROC genie_sign_double_int;
extern GPROC genie_sign_double;
extern GPROC genie_sin_double_compl;
extern GPROC genie_sindg_double;
extern GPROC genie_sinh_double_compl;
extern GPROC genie_sinh_double;
extern GPROC genie_sinpi_double;
extern GPROC genie_sin_double;
extern GPROC genie_sqrt_double_compl;
extern GPROC genie_sqrt_double;
extern GPROC genie_sqrt_double;
extern GPROC genie_sqrt_double;
extern GPROC genie_sub_double_compl;
extern GPROC genie_sub_double_bits;
extern GPROC genie_sub_double_int;
extern GPROC genie_sub_double;
extern GPROC genie_sub_double;
extern GPROC genie_tan_double_compl;
extern GPROC genie_tandg_double;
extern GPROC genie_tanh_double_compl;
extern GPROC genie_tanh_double;
extern GPROC genie_tanpi_double;
extern GPROC genie_tan_double;
extern GPROC genie_timesab_double_compl;
extern GPROC genie_timesab_double_int;
extern GPROC genie_timesab_double_int;
extern GPROC genie_timesab_double;
extern GPROC genie_timesab_double;
extern GPROC genie_times_double_bits;
extern GPROC genie_widen_double_int_to_double;
extern GPROC genie_xor_double_bits;
extern GPROC genie_beta_inc_cf_double;
extern GPROC genie_beta_double;
extern GPROC genie_ln_beta_double;
extern GPROC genie_gamma_inc_double;
extern GPROC genie_zero_double_int;

#endif

#endif
