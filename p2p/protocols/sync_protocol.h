

#ifndef P2P_PROTOCOLS_SYNC_PROTOCOL_H
#define P2P_PROTOCOLS_SYNC_PROTOCOL_H

extern "C" {

    void handle_sync_protocol_data();

}

#include "socket_definitions.h"

#define P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_LENGTH 1

#define P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_REQUEST 0
#define P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_DATA 1
#define P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_DATA_RECEIVED 2
#define P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_DONE 3

namespace p2p {

    struct Sync_Protocol {

        unsigned char state;
        unsigned long long data_size;
        void* data;

        ~Sync_Protocol(); Sync_Protocol( unsigned char, unsigned long long, void* );

        void handle(); 
        
        unsigned long long get_data_representation_size();

        void* get_data_representation(); 
        
        p2p::Packet* get_packet();

        static Sync_Protocol* get_sync_protocol_from_packet( p2p::Packet* );

    };

};

#endif