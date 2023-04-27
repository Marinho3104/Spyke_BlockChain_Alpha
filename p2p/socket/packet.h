

#ifndef P2P_SOCKET_PACKET_H
#define P2P_SOCKET_PACKET_H

#define P2P_SOCKET_PACKET_PROTOCOL_ID_LENGTH 1
#define P2P_SOCKET_PACKET_DATA_SIZE_LENGTH 8

namespace p2p {

    struct Packet {

        unsigned char protocol_id; 
        unsigned long long data_size;
        void* data;

        ~Packet(); Packet( unsigned char, unsigned long long, void* );

        // Return the length of this packet data representation
        unsigned long long get_data_representation_length();

        // Return the data representation of this packet
        // return malloc()
        void* get_data_representation();


        // Returns a packet with the header and body data of a packet
        // Returns malloc()
        static Packet* get_packet_from_data( unsigned char*, void* ); 

    };

}

#endif