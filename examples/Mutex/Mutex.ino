#include <Cpp_Standard_Library.h>
#include <mutex>
void setup() {
  Serial.begin(9600);
  std::mutex Mutex;
  std::lock_guard LockGuard{ Mutex };

  Serial.println("已锁定");

  Serial.flush();
  /*
	此调用永远不会返回，也永远不会输出上面写的文字，因为上了锁之后无法中断，串口缓存依赖中断输出，无法中断就永远无法输出，卡死在这里。
	反之，如果注释掉这一行，反而能输出文字，因为此函数结束后会解锁，串口缓存得以输出。
	*/
}
void loop() {}