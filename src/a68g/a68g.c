//! @file a68g.c
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
//! Algol 68 Genie main driver.

#include "a68g.h"
#include "a68g-genie.h"
#include "a68g-listing.h"
#include "a68g-mp.h"
#include "a68g-optimiser.h"
#include "a68g-options.h"
#include "a68g-parser.h"
#include "a68g-postulates.h"
#include "a68g-prelude.h"
#include "a68g-prelude-mathlib.h"

#if defined (HAVE_MATHLIB)
#include <Rmath.h>
#endif

GLOBALS_T common;

#define EXTENSIONS 11
static char *extensions[EXTENSIONS] = {
  NO_TEXT,
  ".a68", ".A68",
  ".a68g", ".A68G",
  ".algol", ".ALGOL",
  ".algol68", ".ALGOL68",
  ".algol68g", ".ALGOL68G"
};

void compiler_interpreter (void);

//! @brief Verbose statistics, only useful when debugging a68g.

void verbosity (void)
{
#if defined (A68_DEBUG)
  ;
#else
  ;
#endif
}

//! @brief State license of running a68g image.

void state_license (FILE_T f)
{
#define PR(s)\
  ASSERT (snprintf(A68 (output_line), SNPRINTF_SIZE, "%s\n", (s)) >= 0);\
  WRITE (f, A68 (output_line));
//
  if (f == STDOUT_FILENO) {
    io_close_tty_line ();
  }
  ASSERT (snprintf (A68 (output_line), SNPRINTF_SIZE, "Algol 68 Genie %s\n", PACKAGE_VERSION) >= 0);
  WRITE (f, A68 (output_line));
  ASSERT (snprintf (A68 (output_line), SNPRINTF_SIZE, "Copyright 2001-2023 %s.\n", PACKAGE_BUGREPORT) >= 0);
  WRITE (f, A68 (output_line));
  PR ("");
  ASSERT (snprintf (A68 (output_line), SNPRINTF_SIZE, "This is free software covered by the GNU General Public License.\n") >= 0);
  WRITE (f, A68 (output_line));
  ASSERT (snprintf (A68 (output_line), SNPRINTF_SIZE, "There is ABSOLUTELY NO WARRANTY for Algol 68 Genie;\n") >= 0);
  WRITE (f, A68 (output_line));
  ASSERT (snprintf (A68 (output_line), SNPRINTF_SIZE, "not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n") >= 0);
  WRITE (f, A68 (output_line));
  PR ("See the GNU General Public License for more details.");
  PR ("");
  ASSERT (snprintf (A68 (output_line), SNPRINTF_SIZE, "Please report bugs to %s.\n", PACKAGE_BUGREPORT) >= 0);
  WRITE (f, A68 (output_line));
#undef PR
}

//! @brief State version of running a68g image.

