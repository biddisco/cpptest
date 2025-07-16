/* MPI_Allgather bug with cray-mpich 8.1.32 + libfabric 1.22 and cray-mpich 8.1.30 + libfabric 1.15.2
 *
 * This code calls MPI_Allgatherv a single time and compares results against an expected reference.
 * on ALPS (CRAY-EX Blanca Peak), this test segfaults for 40 or more ranks,
 * while on LUMI (cray-mpich 8.1.29, libfabric 1.15.2) it passes for all numRanks <= 1024.
 * 
 * With OpenMPI and libfabric 2.0 on ALPS, the test also passes for all numRanks <= 1024
 *
 * compile:
 * uenv image find
 * uenv image pull prgenv-gnu/25.06:rc5
 * uenv start prgenv-gnu/25.06 --view=default
 * mpic++ -I/user-environment/env/default/include/ -L/user-environment/env/default/lib64 allgather-bug.cpp -lcudart -lcuda
 * 
 * run:
 * srun -t05:00 -N8 --ntasks-per-node=4 -c1 ~/select-gpu.sh ./a.out     // PASS
 * srun -t05:00 -N4 --ntasks-per-node=8 -c1 ~/select-gpu.sh ./a.out     // PASS
 * srun -t05:00 -N5 --ntasks-per-node=8 -c1 ~/select-gpu.sh ./a.out     // FAIL
 * srun -t05:00 -N8 --ntasks-per-node=8 -c1 ~/select-gpu.sh ./a.out     // FAIL
 * srun -t05:00 -N32 --ntasks-per-node=32 -c1 ~/select-gpu.sh ./a.out   // FAIL
 *
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>

#include <mpi.h>

#if defined(__HIP_PLATFORM_AMD__) || defined(__HIPCC__)

#include <hip/hip_runtime.h>

#define cudaDeviceProp hipDeviceProp_t
#define cudaDeviceSynchronize hipDeviceSynchronize
#define cudaErrorInvalidValue hipErrorInvalidValue
#define cudaError_t hipError_t
#define cudaEventCreate hipEventCreate
#define cudaEventDestroy hipEventDestroy
#define cudaEventElapsedTime hipEventElapsedTime
#define cudaEventRecord hipEventRecord
#define cudaEventSynchronize hipEventSynchronize
#define cudaEvent_t hipEvent_t
#define cudaFree hipFree
#define cudaFreeHost hipFreeHost
#define cudaGetDevice hipGetDevice
#define cudaGetDeviceCount hipGetDeviceCount
#define cudaGetDeviceProperties hipGetDeviceProperties
#define cudaGetErrorName hipGetErrorName
#define cudaGetErrorString hipGetErrorString
#define cudaGetLastError hipGetLastError
#define cudaMalloc hipMalloc
#define cudaMallocHost hipMallocHost
#define cudaMallocManaged hipMallocManaged
#define cudaMemAttachGlobal hipMemAttachGlobal
#define cudaMemcpy hipMemcpy
#define cudaMemcpyDeviceToDevice hipMemcpyDeviceToDevice
#define cudaMemcpyDeviceToHost hipMemcpyDeviceToHost
#define cudaMemcpyFromSymbol hipMemcpyFromSymbol
#define cudaMemcpyHostToDevice hipMemcpyHostToDevice
#define cudaMemcpyToSymbol hipMemcpyToSymbol
#define cudaMemoryTypeDevice hipMemoryTypeDevice
#define cudaMemoryTypeManaged hipMemoryTypeManaged
#define cudaMemset hipMemset
#define cudaPointerAttributes hipPointerAttribute_t
#define cudaPointerGetAttributes hipPointerGetAttributes
#define cudaSetDevice hipSetDevice
#define cudaStreamCreate hipStreamCreate
#define cudaStreamDestroy hipStreamDestroy
#define cudaStreamSynchronize hipStreamSynchronize
#define cudaStream_t hipStream_t
#define cudaSuccess hipSuccess

#define GPU_SYMBOL HIP_SYMBOL

#else

#include <cuda_runtime.h>

#define GPU_SYMBOL(x) x

#endif

int main(int argc, char **argv) {

      // size = 1024, number of elements per rank in MPI_Allgatherv
  std::vector<int> counts{
      331, 186, 166, 203, 190, 273, 211, 165, 152, 169, 146, 143, 149, 210, 203,
      204, 191, 158, 131, 117, 133, 150, 139, 141, 166, 187, 256, 251, 212, 226,
      188, 172, 137, 145, 158, 173, 229, 236, 191, 166, 152, 164, 146, 140, 163,
      236, 172, 210, 188, 146, 145, 123, 117, 564, 595, 538, 567, 635, 425, 257,
      126, 163, 276, 663, 596, 564, 465, 577, 629, 527, 369, 242, 283, 265, 264,
      331, 414, 492, 503, 369, 327, 430, 577, 522, 595, 611, 404, 655, 328, 501,
      124, 178, 351, 165, 129, 145, 133, 127, 397, 293, 422, 131, 154, 136, 159,
      167, 168, 163, 143, 156, 155, 128, 116, 129, 144, 155, 168, 218, 206, 239,
      239, 208, 222, 242, 198, 167, 202, 232, 296, 190, 203, 166, 190, 261, 187,
      165, 142, 139, 150, 133, 117, 131, 158, 191, 204, 203, 210, 149, 142, 147,
      169, 152, 165, 211, 273, 190, 203, 166, 186, 323, 268, 185, 175, 200, 217,
      226, 207, 240, 239, 206, 218, 168, 155, 144, 129, 116, 128, 155, 155, 144,
      163, 167, 168, 159, 136, 154, 131, 421, 294, 397, 127, 133, 145, 129, 165,
      350, 179, 124, 499, 329, 655, 405, 610, 595, 523, 577, 430, 327, 369, 502,
      492, 414, 332, 263, 266, 283, 242, 368, 527, 629, 578, 465, 563, 596, 663,
      277, 163, 125, 257, 426, 634, 567, 538, 596, 563, 129, 129, 138, 128, 150,
      148, 157, 147, 165, 178, 151, 132, 152, 173, 182, 265, 217, 212, 204, 205,
      175, 176, 205, 204, 212, 217, 265, 182, 173, 152, 132, 151, 178, 165, 147,
      157, 148, 150, 128, 138, 129, 129, 563, 596, 537, 568, 634, 425, 258, 125,
      163, 276, 663, 597, 563, 465, 577, 629, 528, 368, 242, 283, 266, 263, 331,
      414, 493, 502, 369, 327, 430, 577, 523, 594, 611, 404, 656, 328, 500, 124,
      179, 350, 165, 129, 145, 133, 127, 397, 293, 422, 131, 154, 136, 159, 168,
      167, 163, 144, 155, 155, 128, 116, 129, 144, 155, 168, 218, 206, 239, 239,
      208, 226, 217, 200, 175, 185, 271, 327, 186, 166, 203, 190, 273, 211, 165,
      152, 169, 147, 142, 149, 210, 203, 204, 191, 158, 131, 117, 133, 150, 139,
      142, 165, 187, 261, 190, 166, 203, 190, 301, 236, 169, 177, 258, 195, 312,
      185, 268, 201, 165, 231, 208, 209, 174, 187, 204, 212, 251, 188, 156, 162,
      157, 149, 146, 134, 144, 118, 138, 125, 151, 148, 155, 166, 127, 211, 646,
      279, 636, 533, 656, 534, 625, 365, 379, 563, 128, 168, 136, 141, 164, 133,
      126, 128, 566, 379, 166, 429, 483, 624, 525, 619, 623, 471, 514, 462, 398,
      316, 205, 214, 349, 309, 357, 452, 310, 463, 582, 641, 621, 384, 545, 383,
      129, 142, 132, 127, 142, 161, 158, 185, 248, 172, 179, 226, 189, 261, 176,
      176, 150, 144, 162, 147, 148, 186, 195, 179, 230, 321, 186, 247, 199, 175,
      163, 150, 151, 162, 135, 124, 141, 143, 118, 130, 145, 165, 162, 193, 253,
      167, 194, 195, 167, 250, 196, 162, 165, 145, 130, 118, 143, 140, 125, 135,
      161, 152, 150, 163, 175, 199, 247, 186, 321, 229, 180, 195, 186, 147, 148,
      162, 144, 149, 177, 176, 261, 188, 227, 179, 172, 248, 185, 158, 161, 142,
      127, 131, 143, 129, 382, 545, 384, 621, 641, 582, 464, 310, 452, 356, 310,
      349, 214, 205, 316, 398, 462, 514, 471, 623, 618, 526, 624, 483, 429, 166,
      379, 566, 128, 126, 133, 164, 141, 136, 168, 128, 562, 380, 364, 625, 535,
      656, 533, 636, 279, 646, 211, 127, 165, 156, 148, 151, 125, 138, 118, 144,
      134, 145, 150, 157, 162, 156, 188, 251, 212, 204, 186, 175, 208, 209, 231,
      165, 201, 268, 185, 312, 195, 258, 177, 169, 236, 301, 190, 203, 166, 189,
      261, 187, 166, 142, 139, 150, 133, 117, 131, 158, 191, 204, 203, 210, 149,
      142, 147, 169, 152, 165, 211, 273, 189, 204, 165, 187, 323, 275, 185, 175,
      199, 218, 226, 207, 240, 239, 206, 217, 169, 155, 144, 129, 116, 128, 155,
      155, 144, 163, 167, 168, 159, 136, 154, 131, 421, 294, 397, 127, 133, 145,
      129, 165, 350, 179, 124, 499, 329, 655, 405, 610, 595, 523, 577, 430, 327,
      369, 502, 492, 414, 332, 263, 266, 283, 242, 368, 527, 630, 577, 465, 563,
      596, 663, 277, 163, 125, 258, 425, 634, 567, 538, 596, 563, 129, 129, 138,
      128, 150, 148, 157, 147, 165, 178, 151, 132, 152, 173, 182, 265, 217, 212,
      204, 205, 175, 176, 205, 204, 212, 217, 265, 182, 173, 152, 132, 151, 178,
      165, 147, 157, 148, 150, 128, 138, 129, 129, 563, 596, 537, 568, 634, 425,
      258, 125, 163, 276, 663, 597, 563, 465, 577, 629, 528, 368, 242, 283, 266,
      263, 331, 414, 493, 502, 369, 327, 430, 577, 523, 594, 611, 404, 656, 328,
      500, 124, 179, 350, 165, 129, 145, 133, 127, 397, 293, 422, 131, 154, 136,
      159, 168, 167, 163, 144, 155, 155, 128, 116, 129, 144, 155, 168, 218, 206,
      239, 240, 207, 226, 217, 200, 175, 185, 271, 327, 186, 166, 203, 190, 266,
      211, 165, 152, 169, 147, 142, 149, 210, 203, 204, 191, 158, 131, 117, 133,
      150, 139, 142, 165, 187, 261, 190, 166, 203, 190, 303, 232, 202, 167, 198,
      242, 222, 208, 239, 239, 206, 218, 168, 155, 144, 129, 116, 128, 155, 156,
      143, 163, 168, 167, 159, 136, 154, 131, 422, 293, 397, 127, 133, 145, 129,
      165, 351, 178, 124, 500, 329, 655, 404, 611, 595, 522, 577, 430, 327, 369,
      503, 492, 414, 331, 264, 265, 283, 242, 369, 527, 629, 577, 465, 564, 596,
      663, 276, 163, 126, 257, 425, 635, 567, 538, 595, 564, 117, 123, 145, 146,
      188, 210, 172, 236, 163, 140, 146, 164, 152, 166, 191, 236, 229, 173, 158,
      145, 137, 172, 188, 226, 212, 251, 256, 187, 165, 142, 139, 150, 133, 117,
      131, 158, 191, 204, 203, 210, 149, 143, 146, 169, 152, 165, 211, 273, 190,
      203, 166, 186, 330};

  int myRank;
  int numRanks;
  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
  MPI_Comm_size(MPI_COMM_WORLD, &numRanks);
  assert(numRanks <= 1024);

  std::vector<int> displ(counts.size() + 1);
  std::inclusive_scan(counts.begin(), counts.end(), displ.begin() + 1);

  std::vector<int> buffer(displ[numRanks]);
  for (int i = displ[myRank]; i < displ[myRank + 1]; ++i)
  {
    buffer[i] = i; // each rank fills in counts[myRank] elements starting from displ[myRank]
  }

  int* d_buffer;
  int* d_buffer_dest;
  cudaMalloc(&d_buffer, sizeof(int) * buffer.size());
  cudaMalloc(&d_buffer_dest, sizeof(int) * buffer.size());
  cudaMemcpy(d_buffer, buffer.data(), sizeof(int) * buffer.size(), cudaMemcpyHostToDevice);

  // CPU - works for all numRanks <= 1024
  //MPI_Allgatherv(MPI_IN_PLACE, 0, MPI_INT, buffer.data(), counts.data(), displ.data(), MPI_INT, MPI_COMM_WORLD);

  // GPU in-place - fails for numRanks >= 40
  MPI_Allgatherv(MPI_IN_PLACE, 0, MPI_INT, d_buffer, counts.data(), displ.data(), MPI_INT, MPI_COMM_WORLD);
  cudaMemcpy(buffer.data(), d_buffer, sizeof(int) * buffer.size(), cudaMemcpyDeviceToHost);

  // GPU src-to-dest - works for all numRanks <= 1024
  //MPI_Allgatherv(d_buffer + displ[myRank], counts[myRank], MPI_INT, d_buffer_dest, counts.data(), displ.data(), MPI_INT, MPI_COMM_WORLD);
  //cudaMemcpy(buffer.data(), d_buffer_dest, sizeof(int) * buffer.size(), cudaMemcpyDeviceToHost);

  // each rank should now have the full sequence 0, 1, buffer.size() - 1 in buffer

  std::size_t sum = std::accumulate(buffer.begin(), buffer.begin() + displ[numRanks], 0lu);

  std::size_t n = displ[numRanks] - 1;     // one less element because sequence starts from 0
  std::size_t reference = n * (n + 1) / 2; // sum of all numbers between 1 and n

  int pass = (reference == sum);
  int numPassed;

  MPI_Allreduce(&pass, &numPassed, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

  if (myRank == 0)
  {
    if (numPassed == numRanks) std::cout << "PASS, checksum: " << sum << std::endl;
    else {
        std::cout << "FAIL reference sum: " << reference << ", actual: " << sum << std::endl;
    }
  }

  cudaFree(d_buffer);
  cudaFree(d_buffer_dest);
  
  MPI_Finalize();
}


