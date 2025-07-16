#include <stdio.h>
#include <hip/hip_runtime.h>

int main(int argc, char **argv){
    hipDeviceProp dP;
    float min_cc = 3.0;

    int rc = hipGetDeviceProperties(&dP, 0);
    if(rc != hipSuccess) {
        hipError_t error = hipGetLastError();
        printf("hip error: %s", hipGetErrorString(error));
        return rc; /* Failure */
    }
    if((dP.major+(dP.minor/10)) < min_cc) {
        printf("Min Compute Capability of %2.1f required:  %d.%d found\n Not Building hip Code", min_cc, dP.major, dP.minor);
        return 1; /* Failure */
    } else {
        printf("-arch=sm_%d%d", dP.major, dP.minor);
        return 0; /* Success */
    }
}
