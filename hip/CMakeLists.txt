# on LUMI:
# cmake -DCMAKE_HIP_ARCHITECTURES=gfx90a ~/src/cpptest/

# cxx/host path setup
# cmake -DCMAKE_HIP_ARCHITECTURES=gfx90a -DHIP_CLANG_INCLUDE_PATH=$HIP_PATH/include -DCLANGRT_BUILTINS= ~/src/cpptest/
#

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
# Check working by getting compute capability
#------------------------------------------------------------------------------
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(OUTPUTFILE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/check_hip)
set(HIPFILE ${CMAKE_CURRENT_SOURCE_DIR}/check_hip.cpp)
execute_process(COMMAND ${CMAKE_HIP_COMPILER} ${HIP_NVCC_FLAGS} -lhip ${HIPFILE} -o ${OUTPUTFILE}
    RESULT_VARIABLE HIP_RETURN_CODE
    OUTPUT_VARIABLE ARCH)
if(${HIP_RETURN_CODE} EQUAL 0)
  execute_process(COMMAND ${OUTPUTFILE}
          RESULT_VARIABLE HIP_RETURN_CODE
          OUTPUT_VARIABLE ARCH)
  if(${HIP_RETURN_CODE} EQUAL 0)
    string(REPLACE "-arch=sm_" "" ARCH_NUM "${ARCH}")
    message(STATUS "HIP Architecture: ${ARCH}")
    message(STATUS "HIP Version: ${CMAKE_HIP_COMPILER_VERSION}")
    message(STATUS "HIP Path: ${CMAKE_HIP_COMPILER_LIBRARY_ROOT}")
    message(STATUS "")
    message(STATUS "USE cmake invocation including: cmake -DCMAKE_HIP_ARCHITECTURES=${ARCH_NUM}")
    message(STATUS "")
    set(HIP_NVCC_FLAGS "${ARCH}")
  else()
    message(STATUS "Execute of check_hip failed: ${ARCH}")
  endif()
else()
	message(STATUS "Compile of check_hip failed: ${ARCH}")
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

  add_executable(${test} ${test}.cpp.hip)
  target_link_libraries(${test} hip::device hip::host)
  add_test(NAME ${test} COMMAND ${test} ${_args})
endforeach()
