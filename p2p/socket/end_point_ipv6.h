

#ifndef P2P_SOCKET_END_POINT_IPV6_H
#define P2P_SOCKET_END_POINT_IPV6_H

#define P2P_SOCKET_END_POINT_IPV6_ADDRESS_LENGTH 16
#define P2P_SOCKET_END_POINT_IPV6_PORT_LENGTH 2

#include "socket_definitions.h" // struct Connection;

namespace p2p {

    // Represents a socket information in IPv6 ( address 16 bytes  ) 
    struct End_Point_IPv6 {

        // Array with 16 bytes for address representation
        unsigned char address[ 16 ];
        short port;

        ~End_Point_IPv6(); End_Point_IPv6( unsigned char*, short ); End_Point_IPv6( void* );

        // Returns a ready to use connection with this End_Point information
        Connection* get_connection();

        // Get this struct data
        void* get_data();

    };

}

#endif