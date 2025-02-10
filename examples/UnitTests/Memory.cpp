#include <memory>
std::unique_ptr<char[]> UPI = std::make_unique_for_overwrite<char[]>(1);