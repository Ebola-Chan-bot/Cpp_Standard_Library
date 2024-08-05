#ifdef ARDUINO_ARCH_AVR
/* Define a constant for the name of the standard C locale, so the string
   constant is not repeated in dozens of object files.  */

#include "localeinfo.h"
#define const const __attribute__((__progmem__))
/* Name of our standard locale.  */
const char _nl_C_name[] = "C";
const char _nl_POSIX_name[] = "POSIX";

/* The standard codeset.  */
const char _nl_C_codeset[] = "ANSI_X3.4-1968";
#endif