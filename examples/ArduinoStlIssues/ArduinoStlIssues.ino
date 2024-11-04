// 测试ArduinoSTL中提出的问题
#include <Cpp_Standard_Library.h>
#include <numeric>
#include <array>
#include <iostream>
ARDUINO_ARCH
void setup() {
  // Issue28
  std::array<float, 5> avgTemp = { 23.4, 24.5, 19.9, 25.6, 27.8 };
  float average = std::accumulate(avgTemp.begin(), avgTemp.end(), 0.0) / avgTemp.size();
  // Issue43
  auto str = F("\r\nSystem halted!");
  std::cout << str << std::endl;
}
void loop() {}
//Issue78
class LambdaTest {
public:

  LambdaTest(std::function<void(int n)> lambda) {
    lambda_ptr = &lambda;
  }

  void callLambda(int value) {
    (*lambda_ptr)(value);
  }

protected:
  std::function<void(int n)> *lambda_ptr;
};