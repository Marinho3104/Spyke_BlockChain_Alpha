

#ifndef P2P_PROTOCOLS_SHARE_PROTOCOL_H
#define P2P_PROTOCOLS_SHARE_PROTOCOL_H

#include "socket_definitions.h"

#define P2P_PROTOCOLS_SHARE_PROTOCOL_SHARED_OBJECT_TYPE_LENGTH 1
#define P2P_PROTOCOLS_SHARE_PROTOCOL_SHARED_COUNT_LENGTH 1
#define P2P_PROTOCOLS_SHARE_PROTOCOL_DATA_SIZE_LENGTH 8

#define P2P_PROTOCOLS_SHARE_PROTOCOL_SHARED_OBJECT_TYPE_TRANSACTION 0

extern "C++" { 

    // Initialize all variables and functions needed for share protocol    
    void initialize_share_protocol_cuda(); 

    // Adds a new object into thread to compute
    void add_transaction_data_share_protocol_cuda( unsigned char, void*, unsigned long long );
    
}

namespace p2p {

    struct Share_Protocol {

        unsigned char shared_object_type; unsigned char objects_count;
        void* data; unsigned long long data_size;


        ~Share_Protocol(); Share_Protocol( unsigned char, unsigned char, void*, unsigned long long );

        void handle( p2p::Connection* ); 
        
        unsigned long long get_data_representation_size();

        void* get_data_representation(); 
        
        p2p::Packet* get_packet();

        static Share_Protocol* get_share_protocol_from_packet( p2p::Packet* );

    };

}

#endif
