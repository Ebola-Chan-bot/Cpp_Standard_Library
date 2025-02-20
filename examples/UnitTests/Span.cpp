
#include <span>
void SpanTest(char C) {
  std::span<char>{&C, 1};
}
