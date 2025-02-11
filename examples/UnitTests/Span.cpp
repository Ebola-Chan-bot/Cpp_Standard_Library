#include <chrono>//潜在的冲突可能性
#include <span>
void SpanTest(char C) {
  std::span<char>{&C, 1};
}