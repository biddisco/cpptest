# on LUMI:
# cmake -DCMAKE_HIP_ARCHITECTURES=gfx90a -DCLANGRT_BUILTINS= ~/src/cpptest/

# cxx/host path setup
# cmake -DCMAKE_HIP_ARCHITECTURES=gfx90a -DHIP_CLANG_INCLUDE_PATH=$HIP_PATH/include -DCLANGRT_BUILTINS= ~/src/cpptest/
#

message(STATUS "-------------------------")

include(CheckLanguage)
check_language(HIP)

if(CMAKE_HIP_COMPILER)
  message(STATUS "HIP FOUND")
  enable_language(HIP)
else()
  message(STATUS "No HIP support")
  return()
endif()

find_package(hip)
if (!hip_FOUND)
  message(STATUS "No HIP support")
  return()
endif()

#------------------------------------------------------------------------------
# HIP tests
#------------------------------------------------------------------------------
set(HIP_TEST_SRCS
    hello-hip
)

#------------------------------------------------------------------------------
# create exes for tests
#------------------------------------------------------------------------------
foreach(test ${HIP_TEST_SRCS})
  set(_args)

  set_source_files_properties(${test}.cu PROPERTIES LANGUAGE HIP)  
  add_executable(${test} ${test}.cu)
  target_link_libraries(${test} hip::device)

  add_test(NAME ${test} COMMAND ${test} ${_args})
endforeach()
