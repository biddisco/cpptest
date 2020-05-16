#include <iostream>

int test(int x) {
  std::cout << "shared library call : x is " << x << std::endl;
  return x*x;
}


