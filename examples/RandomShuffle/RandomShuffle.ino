//此示例需要使用串口监视器呈现。先生成一个原始数组0123456789，然后根据用户输入的随机种子，不断产生随机排序。
#include <Cpp_Standard_Library.h>
#include <iostream>
#include <random>
#include <algorithm>
template<typename T>
struct MakeArray {};
template<size_t... V>
struct MakeArray< std::integer_sequence<size_t, V...>> {
  static size_t value[sizeof...(V)];
};
template<size_t... V>
size_t MakeArray<std::integer_sequence<size_t, V...>>::value[sizeof...(V)] = { V... };
size_t (&Array)[10] = MakeArray<std::make_index_sequence<10>>::value;
void setup() {
  Serial.begin(9600);
  std::cout << "原始数组：";
  for (size_t A : Array)
    std::cout << A;
  std::cout << std::endl;
}
void loop() {
#ifdef ARDUINO_ARCH_ESP32
  //ESP32架构推荐使用硬件随机生成器，不需要设置种子即可得到真随机数
  constexpr std::EspUrng Urng;
#else
  //非标准行为：ArduinoUrng是Arduino平台专用的随机生成器。你也可以使用标准库提供的mt19937，但它占用了太多内存，不建议使用。
  constexpr std::ArduinoUrng Urng;
  static uint32_t RandomSeed;
  std::cout << "输入随机种子（非数字的输入将视为0）：";

  std::cin >> RandomSeed;

  // 清除错误标志位。必须紧贴ignore之前，因为不先清除错误状态ignore就不会生效。
  std::cin.clear();

  // 丢弃错误输入。必须在获取RandomSeed之后，否则第一次输入会被忽略
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  std::cout << RandomSeed << std::endl;
  std::ArduinoUrng::seed(RandomSeed);
#endif
  std::shuffle(std::begin(Array), std::end(Array), Urng);
  std::cout << "随机乱序：";
  for (size_t A : Array)
    std::cout << A;
  std::cout << std::endl;
#ifdef __EXCEPTIONS
  //此段仅用于展示异常处理，对本示例主线逻辑无意义。必须在编译选项中启用-fexceptions并禁用-fno-rtti才能使本段生效。AVR不支持异常处理。
  try {
    throw 0;
  } catch (uint32_t RS) {
    std::cout << "【异常处理】随机种子：" << RS << std::endl;
  }
#endif
}