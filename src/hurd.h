/* Copyright (C) 1993-2024 Free Software Foundation, Inc.
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

#ifndef _HURD_H

#define _HURD_H 1
#include <features.h>

//glibc未定义的hurd
typedef int mach_port_t, file_t, mach_msg_type_number_t, vm_address_t, vm_size_t;
#define MACH_PORT_NULL 0
#define MACH_PORT_RIGHT_SEND 0

enum retry_type
{
	None
};
#include <fcntl.h>
#include <signal.h>
#include <stddef.h>
#include <errno.h>
#include <bits/types/error_t.h>

#ifndef _HURD_H_EXTERN_INLINE
#define _HURD_H_EXTERN_INLINE __extern_inline
#endif

extern int __hurd_fail(error_t err) __COLD;

#ifdef __USE_EXTERN_INLINES
_HURD_H_EXTERN_INLINE int
__hurd_fail(error_t err)
{
	switch (err)
	{
	case EMACH_SEND_INVALID_DEST:
	case EMIG_SERVER_DIED:
		/* The server has disappeared!  */
		err = (error_t)EIEIO;
		break;

	case KERN_NO_SPACE:
		err = (error_t)ENOMEM;
		break;

	case KERN_INVALID_ADDRESS:
	case KERN_INVALID_ARGUMENT:
		err = (error_t)EINVAL;
		break;

	case 0:
		return 0;

	default:
		break;
	}

	errno = err;
	return -1;
}
#endif

/* Basic ports and info, initialized by startup.  */

extern int _hurd_exec_flags; /* Flags word passed in exec_startup.  */
extern struct hurd_port *_hurd_ports;
extern unsigned int _hurd_nports;
extern mode_t _hurd_umask;
extern sigset_t _hurdsig_traced;

/* Shorthand macro for internal library code referencing _hurd_ports (see
   <hurd/port.h>).  */
/* Also see __USEPORT_CANCEL.  */

#define __USEPORT(which, expr) \
	HURD_PORT_USE(&_hurd_ports[INIT_PORT_##which], (expr))

/* Function version of __USEPORT: calls OPERATE with a send right.  */

extern error_t _hurd_ports_use(int which, error_t (*operate)(mach_port_t));

/* Initial file descriptor table we were passed at startup.  If we are
   using a real dtable, these are turned into that and then cleared at
   startup.  If not, these are never changed after startup.  Not locked.  */

extern mach_port_t *_hurd_init_dtable;

/* Current process IDs.  */

extern pid_t _hurd_pid, _hurd_ppid, _hurd_pgrp;
extern int _hurd_orphaned;

/* This variable is incremented every time the process IDs change.  */
extern unsigned int _hurd_pids_changed_stamp;

/* Unix `data break', for brk and sbrk.
   If brk and sbrk are not used, this info will not be initialized or used.  */

/* This mutex locks _hurd_brk and _hurd_data_end.  */

extern struct mutex _hurd_brk_lock;

#include <bits/types/FILE.h>

/* Calls to get and set basic ports.  */

extern error_t _hurd_ports_get(unsigned int which, mach_port_t *result);
extern error_t _hurd_ports_set(unsigned int which, mach_port_t newport);

extern mach_port_t getcttyid(void);
extern int setcttyid(mach_port_t);

/* Does reauth with the proc server and fd io servers.  */
extern int __setauth(auth_t), setauth(auth_t);

/* Modify a port cell by looking up a directory name.
   This verifies that it is a directory and that we have search permission.  */
extern int _hurd_change_directory_port_from_name(struct hurd_port *portcell,
												 const char *name);
/* Same thing, but using an open file descriptor.
   Also verifies that it is a directory and that we have search permission.  */
extern int _hurd_change_directory_port_from_fd(struct hurd_port *portcell,
											   int fd);

/* Inform the proc server we have exited with STATUS, and kill the
   task thoroughly.  This function never returns, no matter what.  */

extern void _hurd_exit(int status) __attribute__((noreturn));

/* Initialize the library data structures from the
   ints and ports passed to us by the exec server.
   Then vm_deallocate PORTARRAY and INTARRAY.  */

extern void _hurd_init(int flags, char **argv,
					   mach_port_t *portarray, size_t portarraysize,
					   int *intarray, size_t intarraysize);

/* Register the process to the proc server.  */
extern void _hurd_libc_proc_init(char **argv);

/* Do startup handshaking with the proc server, and initialize library data
   structures that require proc server interaction.  This includes
   initializing signals; see _hurdsig_init in <hurd/signal.h>.  */

extern void _hurd_proc_init(char **argv,
							const int *intarray, size_t intarraysize);

/* Send a `sig_post' RPC to process number PID.  If PID is zero,
   send the message to all processes in the current process's process group.
   If PID is < -1, send SIG to all processes in process group - PID.
   SIG and REFPORT are passed along in the request message.  */

extern error_t _hurd_sig_post(pid_t pid, int sig, mach_port_t refport);
extern error_t hurd_sig_post(pid_t pid, int sig, mach_port_t refport);

/* Test and clear the calling thread's "cancelled" flag.  */
extern int hurd_check_cancel(void);

#define __need___va_list
#include <stdarg.h>

#endif /* hurd.h */