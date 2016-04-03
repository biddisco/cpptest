#include <iostream>

int main(int argc, char **argv)
{
  std::cout << "Hello " << std::endl;
#ifdef __ARM_ARCH_7__
  std::cout << "ARM 7 detected " << std::endl;
#endif
#ifdef __arm__
  std::cout << "ARM ARCH detected " << std::endl;
#endif
}