void state_version (FILE_T f)
{
#define PR(s)\
  ASSERT (snprintf(A68 (output_line), SNPRINTF_SIZE, "%s\n", (s)) >= 0);\
  WRITE (f, A68 (output_line));
//
  if (f == STDOUT_FILENO) {
    io_close_tty_line ();
  }
  state_license (f);
  PR ("");
#if defined (BUILD_WIN32)
  ASSERT (snprintf (A68 (output_line), SNPRINTF_SIZE, "WIN32 executable\n") >= 0);
  WRITE (f, A68 (output_line));
  WRITELN (f, "");
#endif
#if (A68_LEVEL >= 3)
  ASSERT (snprintf (A68 (output_line), SNPRINTF_SIZE, "With hardware support for long modes\n") >= 0);
  WRITE (f, A68 (output_line));
#endif
#if defined (BUILD_A68_COMPILER)
  ASSERT (snprintf (A68 (output_line), SNPRINTF_SIZE, "With plugin-compilation support\n") >= 0);
  WRITE (f, A68 (output_line));
#endif
#if defined (BUILD_PARALLEL_CLAUSE)
  ASSERT (snprintf (A68 (output_line), SNPRINTF_SIZE, "With parallel-clause support\n") >= 0);
  WRITE (f, A68 (output_line));
#endif
#if defined (HAVE_POSTGRESQL)
  ASSERT (snprintf (A68 (output_line), SNPRINTF_SIZE, "With PostgreSQL support\n") >= 0);
  WRITE (f, A68 (output_line));
#endif
#if defined (BUILD_HTTP)
  ASSERT (snprintf (A68 (output_line), SNPRINTF_SIZE, "With TCP/IP support\n") >= 0);
  WRITE (f, A68 (output_line));
#endif
#if defined (HAVE_GNU_MPFR)
  ASSERT (snprintf (A68 (output_line), SNPRINTF_SIZE, "With GNU MP %s\n", gmp_version) >= 0);
  WRITE (f, A68 (output_line));
  ASSERT (snprintf (A68 (output_line), SNPRINTF_SIZE, "With GNU MPFR %s\n", mpfr_get_version ()) >= 0);
  WRITE (f, A68 (output_line));
#endif
#if defined (HAVE_MATHLIB)
  ASSERT (snprintf (A68 (output_line), SNPRINTF_SIZE, "With mathlib from R %s\n", R_VERSION_STRING) >= 0);
  WRITE (f, A68 (output_line));
#endif
#if defined (HAVE_GSL)
  ASSERT (snprintf (A68 (output_line), SNPRINTF_SIZE, "With GNU Scientific Library %s\n", GSL_VERSION) >= 0);
  WRITE (f, A68 (output_line));
#endif
#if defined (HAVE_GNU_PLOTUTILS)
  ASSERT (snprintf (A68 (output_line), SNPRINTF_SIZE, "With GNU plotutils %s\n", PL_LIBPLOT_VER_STRING) >= 0);
  WRITE (f, A68 (output_line));
#endif
#if defined (HAVE_CURSES)
  ASSERT (snprintf (A68 (output_line), SNPRINTF_SIZE, "With curses %s\n", NCURSES_VERSION) >= 0);
  WRITE (f, A68 (output_line));
#endif
#if defined (_CS_GNU_LIBC_VERSION) && defined (BUILD_UNIX)
  if (confstr (_CS_GNU_LIBC_VERSION, A68 (input_line), BUFFER_SIZE) > (size_t) 0) {
    ASSERT (snprintf (A68 (output_line), SNPRINTF_SIZE, "GNU libc version %s\n", A68 (input_line)) >= 0);
    WRITE (f, A68 (output_line));
  }
#if (defined (BUILD_PARALLEL_CLAUSE) && defined (_CS_GNU_LIBPTHREAD_VERSION))
  if (confstr (_CS_GNU_LIBPTHREAD_VERSION, A68 (input_line), BUFFER_SIZE) > (size_t) 0) {
    ASSERT (snprintf (A68 (output_line), SNPRINTF_SIZE, "GNU libpthread version %s\n", A68 (input_line)) >= 0);
    WRITE (f, A68 (output_line));
  }
#endif
#endif

#define RSIZE(n) (unt) (sizeof (n) / sizeof (int))
#if defined (BUILD_A68_COMPILER) && defined (C_COMPILER)
  ASSERT (snprintf (A68 (output_line), SNPRINTF_SIZE, "Build level %d.%x%x%x%x %s %s\n", A68_LEVEL, RSIZE (INT_T), RSIZE (REAL_T), RSIZE (MP_INT_T), RSIZE (MP_REAL_T), C_COMPILER, __DATE__) >= 0);
#else
  ASSERT (snprintf (A68 (output_line), SNPRINTF_SIZE, "Build level %d.%x%x%x%x %s\n", A68_LEVEL, RSIZE (INT_T), RSIZE (REAL_T), RSIZE (MP_INT_T), RSIZE (MP_REAL_T), __DATE__) >= 0);
#endif
#undef RSIZE
WRITE (f, A68 (output_line));

#undef PR
}

//! @brief Give brief help if someone types 'a68g --help'.

void online_help (FILE_T f)
{
  if (f == STDOUT_FILENO) {
    io_close_tty_line ();
  }
  state_license (f);
  ASSERT (snprintf (A68 (output_line), SNPRINTF_SIZE, "Usage: %s [options | filename]", A68 (a68_cmd_name)) >= 0);
  WRITELN (f, A68 (output_line));
  ASSERT (snprintf (A68 (output_line), SNPRINTF_SIZE, "For help: %s --apropos [keyword]", A68 (a68_cmd_name)) >= 0);
  WRITELN (f, A68 (output_line));
}

//! @brief Start book keeping for a phase.

void announce_phase (char *t)
{
  if (OPTION_VERBOSE (&A68_JOB)) {
    ASSERT (snprintf (A68 (output_line), SNPRINTF_SIZE, "%s: %s", A68 (a68_cmd_name), t) >= 0);
    io_close_tty_line ();
    WRITE (STDOUT_FILENO, A68 (output_line));
  }
}

//! @brief Test extension and strip.

BOOL_T strip_extension (char *ext)
{
  if (ext == NO_TEXT) {
    return A68_FALSE;
  }
  int nlen = (int) strlen (FILE_SOURCE_NAME (&A68_JOB)), xlen = (int) strlen (ext);
  if (nlen > xlen && strcmp (&(FILE_SOURCE_NAME (&A68_JOB)[nlen - xlen]), ext) == 0) {
    char *fn = (char *) get_heap_space ((size_t) (nlen + 1));
    bufcpy (fn, FILE_SOURCE_NAME (&A68_JOB), nlen);
    fn[nlen - xlen] = NULL_CHAR;
    a68_free (FILE_GENERIC_NAME (&A68_JOB));
    FILE_GENERIC_NAME (&A68_JOB) = new_string (fn, NO_TEXT);
    a68_free (fn);
    return A68_TRUE;
  } else {
    return A68_FALSE;
  }
}

//! @brief Try opening with an extension.

