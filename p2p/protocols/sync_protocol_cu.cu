

// Responsible for handle the data received from sycn protocol

#include <stdio.h>

__global__ void handle_sync_data() {

    printf("Running in gpu");

}

extern "C" {

    __host__ void handle_sync_protocol_data() {

        handle_sync_data<<<1, 1>>>();

    }

}






