#pragma once
#ifdef ARDUINO_ARCH_AVR
#define snprintf __glibc_snprintf
#endif
#include_next <stdio.h>