void open_with_extensions (void)
{
  FILE_SOURCE_FD (&A68_JOB) = -1;
  for (int k = 0; k < EXTENSIONS && FILE_SOURCE_FD (&A68_JOB) == -1; k++) {
    int len;
    char *fn = NULL;
    if (extensions[k] == NO_TEXT) {
      len = (int) strlen (FILE_INITIAL_NAME (&A68_JOB)) + 1;
      fn = (char *) get_heap_space ((size_t) len);
      bufcpy (fn, FILE_INITIAL_NAME (&A68_JOB), len);
    } else {
      len = (int) strlen (FILE_INITIAL_NAME (&A68_JOB)) + (int) strlen (extensions[k]) + 1;
      fn = (char *) get_heap_space ((size_t) len);
      bufcpy (fn, FILE_INITIAL_NAME (&A68_JOB), len);
      bufcat (fn, extensions[k], len);
    }
    FILE_SOURCE_FD (&A68_JOB) = open (fn, O_RDONLY | O_BINARY);
    if (FILE_SOURCE_FD (&A68_JOB) != -1) {
      BOOL_T cont = A68_TRUE;
      a68_free (FILE_SOURCE_NAME (&A68_JOB));
      a68_free (FILE_GENERIC_NAME (&A68_JOB));
      FILE_SOURCE_NAME (&A68_JOB) = new_string (fn, NO_TEXT);
      FILE_GENERIC_NAME (&A68_JOB) = new_string (a68_basename (fn), NO_TEXT);
      FILE_PATH (&A68_JOB) = new_string (a68_dirname (fn), NO_TEXT);
      for (int l = 0; l < EXTENSIONS && cont; l++) {
        if (strip_extension (extensions[l])) {
          cont = A68_FALSE;
        }
      }
    }
    a68_free (fn);
  }
}

//! @brief Remove a regular file.

void a68_rm (char *fn)
{
  struct stat path_stat;
  if (stat (fn, &path_stat) == 0) {
    if (S_ISREG (path_stat.st_mode)) {
      ABEND (remove (fn) != 0, ERROR_ACTION, FILE_OBJECT_NAME (&A68_JOB));
    }
  }
}

//! @brief Drives compilation and interpretation.

