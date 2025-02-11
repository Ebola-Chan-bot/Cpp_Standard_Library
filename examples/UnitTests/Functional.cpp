#include <functional>
void Functional() {
  std::move_only_function<void() const>{}();
}