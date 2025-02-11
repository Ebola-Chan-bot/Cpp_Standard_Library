#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
void ContainsTest()
{
  std::map<char,char>{}.contains(0);
  std::set<char>{}.contains(0);
  std::unordered_map<char,char>{}.contains(0);
  std::unordered_set<char>{}.contains(0);
}