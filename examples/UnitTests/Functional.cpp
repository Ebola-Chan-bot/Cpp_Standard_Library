
#include <functional>
struct Type
{
  Type(Type&&){}
  Type(int){}
  Type&operator=(int){}
};
struct AsMember
{
  Type MemberB=0;
  //std::move_only_function<void() const> const Member = []() {};
};
void Functional()
{
  std::move_only_function<void() const>{}();
  AsMember AM;
}
