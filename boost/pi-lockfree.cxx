#include <atomic>
#include <iostream>
#include <cstring>
//
#include <boost/lockfree/queue.hpp>
#include <boost/lockfree/detail/atomic.hpp>
#include <boost/preprocessor/stringize.hpp>

// simple test that can cause "bus error" on raspberry pi
// due to bad alignment of memory in boost::lockfree

//#define STR(x)   #x
#define SHOW_DEFINE(x) { \
  bool exists = strcmp(BOOST_PP_STRINGIZE(x),#x)!=0; \
  printf("%-40s %20s\t", #x, exists ? "is     defined" : "is NOT defined"); \
  if (exists) { printf("%s",BOOST_PP_STRINGIZE(x)); } \
  printf("\n"); }

#define SHOW_DEFINE_2(x, a, b) { \
  bool exists = strcmp(BOOST_PP_STRINGIZE(x),#x)!=0; \
  printf("%-40s %20s\t", #x, exists ? "is     defined" : "is NOT defined"); \
  if (exists) { printf("%s\t%s",BOOST_PP_STRINGIZE(x), a); } \
  else        { printf("%s",b); } \
  printf("\n"); }


int main()
{
  SHOW_DEFINE(__arm__);
  SHOW_DEFINE(__ARM_ARCH);
  //
  SHOW_DEFINE(__thumb__);
  SHOW_DEFINE(_M_ARM);
  //
  SHOW_DEFINE(__ARM_ARCH_2__);
  SHOW_DEFINE(__ARM_ARCH_3__);
  SHOW_DEFINE(__ARM_ARCH_3M__);   
  SHOW_DEFINE(__ARM_ARCH_4T__);
  SHOW_DEFINE(__TARGET_ARM_4T);
  //
  SHOW_DEFINE(__ARM_ARCH_5__);
  SHOW_DEFINE(__ARM_ARCH_5E__);   
  SHOW_DEFINE(__ARM_ARCH_5T__);
  SHOW_DEFINE(__ARM_ARCH_5TE__);
  SHOW_DEFINE(__ARM_ARCH_5TEJ__);
  //
  SHOW_DEFINE(__ARM_ARCH_6__);
  SHOW_DEFINE(__ARM_ARCH_6J__);
  SHOW_DEFINE(__ARM_ARCH_6K__);
  SHOW_DEFINE(__ARM_ARCH_6Z__);
  SHOW_DEFINE(__ARM_ARCH_6ZK__);
  SHOW_DEFINE(__ARM_ARCH_6T2_);
  //
  SHOW_DEFINE(__ARM_ARCH_7__);
  SHOW_DEFINE(__ARM_ARCH_7A__);
  SHOW_DEFINE(__ARM_ARCH_7R__);
  SHOW_DEFINE(__ARM_ARCH_7M__);
  SHOW_DEFINE(__ARM_ARCH_7S__);
  //
  SHOW_DEFINE(__aarch64__);
  //
  SHOW_DEFINE(_POSIX_VERSION);
  //
  SHOW_DEFINE_2(BOOST_LOCKFREE_NO_HDR_ATOMIC, "(using boost::atomic)", "(using std::atomic)");
  SHOW_DEFINE(BOOST_LOCKFREE_CACHELINE_ALIGNMENT);
  SHOW_DEFINE(BOOST_LOCKFREE_CACHELINE_BYTES);
  //
  std::cout << "Instantiating boost::lockfree::queue<int> \n";
  {
    std::atomic_int intcount;
    boost::lockfree::queue<int> intpool(100);
  }
  return 0;
}
