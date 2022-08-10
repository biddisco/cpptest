cmake_policy(SET CMP0104 NEW) # Set CMAKE_CUDA_ARCHITECTURES for NVidia compiler

#------------------------------------------------------------------------------
# CUDA Language support
#------------------------------------------------------------------------------
include(CheckLanguage)
check_language(CUDA)

if(CMAKE_CUDA_COMPILER)
    enable_language(CUDA)
else()
    message(STATUS "No CUDA support")
    return()
endif()

#------------------------------------------------------------------------------
# Check working by getting CUDA compute capability
#------------------------------------------------------------------------------
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(OUTPUTFILE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/check_cuda)
set(CUDAFILE ${CMAKE_CURRENT_SOURCE_DIR}/check_cuda.cu)
execute_process(COMMAND ${CMAKE_CUDA_COMPILER} -lcuda ${CUDAFILE} -o ${OUTPUTFILE}
                RESULT_VARIABLE CUDA_RETURN_CODE
                OUTPUT_VARIABLE ARCH)
if(${CUDA_RETURN_CODE} EQUAL 0)
    execute_process(COMMAND ${OUTPUTFILE}
                    RESULT_VARIABLE CUDA_RETURN_CODE
                    OUTPUT_VARIABLE ARCH)
    if(${CUDA_RETURN_CODE} EQUAL 0)
        string(REPLACE "-arch=sm_" "" ARCH_NUM "${ARCH}")
        message(STATUS "CUDA Architecture: ${ARCH}")
        message(STATUS "CUDA Version: ${CMAKE_CUDA_COMPILER_VERSION}")
        message(STATUS "CUDA Path: ${CMAKE_CUDA_COMPILER_LIBRARY_ROOT}")
        message(STATUS "")
        message(STATUS "USE cmake invocation including: cmake -DCMAKE_CUDA_ARCHITECTURES=${ARCH_NUM}")
        message(STATUS "")
        set(CUDA_NVCC_FLAGS "${ARCH}")
    else()
        message(WARNING "Execute of check_cuda failed: ${ARCH}")
    endif()
else()
    message(WARNING "Compile of check_cuda failed: ${ARCH}")
endif()


#------------------------------------------------------------------------------
# Check Find CudaToolkit works ok
#------------------------------------------------------------------------------
include(FindCUDAToolkit)
find_package(CUDAToolkit)

if (CUDAToolkit_FOUND)
    message(STATUS "CUDAToolkit found")
else()
    message(WARNING "CUDAToolkit NOT found")
endif()

#------------------------------------------------------------------------------
# CUDA tests
#------------------------------------------------------------------------------
set(CUDA_TEST_SRCS
    hello-cuda
    transpose
)

#------------------------------------------------------------------------------
# create exes for tests
#------------------------------------------------------------------------------
foreach(test ${CUDA_TEST_SRCS})
  set(_args)
  add_executable(${test} ${test}.cu)
  target_include_directories(${test} PRIVATE ./Common)
  add_test(NAME ${test} COMMAND ${test} ${_args})
endforeach()