void compiler_interpreter (void)
{
  int len, num;
#if defined (BUILD_A68_COMPILER)
  BOOL_T emitted = A68_FALSE;
#endif
  TREE_LISTING_SAFE (&A68_JOB) = A68_FALSE;
  CROSS_REFERENCE_SAFE (&A68_JOB) = A68_FALSE;
  A68 (in_execution) = A68_FALSE;
  A68 (new_nodes) = 0;
  A68 (new_modes) = 0;
  A68 (new_postulates) = 0;
  A68 (new_node_infos) = 0;
  A68 (new_genie_infos) = 0;
  A68 (symbol_table_count) = 0;
  A68 (mode_count) = 0;
  A68 (node_register) = NO_VAR;
  init_postulates ();
  A68 (do_confirm_exit) = A68_TRUE;
  A68 (f_entry) = NO_NODE;
  A68 (global_level) = 0;
  A68 (max_lex_lvl) = 0;
  A68_PARSER (stop_scanner) = A68_FALSE;
  A68_PARSER (read_error) = A68_FALSE;
  A68_PARSER (no_preprocessing) = A68_FALSE;
  A68_PARSER (reductions) = 0;
  A68_PARSER (tag_number) = 0;
  A68 (curses_mode) = A68_FALSE;
  A68 (top_soid_list) = NO_SOID;
  A68 (max_simplout_size) = 0;
  A68_MON (in_monitor) = A68_FALSE;
  A68_MP (mp_ln_scale_size) = -1;
  A68_MP (mp_ln_10_size) = -1;
  A68_MP (mp_gamma_size) = -1;
  A68_MP (mp_one_size) = -1;
  A68_MP (mp_pi_size) = -1;
// File set-up.
  SCAN_ERROR (FILE_INITIAL_NAME (&A68_JOB) == NO_TEXT, NO_LINE, NO_TEXT, ERROR_NO_SOURCE_FILE);
  FILE_BINARY_OPENED (&A68_JOB) = A68_FALSE;
  FILE_BINARY_WRITEMOOD (&A68_JOB) = A68_TRUE;
  FILE_PLUGIN_OPENED (&A68_JOB) = A68_FALSE;
  FILE_PLUGIN_WRITEMOOD (&A68_JOB) = A68_TRUE;
  FILE_LISTING_OPENED (&A68_JOB) = A68_FALSE;
  FILE_LISTING_WRITEMOOD (&A68_JOB) = A68_TRUE;
  FILE_OBJECT_OPENED (&A68_JOB) = A68_FALSE;
  FILE_OBJECT_WRITEMOOD (&A68_JOB) = A68_TRUE;
  FILE_PRETTY_OPENED (&A68_JOB) = A68_FALSE;
  FILE_SCRIPT_OPENED (&A68_JOB) = A68_FALSE;
  FILE_SCRIPT_WRITEMOOD (&A68_JOB) = A68_FALSE;
  FILE_SOURCE_OPENED (&A68_JOB) = A68_FALSE;
  FILE_SOURCE_WRITEMOOD (&A68_JOB) = A68_FALSE;
  FILE_DIAGS_OPENED (&A68_JOB) = A68_FALSE;
  FILE_DIAGS_WRITEMOOD (&A68_JOB) = A68_TRUE;
// Open the source file. 
// Open it for binary reading for systems that require so (Win32).
// Accept various silent extensions.
  errno = 0;
  FILE_SOURCE_NAME (&A68_JOB) = NO_TEXT;
  FILE_GENERIC_NAME (&A68_JOB) = NO_TEXT;
  open_with_extensions ();
  if (FILE_SOURCE_NAME (&A68_JOB) == NO_TEXT) {
    errno = ENOENT;
    SCAN_ERROR (A68_TRUE, NO_LINE, NO_TEXT, ERROR_SOURCE_FILE_OPEN);
  } else {
    struct stat path_stat;
    errno = 0;
    SCAN_ERROR (stat (FILE_SOURCE_NAME (&A68_JOB), &path_stat) != 0, NO_LINE, NO_TEXT, ERROR_SOURCE_FILE_OPEN);
    SCAN_ERROR (S_ISDIR (path_stat.st_mode), NO_LINE, NO_TEXT, ERROR_IS_DIRECTORY);
    SCAN_ERROR (!S_ISREG (path_stat.st_mode), NO_LINE, NO_TEXT, ERROR_NO_REGULAR_FILE);
  }
  if (FILE_SOURCE_FD (&A68_JOB) == -1) {
    scan_error (NO_LINE, NO_TEXT, ERROR_SOURCE_FILE_OPEN);
  }
  ABEND (FILE_SOURCE_NAME (&A68_JOB) == NO_TEXT, ERROR_INTERNAL_CONSISTENCY, __func__);
  ABEND (FILE_GENERIC_NAME (&A68_JOB) == NO_TEXT, ERROR_INTERNAL_CONSISTENCY, __func__);
// Object file.
  len = 1 + (int) strlen (FILE_GENERIC_NAME (&A68_JOB)) + (int) strlen (OBJECT_EXTENSION);
  FILE_OBJECT_NAME (&A68_JOB) = (char *) get_heap_space ((size_t) len);
  bufcpy (FILE_OBJECT_NAME (&A68_JOB), FILE_GENERIC_NAME (&A68_JOB), len);
  bufcat (FILE_OBJECT_NAME (&A68_JOB), OBJECT_EXTENSION, len);
// Binary.
  len = 1 + (int) strlen (FILE_GENERIC_NAME (&A68_JOB)) + (int) strlen (PLUGIN_EXTENSION);
  FILE_BINARY_NAME (&A68_JOB) = (char *) get_heap_space ((size_t) len);
  bufcpy (FILE_BINARY_NAME (&A68_JOB), FILE_GENERIC_NAME (&A68_JOB), len);
  bufcat (FILE_BINARY_NAME (&A68_JOB), BINARY_EXTENSION, len);
// Library file.
  len = 1 + (int) strlen (FILE_GENERIC_NAME (&A68_JOB)) + (int) strlen (PLUGIN_EXTENSION);
  FILE_PLUGIN_NAME (&A68_JOB) = (char *) get_heap_space ((size_t) len);
  bufcpy (FILE_PLUGIN_NAME (&A68_JOB), FILE_GENERIC_NAME (&A68_JOB), len);
  bufcat (FILE_PLUGIN_NAME (&A68_JOB), PLUGIN_EXTENSION, len);
// Listing file.
  len = 1 + (int) strlen (FILE_GENERIC_NAME (&A68_JOB)) + (int) strlen (LISTING_EXTENSION);
  FILE_LISTING_NAME (&A68_JOB) = (char *) get_heap_space ((size_t) len);
  bufcpy (FILE_LISTING_NAME (&A68_JOB), FILE_GENERIC_NAME (&A68_JOB), len);
  bufcat (FILE_LISTING_NAME (&A68_JOB), LISTING_EXTENSION, len);
// Pretty file.
  len = 1 + (int) strlen (FILE_GENERIC_NAME (&A68_JOB)) + (int) strlen (PRETTY_EXTENSION);
  FILE_PRETTY_NAME (&A68_JOB) = (char *) get_heap_space ((size_t) len);
  bufcpy (FILE_PRETTY_NAME (&A68_JOB), FILE_GENERIC_NAME (&A68_JOB), len);
  bufcat (FILE_PRETTY_NAME (&A68_JOB), PRETTY_EXTENSION, len);
// Script file.
  len = 1 + (int) strlen (FILE_GENERIC_NAME (&A68_JOB)) + (int) strlen (SCRIPT_EXTENSION);
  FILE_SCRIPT_NAME (&A68_JOB) = (char *) get_heap_space ((size_t) len);
  bufcpy (FILE_SCRIPT_NAME (&A68_JOB), FILE_GENERIC_NAME (&A68_JOB), len);
  bufcat (FILE_SCRIPT_NAME (&A68_JOB), SCRIPT_EXTENSION, len);
// Parser.
  a68_parser ();
  if (TOP_NODE (&A68_JOB) == NO_NODE) {
    errno = ECANCELED;
    ABEND (A68_TRUE, ERROR_SOURCE_FILE_EMPTY, NO_TEXT);
  }
// Portability checker.
  if (ERROR_COUNT (&A68_JOB) == 0) {
    announce_phase ("portability checker");
    portcheck (TOP_NODE (&A68_JOB));
    verbosity ();
  }
// Finalise syntax tree.
  if (ERROR_COUNT (&A68_JOB) == 0) {
    num = 0;
    renumber_nodes (TOP_NODE (&A68_JOB), &num);
    NEST (TABLE (TOP_NODE (&A68_JOB))) = A68 (symbol_table_count) = 3;
    reset_symbol_table_nest_count (TOP_NODE (&A68_JOB));
    verbosity ();
  }
//
  if (A68_MP (varying_mp_digits) > width_to_mp_digits (MP_MAX_DECIMALS)) {
    diagnostic (A68_WARNING, NO_NODE, WARNING_PRECISION, NO_LINE, 0, A68_MP (varying_mp_digits) * LOG_MP_RADIX);
  }
// Compiler.
  if (ERROR_COUNT (&A68_JOB) == 0 && OPTION_OPT_LEVEL (&A68_JOB) > NO_OPTIMISE) {
    announce_phase ("optimiser (code generator)");
    num = 0;
    renumber_nodes (TOP_NODE (&A68_JOB), &num);
    A68 (node_register) = (NODE_T **) get_heap_space ((size_t) num * sizeof (NODE_T));
    ABEND (A68 (node_register) == NO_VAR, ERROR_ACTION, __func__);
    register_nodes (TOP_NODE (&A68_JOB));
    FILE_OBJECT_FD (&A68_JOB) = open (FILE_OBJECT_NAME (&A68_JOB), O_WRONLY | O_CREAT | O_TRUNC, A68_PROTECTION);
    ABEND (FILE_OBJECT_FD (&A68_JOB) == -1, ERROR_ACTION, FILE_OBJECT_NAME (&A68_JOB));
    FILE_OBJECT_OPENED (&A68_JOB) = A68_TRUE;
    compiler (FILE_OBJECT_FD (&A68_JOB));
    ASSERT (close (FILE_OBJECT_FD (&A68_JOB)) == 0);
    FILE_OBJECT_OPENED (&A68_JOB) = A68_FALSE;
#if defined (BUILD_A68_COMPILER)
    emitted = A68_TRUE;
#endif
  }
#if defined (BUILD_A68_COMPILER)
// Only compile C if the A68 compiler found no errors (constant folder for instance).
  if (ERROR_COUNT (&A68_JOB) == 0 && OPTION_OPT_LEVEL (&A68_JOB) > 0 && !OPTION_RUN_SCRIPT (&A68_JOB)) {
    BUFFER cmd, options;
    BUFCLR (cmd);
    BUFCLR (options);
    if (OPTION_RERUN (&A68_JOB) == A68_FALSE) {
      announce_phase ("optimiser (code compiler)");
      errno = 0;
//
// Compilation on Linux, BSD.
// Build shared library using gcc or clang.
// TODO: One day this should be all portable between platforms.
//
// -fno-stack-protector is needed for OS's that enforce -fstack-protector-strong which may give
// undefined reference to `__stack_chk_fail_local'
// by ld. Ubuntu is one such.
//
      ASSERT (snprintf (options, SNPRINTF_SIZE, "%s %s", optimisation_option (), A68_GCC_OPTIONS) >= 0);
#if defined (HAVE_PIC)
      bufcat (options, " ", BUFFER_SIZE);
      bufcat (options, HAVE_PIC, BUFFER_SIZE);
#endif
      ASSERT (snprintf (cmd, SNPRINTF_SIZE, "%s -I%s %s -c -o \"%s\" \"%s\"", C_COMPILER, INCLUDEDIR, options, FILE_BINARY_NAME (&A68_JOB), FILE_OBJECT_NAME (&A68_JOB)) >= 0);
      ABEND (system (cmd) != 0, ERROR_ACTION, cmd);
      ASSERT (snprintf (cmd, SNPRINTF_SIZE, "ld -export-dynamic -shared -o \"%s\" \"%s\"", FILE_PLUGIN_NAME (&A68_JOB), FILE_BINARY_NAME (&A68_JOB)) >= 0);
      ABEND (system (cmd) != 0, ERROR_ACTION, cmd);
      a68_rm (FILE_BINARY_NAME (&A68_JOB));
    }
    verbosity ();
  }
#else
  if (OPTION_OPT_LEVEL (&A68_JOB) > 0) {
    diagnostic (A68_WARNING | A68_FORCE_DIAGNOSTICS, TOP_NODE (&A68_JOB), WARNING_OPTIMISATION);
  }
#endif
// Indenter.
  if (ERROR_COUNT (&A68_JOB) == 0 && OPTION_PRETTY (&A68_JOB)) {
    announce_phase ("indenter");
    indenter (&A68_JOB);
    verbosity ();
  }
// Interpreter.
  diagnostics_to_terminal (TOP_LINE (&A68_JOB), A68_ALL_DIAGNOSTICS);
// Restore seed for rng.
  GetRNGstate ();
  A68 (f_entry) = TOP_NODE (&A68_JOB);
//
  if (ERROR_COUNT (&A68_JOB) == 0 && OPTION_COMPILE (&A68_JOB) == A68_FALSE && (OPTION_CHECK_ONLY (&A68_JOB) ? OPTION_RUN (&A68_JOB) : A68_TRUE)) {
#if defined (BUILD_A68_COMPILER)
    void *compile_plugin;
#endif
    A68 (close_tty_on_exit) = A68_FALSE;        // Assuming no runtime errors a priori
#if defined (BUILD_A68_COMPILER)
    if (OPTION_RUN_SCRIPT (&A68_JOB)) {
      rewrite_script_source ();
    }
#endif
    if (OPTION_DEBUG (&A68_JOB)) {
      state_license (STDOUT_FILENO);
    }
#if defined (BUILD_A68_COMPILER)
    if (OPTION_OPT_LEVEL (&A68_JOB) > 0) {
      char plugin_name[BUFFER_SIZE];
      void *a68_plugin;
      struct stat srcstat, objstat;
      int ret;
      announce_phase ("dynamic linker");
      ASSERT (snprintf (plugin_name, SNPRINTF_SIZE, "%s", FILE_PLUGIN_NAME (&A68_JOB)) >= 0);
// Correction when pwd is outside LD_PLUGIN_PATH.
// The DL cannot be loaded if it is.
      if (strcmp (plugin_name, a68_basename (plugin_name)) == 0) {
        ASSERT (snprintf (plugin_name, SNPRINTF_SIZE, "./%s", FILE_PLUGIN_NAME (&A68_JOB)) >= 0);
      }
// Check whether we are doing something rash.
      ret = stat (FILE_SOURCE_NAME (&A68_JOB), &srcstat);
      ABEND (ret != 0, ERROR_ACTION, FILE_SOURCE_NAME (&A68_JOB));
      ret = stat (plugin_name, &objstat);
      ABEND (ret != 0, ERROR_ACTION, plugin_name);
      if (OPTION_RERUN (&A68_JOB)) {
        ABEND (ST_MTIME (&srcstat) > ST_MTIME (&objstat), "plugin outdates source", "cannot RERUN");
      }
// First load a68g itself so compiler code can resolve a68g symbols.
      a68_plugin = dlopen (NULL, RTLD_NOW | RTLD_GLOBAL);
      ABEND (a68_plugin == NULL, ERROR_CANNOT_OPEN_PLUGIN, dlerror ());
// Then load compiler code.
      compile_plugin = dlopen (plugin_name, RTLD_NOW | RTLD_GLOBAL);
      ABEND (compile_plugin == NULL, ERROR_CANNOT_OPEN_PLUGIN, dlerror ());
    } else {
      compile_plugin = NULL;
    }
    announce_phase ("genie");
    genie (compile_plugin);
// Unload compiler plugin.
    if (OPTION_OPT_LEVEL (&A68_JOB) > 0) {
      int ret = dlclose (compile_plugin);
      ABEND (ret != 0, ERROR_ACTION, dlerror ());
    }
#else
    announce_phase ("genie");
    genie (NO_NODE);
#endif
// Free heap allocated by genie.
    genie_free (TOP_NODE (&A68_JOB));
// Store seed for rng.
    announce_phase ("store rng state");
    PutRNGstate ();
// Normal end of program.
    diagnostics_to_terminal (TOP_LINE (&A68_JOB), A68_RUNTIME_ERROR);
    if (OPTION_DEBUG (&A68_JOB) || OPTION_TRACE (&A68_JOB) || OPTION_CLOCK (&A68_JOB)) {
      ASSERT (snprintf (A68 (output_line), SNPRINTF_SIZE, "\nGenie finished in %.2f seconds\n", seconds () - A68 (cputime_0)) >= 0);
      WRITE (STDOUT_FILENO, A68 (output_line));
    }
    verbosity ();
  }
// Setting up listing file.
  announce_phase ("write listing");
  if (OPTION_MOID_LISTING (&A68_JOB) || OPTION_TREE_LISTING (&A68_JOB) || OPTION_SOURCE_LISTING (&A68_JOB) || OPTION_OBJECT_LISTING (&A68_JOB) || OPTION_STATISTICS_LISTING (&A68_JOB)) {
    FILE_LISTING_FD (&A68_JOB) = open (FILE_LISTING_NAME (&A68_JOB), O_WRONLY | O_CREAT | O_TRUNC, A68_PROTECTION);
    ABEND (FILE_LISTING_FD (&A68_JOB) == -1, ERROR_ACTION, __func__);
    FILE_LISTING_OPENED (&A68_JOB) = A68_TRUE;
  } else {
    FILE_LISTING_OPENED (&A68_JOB) = A68_FALSE;
  }
// Write listing.
  if (FILE_LISTING_OPENED (&A68_JOB)) {
    A68 (heap_is_fluid) = A68_TRUE;
    write_listing_header ();
    write_source_listing ();
    write_tree_listing ();
    if (ERROR_COUNT (&A68_JOB) == 0 && OPTION_OPT_LEVEL (&A68_JOB) > 0) {
      write_object_listing ();
    }
    write_listing ();
    ASSERT (close (FILE_LISTING_FD (&A68_JOB)) == 0);
    FILE_LISTING_OPENED (&A68_JOB) = A68_FALSE;
    verbosity ();
  }
// Cleaning up the intermediate files.
#if defined (BUILD_A68_COMPILER)
  announce_phase ("clean up intermediate files");
  if (OPTION_OPT_LEVEL (&A68_JOB) >= OPTIMISE_0 && OPTION_REGRESSION_TEST (&A68_JOB) && !OPTION_KEEP (&A68_JOB)) {
    if (emitted) {
      a68_rm (FILE_OBJECT_NAME (&A68_JOB));
    }
    a68_rm (FILE_PLUGIN_NAME (&A68_JOB));
  }
  if (OPTION_RUN_SCRIPT (&A68_JOB) && !OPTION_KEEP (&A68_JOB)) {
    if (emitted) {
      a68_rm (FILE_OBJECT_NAME (&A68_JOB));
    }
    a68_rm (FILE_SOURCE_NAME (&A68_JOB));
    a68_rm (FILE_PLUGIN_NAME (&A68_JOB));
  } else if (OPTION_COMPILE (&A68_JOB)) {
    build_script ();
    if (!OPTION_KEEP (&A68_JOB)) {
      if (emitted) {
        a68_rm (FILE_OBJECT_NAME (&A68_JOB));
      }
      a68_rm (FILE_PLUGIN_NAME (&A68_JOB));
    }
  } else if (OPTION_OPT_LEVEL (&A68_JOB) == OPTIMISE_0 && !OPTION_KEEP (&A68_JOB)) {
    if (emitted) {
      a68_rm (FILE_OBJECT_NAME (&A68_JOB));
    }
    a68_rm (FILE_PLUGIN_NAME (&A68_JOB));
  } else if (OPTION_OPT_LEVEL (&A68_JOB) > OPTIMISE_0 && !OPTION_KEEP (&A68_JOB)) {
    if (emitted) {
      a68_rm (FILE_OBJECT_NAME (&A68_JOB));
    }
  } else if (OPTION_RERUN (&A68_JOB) && !OPTION_KEEP (&A68_JOB)) {
    if (emitted) {
      a68_rm (FILE_OBJECT_NAME (&A68_JOB));
    }
  }
#endif
}

