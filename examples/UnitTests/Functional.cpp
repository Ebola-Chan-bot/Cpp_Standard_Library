#include <functional>
void Functional() {
#ifdef ARDUINO_ARCH_SAM
  std::move_only_function<void() const> const Member{ []() {} };
#else
  std::move_only_function<void() const> const Member = []() {};  //SAM编译器的bug，不允许这样初始化
#endif
}