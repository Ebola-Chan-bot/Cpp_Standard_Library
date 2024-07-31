# include <dl-dtv.h>
//32
/* Type of the TCB.  */
typedef struct
{
  void *tcb;			/* Points to this structure.  */
  dtv_t *dtv;			/* Vector of pointers to TLS data.  */
  thread_t self_do_no_use;	/* This thread's control port.  */
  int __glibc_padding1;
  int multiple_threads;
  int gscope_flag;
  uintptr_t sysinfo;
  uintptr_t stack_guard;
  uintptr_t pointer_guard;
  long __glibc_padding2[2];
  int private_futex;
  int __glibc_padding3;
  /* Reservation of some values for the TM ABI.  */
  void *__private_tm[4];
  /* GCC split stack support.  */
  void *__private_ss;
  /* The lowest address of shadow stack.  */
  unsigned long long int ssp_base;

  /* Keep these fields last, so offsets of fields above can continue being
     compatible with the x86_64 NPTL version.  */
  mach_port_t reply_port;      /* This thread's reply port.  */
  struct hurd_sigstate *_hurd_sigstate;

  /* Used by the exception handling implementation in the dynamic loader.  */
  struct rtld_catch *rtld_catch;
} tcbhead_t;

/* GCC generates %fs:0x28 to access the stack guard.  */
_Static_assert (offsetof (tcbhead_t, stack_guard) == 0x28,
                "stack guard offset");
/* libgcc uses %fs:0x70 to access the split stack pointer.  */
_Static_assert (offsetof (tcbhead_t, __private_ss) == 0x70,
                "split stack pointer offset");

/* The TCB can have any size and the memory following the address the
   thread pointer points to is unspecified.  Allocate the TCB there.  */
# define TLS_TCB_AT_TP	1
# define TLS_DTV_AT_TP	0

/* Alignment requirement for TCB.

   Some processors such as Intel Atom pay a big penalty on every
   access using a segment override if that segment's base is not
   aligned to the size of a cache line.  (See Intel 64 and IA-32
   Architectures Optimization Reference Manual, section 13.3.3.3,
   "Segment Base".)  On such machines, a cache line is 64 bytes.  */
# define TCB_ALIGNMENT	64


# define THREAD_SELF							\
  (*(tcbhead_t * __seg_fs *) offsetof (tcbhead_t, tcb))
/* Read member of the thread descriptor directly.  */
# define THREAD_GETMEM(descr, member)					\
  (*(__typeof (descr->member) __seg_fs *) offsetof (tcbhead_t, member))
/* Write member of the thread descriptor directly.  */
# define THREAD_SETMEM(descr, member, value)				\
  (*(__typeof (descr->member) __seg_fs *) offsetof (tcbhead_t, member) = value)
  //94