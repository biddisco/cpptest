#include <iostream>

extern int test(int);

int main(int argc, char **argv)
{
  std::cout << "Hello c++" << std::endl;
  int x = 5;
  std::cout
      << "Calling shared lib with " << x << std::endl;
  int ret = test(x);
  std::cout << "Returned " << ret << std::endl;

#ifdef __ARM_ARCH_7__
  std::cout << "ARM 7 detected " << std::endl;
#endif
#ifdef __arm__
  std::cout << "ARM ARCH detected " << std::endl;
#endif
}

