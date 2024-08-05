#ifdef ARDUINO_ARCH_AVR
/* libio vtable validation.
   Copyright (C) 2016-2024 Free Software Foundation, Inc.
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

#include <libio/libioP.h>
#include <stdio.h>
#include <array_length.h>
#define const const __attribute__((__progmem__))
/* Both _IO_str_* and _IO_new_file functions are pulled into every link (from
   stdio initialization).  */
#ifndef SHARED
/* NB: the following directives do add pragma weak for _IO_default _* and
   _IO_wdefault_* symbols to potentially avoid link failures, since they
   are always used when the __io_vtables is used.  */
#pragma weak _IO_wstr_finish
#pragma weak _IO_wstr_overflow
#pragma weak _IO_wstr_pbackfail
#pragma weak _IO_wstr_seekoff
#pragma weak _IO_wstr_underflow

#pragma weak _IO_file_close
#pragma weak _IO_file_close_mmap
#pragma weak _IO_file_doallocate
#pragma weak _IO_file_finish
#pragma weak _IO_file_overflow
#pragma weak _IO_file_read
#pragma weak _IO_file_seek
#pragma weak _IO_file_seekoff_maybe_mmap
#pragma weak _IO_file_seekoff_mmap
#pragma weak _IO_file_setbuf
#pragma weak _IO_file_setbuf_mmap
#pragma weak _IO_file_setbuf_mmap
#pragma weak _IO_file_stat
#pragma weak _IO_file_sync
#pragma weak _IO_file_sync_mmap
#pragma weak _IO_file_underflow
#pragma weak _IO_file_underflow_maybe_mmap
#pragma weak _IO_file_underflow_mmap
#pragma weak _IO_file_xsgetn
#pragma weak _IO_file_xsgetn_maybe_mmap
#pragma weak _IO_file_xsgetn_mmap
#pragma weak _IO_file_xsputn

#pragma weak _IO_wfile_overflow
#pragma weak _IO_wfile_sync
#pragma weak _IO_wfile_underflow
#pragma weak _IO_wfile_underflow_maybe_mmap
#pragma weak _IO_wfile_underflow_mmap
#pragma weak _IO_wfile_doallocate
#pragma weak _IO_wfile_seekoff
#pragma weak _IO_wfile_xsputn

#pragma weak _IO_new_proc_close

#pragma weak _IO_cookie_close
#pragma weak _IO_cookie_read
#pragma weak _IO_cookie_seek
#pragma weak _IO_cookie_seekoff
#pragma weak _IO_cookie_write

#pragma weak _IO_mem_finish
#pragma weak _IO_mem_sync

#pragma weak _IO_wmem_finish
#pragma weak _IO_wmem_sync

#pragma weak __printf_buffer_as_file_overflow
#pragma weak __printf_buffer_as_file_xsputn

#pragma weak __wprintf_buffer_as_file_overflow
#pragma weak __wprintf_buffer_as_file_xsputn
#endif

const struct _IO_jump_t __io_vtables[] =
    {
        [IO_WPRINTF_BUFFER_AS_FILE_JUMPS] = {},
#if SHLIB_COMPAT(libc, GLIBC_2_0, GLIBC_2_1)
        /* _IO_old_file_jumps  */
        [IO_OLD_FILE_JUMPS] = {
            JUMP_INIT_DUMMY,
            JUMP_INIT(finish, _IO_old_file_finish),
            JUMP_INIT(overflow, _IO_old_file_overflow),
            JUMP_INIT(underflow, _IO_old_file_underflow),
            JUMP_INIT(uflow, _IO_default_uflow),
            JUMP_INIT(pbackfail, _IO_default_pbackfail),
            JUMP_INIT(xsputn, _IO_old_file_xsputn),
            JUMP_INIT(xsgetn, _IO_default_xsgetn),
            JUMP_INIT(seekoff, _IO_old_file_seekoff),
            JUMP_INIT(seekpos, _IO_default_seekpos),
            JUMP_INIT(setbuf, _IO_old_file_setbuf),
            JUMP_INIT(sync, _IO_old_file_sync),
            JUMP_INIT(doallocate, _IO_file_doallocate),
            JUMP_INIT(read, _IO_file_read),
            JUMP_INIT(write, _IO_old_file_write),
            JUMP_INIT(seek, _IO_file_seek),
            JUMP_INIT(close, _IO_file_close),
            JUMP_INIT(stat, _IO_file_stat)},
        /*  _IO_old_proc_jumps  */
        [IO_OLD_PROC_JUMPS] = {JUMP_INIT_DUMMY, JUMP_INIT(finish, _IO_old_file_finish), JUMP_INIT(overflow, _IO_old_file_overflow), JUMP_INIT(underflow, _IO_old_file_underflow), JUMP_INIT(uflow, _IO_default_uflow), JUMP_INIT(pbackfail, _IO_default_pbackfail), JUMP_INIT(xsputn, _IO_old_file_xsputn), JUMP_INIT(xsgetn, _IO_default_xsgetn), JUMP_INIT(seekoff, _IO_old_file_seekoff), JUMP_INIT(seekpos, _IO_default_seekpos), JUMP_INIT(setbuf, _IO_old_file_setbuf), JUMP_INIT(sync, _IO_old_file_sync), JUMP_INIT(doallocate, _IO_file_doallocate), JUMP_INIT(read, _IO_file_read), JUMP_INIT(write, _IO_old_file_write), JUMP_INIT(seek, _IO_file_seek), JUMP_INIT(close, _IO_old_proc_close), JUMP_INIT(stat, _IO_file_stat), JUMP_INIT(showmanyc, _IO_default_showmanyc), JUMP_INIT(imbue, _IO_default_imbue)},
#endif

#if SHLIB_COMPAT(libc, GLIBC_2_0, GLIBC_2_2)
        /* _IO_old_cookie_jumps  */
        [IO_OLD_COOKIED_JUMPS] = {
            JUMP_INIT_DUMMY,
            JUMP_INIT(finish, _IO_file_finish),
            JUMP_INIT(overflow, _IO_file_overflow),
            JUMP_INIT(underflow, _IO_file_underflow),
            JUMP_INIT(uflow, _IO_default_uflow),
            JUMP_INIT(pbackfail, _IO_default_pbackfail),
            JUMP_INIT(xsputn, _IO_file_xsputn),
            JUMP_INIT(xsgetn, _IO_default_xsgetn),
            JUMP_INIT(seekoff, _IO_cookie_seekoff),
            JUMP_INIT(seekpos, _IO_default_seekpos),
            JUMP_INIT(setbuf, _IO_file_setbuf),
            JUMP_INIT(sync, _IO_file_sync),
            JUMP_INIT(doallocate, _IO_file_doallocate),
            JUMP_INIT(read, _IO_cookie_read),
            JUMP_INIT(write, _IO_cookie_write),
            JUMP_INIT(seek, _IO_old_cookie_seek),
            JUMP_INIT(close, _IO_cookie_close),
            JUMP_INIT(stat, _IO_default_stat),
            JUMP_INIT(showmanyc, _IO_default_showmanyc),
            JUMP_INIT(imbue, _IO_default_imbue),
        },
#endif
};
_Static_assert(array_length(__io_vtables) == IO_VTABLES_NUM,
               "initializer count");

#ifdef SHARED

void (*IO_accept_foreign_vtables)(void) attribute_hidden;

#else /* !SHARED */

/* Used to check whether static dlopen support is needed.  */
#pragma weak __dlopen

#endif

void attribute_hidden
_IO_vtable_check(void)
{
  abort();
}

#define STR(s) XSTR(s)
#define XSTR(s) #s
#endif