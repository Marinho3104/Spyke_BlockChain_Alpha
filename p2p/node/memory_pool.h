

#ifndef P2P_NODE_MEMORY_POOL_H
#define P2P_NODE_MEMORY_POOL_H

namespace p2p {

    // Struct to hold addresses of transactions not confirmed yet
    // Separated by types
    struct Memory_Pool {

        void* pool;

        ~Memory_Pool(); Memory_Pool( unsigned long long );

    };

}

#endif