#include <cuda/semaphore>
#include <cuda_runtime.h>
#include <stdio.h>

// Defines
#define P2P_PROTOCOLS_SHARE_PROTOCOL_CUDA_BLOCKS 1
#define P2P_PROTOCOLS_SHARE_PROTOCOL_CUDA_THREAD_BLOCKS 1024

#define P2P_PROTOCOLS_SHARE_PROTOCOL_CUDA_THREAD_BLOCKS_SEMAPHORES_SIZE \
    ( sizeof(cuda::std::binary_semaphore) * P2P_PROTOCOLS_SHARE_PROTOCOL_CUDA_THREAD_BLOCKS * P2P_PROTOCOLS_SHARE_PROTOCOL_CUDA_BLOCKS )


// Stream to add data into thread blocks
cudaStream_t stream_add_data;

// Stream to execute all thread blocks
cudaStream_t stream_thread_blocks;

// Semaphore that indicates when data is available
cuda::std::binary_semaphore* _sem_data_available;

// Semaphore that indicates if a thread block is working ( 0: Means is working 1: Means is not working)
cuda::std::binary_semaphore* _sem_is_working;


// Function to compute all confirmations about a transactions
__global__ void compute_transaction_confirmations( cuda::std::binary_semaphore* _sem_data_available, cuda::std::binary_semaphore* _sem_is_working ) {

    int _global_id = 
        blockIdx.x * blockDim.x + threadIdx.x;

    while( 1 ) {

        _sem_data_available[ _global_id ].acquire();

        printf("Data received %d\n", _global_id);

        clock_t start = clock64(); // Record the start time
        clock_t end = start + 10 * 1000000000ULL; // Calculate the end time (5 seconds from start)
        while (clock64() < end) {
            // Do nothing, just wait
        }

        printf("Done");

        _sem_is_working[ _global_id ].release();

    }

}

void add_transaction_data_into_stream( void* __args ) {

    for ( int _ = 0; _ < P2P_PROTOCOLS_SHARE_PROTOCOL_CUDA_THREAD_BLOCKS * P2P_PROTOCOLS_SHARE_PROTOCOL_CUDA_BLOCKS; _++ ) 

        if ( _sem_is_working[ _ ].try_acquire() ) { _sem_data_available[ _ ].release(); break; }

}


extern "C++" {

    // Initialize all needed variables
    void initialize_share_protocol_cuda() {

        // Creates the streams
        cudaStreamCreate(&stream_add_data);
        cudaStreamCreate(&stream_thread_blocks);

        // Heap allocation for semaphores 
        cudaMallocManaged( &_sem_data_available, P2P_PROTOCOLS_SHARE_PROTOCOL_CUDA_THREAD_BLOCKS_SEMAPHORES_SIZE );
        cudaMallocManaged( &_sem_is_working, P2P_PROTOCOLS_SHARE_PROTOCOL_CUDA_THREAD_BLOCKS_SEMAPHORES_SIZE );

        // Inicialize semaphores 
        for (int _ = 0; _ < P2P_PROTOCOLS_SHARE_PROTOCOL_CUDA_THREAD_BLOCKS * P2P_PROTOCOLS_SHARE_PROTOCOL_CUDA_BLOCKS; _++) {

            new(_sem_data_available + _) cuda::std::binary_semaphore(0); new(_sem_is_working + _) cuda::std::binary_semaphore(1);

        }

        compute_transaction_confirmations<<<P2P_PROTOCOLS_SHARE_PROTOCOL_CUDA_BLOCKS, P2P_PROTOCOLS_SHARE_PROTOCOL_CUDA_THREAD_BLOCKS, 0, stream_thread_blocks>>>( _sem_data_available, _sem_is_working );

    }

    // Adds a new host function into the stream to be executed
    void add_transaction_data_share_protocol_cuda(unsigned char __object_count, void* __data, unsigned long long __data_size) {
    
        cudaLaunchHostFunc(stream_add_data, add_transaction_data_into_stream, 0);

        cudaEvent_t event;
        cudaEventCreate(&event);

        cudaEventRecord(event, stream_add_data);

        cudaError_t err = cudaEventSynchronize(event);
        if (err != cudaSuccess) {
            printf("Error synchronizing event: %s\n", cudaGetErrorString(err));
            return;
        }
    
    }

}

