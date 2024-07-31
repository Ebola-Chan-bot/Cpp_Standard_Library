#ifdef ARDUINO_ARCH_AVR
/* Initialization code for TLS in statically linked application.
   Copyright (C) 2002-2024 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

#include <errno.h>
#include <ldsodefs.h>
#include <tls.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/param.h>
#include <array_length.h>

#ifdef SHARED
 #error makefile bug, this file is for static only
#endif

dtv_t _dl_static_dtv[2 + TLS_SLOTINFO_SURPLUS];


static struct dtv_slotinfo_list static_slotinfo =
  {
   /* Allocate an array of 2 + TLS_SLOTINFO_SURPLUS elements.  */
   .slotinfo =  { [array_length (_dl_static_dtv) - 1] = { 0 } },
  };

/* Highest dtv index currently needed.  */
size_t _dl_tls_max_dtv_idx;
/* Flag signalling whether there are gaps in the module ID allocation.  */
bool _dl_tls_dtv_gaps;
/* Information about the dtv slots.  */
struct dtv_slotinfo_list *_dl_tls_dtv_slotinfo_list;
/* Number of modules in the static TLS block.  */
size_t _dl_tls_static_nelem;
/* Size of the static TLS block.  */
size_t _dl_tls_static_size;
/* Size actually allocated in the static TLS block.  */
size_t _dl_tls_static_used;
/* Alignment requirement of the static TLS block.  */
size_t _dl_tls_static_align;
/* Size of surplus space in the static TLS area for dynamically
   loaded modules with IE-model TLS or for TLSDESC optimization.
   See comments in elf/dl-tls.c where it is initialized.  */
size_t _dl_tls_static_surplus;
/* Remaining amount of static TLS that may be used for optimizing
   dynamic TLS access (e.g. with TLSDESC).  */
size_t _dl_tls_static_optional;

/* Generation counter for the dtv.  */
size_t _dl_tls_generation;


/* Additional definitions needed by TLS initialization.  */
#ifdef TLS_INIT_HELPER
TLS_INIT_HELPER
#endif

static void
init_slotinfo (void)
{
  /* Create the slotinfo list.  Note that the type of static_slotinfo
     has effectively a zero-length array, so we cannot use the size of
     static_slotinfo to determine the array length.  */
  static_slotinfo.len = array_length (_dl_static_dtv);
  /* static_slotinfo.next = NULL; -- Already zero.  */

  /* The slotinfo list.  Will be extended by the code doing dynamic
     linking.  */
  GL(dl_tls_max_dtv_idx) = 1;
  GL(dl_tls_dtv_slotinfo_list) = &static_slotinfo;
}
#endif