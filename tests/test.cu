#include <stdio.h>
#include <cuda_runtime.h>

__global__ void mm() {
    while(1);
}

__host__ void test(void*) {
    printf("Host function\n");
}

int main() {
    cudaStream_t stream;
    cudaStreamCreate(&stream);

    cudaStream_t stream_1;
    cudaStreamCreate(&stream_1);

    cudaEvent_t event;
    cudaEventCreate(&event);


    mm<<<1,1, 0, stream_1>>>();
    cudaLaunchHostFunc(stream, test, 0);

    cudaEventRecord(event, stream);
    cudaStreamWaitEvent(stream, event, 0);

    cudaStreamSynchronize(stream_1);

    cudaEventDestroy(event);
    cudaStreamDestroy(stream);

    return 0;
}

/*
__global__ void handle_share_protocol_object( cudaSemaphore_t* _sem ) {
    int _global_id = blockIdx.x * blockDim.x + threadIdx.x;
    
    printf("Waiting for data ... ");

    cudaSemaphoreWait(semaphore);

    

}

// Stream for share protocol
cudaStream_t share_protocol_handle_object_cuda_stream;

extern "C" {

    // Initialize the stream variable and functions to block threads
    void initialize_cuda_share_protocol() {

    }

}


/*
cudaSemaphore_t semaphore;
    cudaSemaphoreCreate(&semaphore, 1);


*/

