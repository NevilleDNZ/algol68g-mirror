//! @file a68g-platform.h
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

//! @section Synopsis
//!
//! Platform dependent definitions.

#if ! defined (__A68G_PLATFORM_H__)
#define __A68G_PLATFORM_H__

#if defined (__MINGW32__)
#  if ! defined (BUILD_WIN32)
#    define BUILD_WIN32
#  endif
#  include "a68g-config.win32.h"
#  if ! defined (HAVE_GCC)
#    define HAVE_GCC
#  endif
#  undef C_COMPILER
#  undef HAVE_CLANG
#elif defined (__clang__)
#  include "a68g-config.h"
#  if ! defined (HAVE_CLANG)
#    define HAVE_CLANG
#  endif
#  if ! defined (C_COMPILER)
#    define C_COMPILER "clang"
#  endif
#  undef HAVE_GCC
#  undef HAVE_GNU_MPFR
#elif defined (__GNUC__)
#  include "a68g-config.h"
#  if ! defined (HAVE_GCC)
#    define HAVE_GCC
#  endif
#  if ! defined (C_COMPILER)
#    define C_COMPILER "gcc"
#  endif
#  undef HAVE_CLANG
#else
#  error "a68g: abend: a68g requires either gcc or clang"
#endif

// Decide on A68G "LEVEL".
//
// LEVEL 3 uses INT*8, REAL*10 and REAL*16 data types, when available in hardware.
// LEVEL 2 builds generic A68G with INT*4 and REAL*8 data types.
// LEVEL 1 is reserved for (possible) restrictive builds.

#if defined (BUILD_WIN32)
#  define A68_LEVEL 3
#elif defined (HAVE_LONG_TYPES)
#  define A68_LEVEL 3
#else
#  define A68_LEVEL 2
#endif

// R mathlib

#if defined (HAVE_MATHLIB)
#  if !defined (MATHLIB_STANDALONE)
#  define MATHLIB_STANDALONE
#  endif
#endif

// Imported libraries

#undef __cplusplus

#endif
