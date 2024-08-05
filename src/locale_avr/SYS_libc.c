#ifdef ARDUINO_ARCH_AVR
/* Define a constant for the dgettext domainname for libc internal messages,
   so the string constant is not repeated in dozens of object files.  */

#include <libintl.h>
#define const const __attribute__((__progmem__))
const char _libc_intl_domainname[] = "libc";
libc_hidden_data_def (_libc_intl_domainname)
#endif