//! @brief Exit a68g in an orderly manner.

void a68_exit (int code)
{
  announce_phase ("exit");
#if defined (HAVE_GNU_MPFR)
  mpfr_free_cache ();
#endif
// Close unclosed files, remove temp files.
  free_file_entries ();
// Close the terminal.
  if (A68 (close_tty_on_exit) || OPTION_REGRESSION_TEST (&A68_JOB)) {
    io_close_tty_line ();
  } else if (OPTION_VERBOSE (&A68_JOB)) {
    io_close_tty_line ();
  }
#if defined (HAVE_CURSES)
// "curses" might still be open if it was not closed from A68, or the program
// was interrupted, or a runtime error occured. That wreaks havoc on your
// terminal. 
  genie_curses_end (NO_NODE);
#endif
// Clean up stale things.
  free_syntax_tree (TOP_NODE (&A68_JOB));
  free_option_list (OPTION_LIST (&A68_JOB));
  a68_free (A68 (node_register));
  a68_free (A68 (options));
//
  discard_heap ();
//
  a68_free (FILE_PATH (&A68_JOB));
  a68_free (FILE_INITIAL_NAME (&A68_JOB));
  a68_free (FILE_GENERIC_NAME (&A68_JOB));
  a68_free (FILE_SOURCE_NAME (&A68_JOB));
  a68_free (FILE_LISTING_NAME (&A68_JOB));
  a68_free (FILE_OBJECT_NAME (&A68_JOB));
  a68_free (FILE_PLUGIN_NAME (&A68_JOB));
  a68_free (FILE_BINARY_NAME (&A68_JOB));
  a68_free (FILE_PRETTY_NAME (&A68_JOB));
  a68_free (FILE_SCRIPT_NAME (&A68_JOB));
  a68_free (FILE_DIAGS_NAME (&A68_JOB));
//
  a68_free (A68_MP (mp_one));
  a68_free (A68_MP (mp_pi));
  a68_free (A68_MP (mp_half_pi));
  a68_free (A68_MP (mp_two_pi));
  a68_free (A68_MP (mp_sqrt_two_pi));
  a68_free (A68_MP (mp_sqrt_pi));
  a68_free (A68_MP (mp_ln_pi));
  a68_free (A68_MP (mp_180_over_pi));
  a68_free (A68_MP (mp_pi_over_180));
//
  exit (code);
}

