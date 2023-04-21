

#ifndef P2P_SOCKET_END_POINT_IPV4_H
#define P2P_SOCKET_END_POINT_IPV4_H

#define P2P_SOCKET_END_POINT_IPV4_ADDRESS_LENGTH 4
#define P2P_SOCKET_END_POINT_IPV4_PORT_LENGTH 2

#include "socket_definitions.h" // struct Connection;

namespace p2p {

    // Represents a socket information in IPv4 ( address 4 bytes  ) 
    struct End_Point_IPv4 {

        int address;
        short port;

        ~End_Point_IPv4(); End_Point_IPv4( int, short ); End_Point_IPv4( void*, short ); End_Point_IPv4( void* );

        // Returns a ready to use connection with this End_Point information
        Connection* get_connection();

        // Get this struct data
        void* get_data();

    };

}

#endif

