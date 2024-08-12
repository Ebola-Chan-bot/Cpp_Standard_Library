#include <Cpp_Standard_Library.h>
//演示多个标准库的功能
#include <chrono>
#include <algorithm>
#include <iostream>
#include <memory>
#include <dynarray>
#include <cmath>
#include <unordered_map>
#include <optional>
void Translate(std::chrono::hours H) {
  std::cout << std::chrono::duration_cast<std::chrono::seconds>(H + H).count() << std::endl;
}
std::vector<std::optional<std::move_only_function<void()const>>> FF;
void setup() {
  std::unique_ptr<std::chrono::hours[]> US = std::make_unique_for_overwrite<std::chrono::hours[]>(10);
  std::fill_n(US.get(), 10, std::chrono::hours(1));
  std::chrono::hours H = US[5];
  FF[0] = [H]() {
    Translate(H);
  };
  std::dynarray<int> D(std::log2(1024));
  std::unordered_map<int, std::move_only_function<void()>> UIF;
  UIF[1] = []() {};
#if __cpp_exceptions
  try {
    throw 0;
  } catch (int E) {}
#endif
}
void loop() {
  FF[0].value()();
}