//! @brief Main entry point.

int main (int argc, char *argv[])
{
  BYTE_T stack_offset;          // Leave this here!
  A68 (argc) = argc;
  A68 (argv) = argv;
  A68 (close_tty_on_exit) = A68_TRUE;
  FILE_DIAGS_FD (&A68_JOB) = -1;
// Get command name and discard path.
  bufcpy (A68 (a68_cmd_name), argv[0], BUFFER_SIZE);
  for (int k = (int) strlen (A68 (a68_cmd_name)) - 1; k >= 0; k--) {
#if defined (BUILD_WIN32)
    char delim = '\\';
#else
    char delim = '/';
#endif
    if (A68 (a68_cmd_name)[k] == delim) {
      MOVE (&A68 (a68_cmd_name)[0], &A68 (a68_cmd_name)[k + 1], (int) strlen (A68 (a68_cmd_name)) - k + 1);
      k = -1;
    }
  }
// Try to read maximum line width on the terminal,
// used to pretty print diagnostics to same.
  a68_getty (&A68 (term_heigth), &A68 (term_width));
// Determine clock resolution.
  {
    clock_t t0 = clock (), t1;
    do {
      t1 = clock ();
    } while (t1 == t0);
    A68 (clock_res) = (t1 - t0) / (clock_t) CLOCKS_PER_SEC;
  }
// Set the main thread id.
#if defined (BUILD_PARALLEL_CLAUSE)
  A68_PAR (main_thread_id) = pthread_self ();
#endif
  A68 (heap_is_fluid) = A68_TRUE;
  A68 (system_stack_offset) = &stack_offset;
  init_file_entries ();
  if (!setjmp (RENDEZ_VOUS (&A68_JOB))) {
    init_tty ();
// Initialise option handling.
    init_options ();
    SOURCE_SCAN (&A68_JOB) = 1;
    default_options (&A68_JOB);
    default_mem_sizes (1);
// Initialise core.
    A68_STACK = NO_BYTE;
    A68_HEAP = NO_BYTE;
    A68_HANDLES = NO_BYTE;
    get_stack_size ();
// Well, let's start.
    TOP_REFINEMENT (&A68_JOB) = NO_REFINEMENT;
    FILE_INITIAL_NAME (&A68_JOB) = NO_TEXT;
    FILE_GENERIC_NAME (&A68_JOB) = NO_TEXT;
    FILE_SOURCE_NAME (&A68_JOB) = NO_TEXT;
    FILE_LISTING_NAME (&A68_JOB) = NO_TEXT;
    FILE_OBJECT_NAME (&A68_JOB) = NO_TEXT;
    FILE_PLUGIN_NAME (&A68_JOB) = NO_TEXT;
    FILE_BINARY_NAME (&A68_JOB) = NO_TEXT;
    FILE_PRETTY_NAME (&A68_JOB) = NO_TEXT;
    FILE_SCRIPT_NAME (&A68_JOB) = NO_TEXT;
    FILE_DIAGS_NAME (&A68_JOB) = NO_TEXT;
// Options are processed here.
    read_rc_options ();
    read_env_options ();
// Posix copies arguments from the command line.
    if (argc <= 1) {
      online_help (STDOUT_FILENO);
      a68_exit (EXIT_FAILURE);
    }
    for (int k = 1; k < argc; k++) {
      add_option_list (&(OPTION_LIST (&A68_JOB)), argv[k], NO_LINE);
    }
    if (!set_options (OPTION_LIST (&A68_JOB), A68_TRUE)) {
      a68_exit (EXIT_FAILURE);
    }
// State license.
    if (OPTION_LICENSE (&A68_JOB)) {
      state_license (STDOUT_FILENO);
    }
// State version.
    if (OPTION_VERSION (&A68_JOB)) {
      state_version (STDOUT_FILENO);
    }
// Start the UI.
    init_before_tokeniser ();
// Running a script.
#if defined (BUILD_A68_COMPILER)
    if (OPTION_RUN_SCRIPT (&A68_JOB)) {
      load_script ();
    }
#endif
// We translate the program.
    if (FILE_INITIAL_NAME (&A68_JOB) == NO_TEXT || strlen (FILE_INITIAL_NAME (&A68_JOB)) == 0) {
      SCAN_ERROR (!(OPTION_LICENSE (&A68_JOB) || OPTION_VERSION (&A68_JOB)), NO_LINE, NO_TEXT, ERROR_NO_SOURCE_FILE);
    } else {
      compiler_interpreter ();
    }
    a68_exit (ERROR_COUNT (&A68_JOB) == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
    return EXIT_SUCCESS;
  } else {
    diagnostics_to_terminal (TOP_LINE (&A68_JOB), A68_ALL_DIAGNOSTICS);
    a68_exit (EXIT_FAILURE);
    return EXIT_FAILURE;
  }
}
