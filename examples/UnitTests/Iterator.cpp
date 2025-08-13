#include <iterator>
struct Module;
struct Sequential {
  Module* const Array[] = { nullptr };
  Module* const* It = std::cend(Array);
};
void Iterator
{
  Sequential S;
};