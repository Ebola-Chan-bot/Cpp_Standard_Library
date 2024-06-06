#ifdef ARDUINO_ARCH_AVR
#include <Arduino.h>
struct SjLj_Function_Context;
extern "C" void __wrap__Unwind_SjLj_Register (struct SjLj_Function_Context *)
{
  Serial.println("重定向");
  Serial.flush();
}
#endif