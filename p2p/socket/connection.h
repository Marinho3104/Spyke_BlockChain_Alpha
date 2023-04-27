

#ifndef P2P_SOCKET_CONNECTION_H
#define P2P_SOCKET_CONNECTION_H

// Internet protocol version
#define P2P_SOCKET_CONNECTION_VERSION_IPV4 0
#define P2P_SOCKET_CONNECTION_VERSION_IPV6 1

// Current Connection status 
#define P2P_SOCKET_CONNECTION_STATUS_NOT_CONNECTED 0
#define P2P_SOCKET_CONNECTION_STATUS_CONNECTED 1
#define P2P_SOCKET_CONNECTION_STATUS_SERVER 2

#define P2P_SOCKET_VERSION_LENGTH 1
#define P2P_SOCKET_END_POINT_V4_LENGTH 1

#include "socket_definitions.h"

#include <semaphore.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>

namespace p2p {

    struct Connection {

        // Internet protocol in use ( IPv4 or IPv6 )
        char version;

        // Connection End Point
        void* end_point;

        // Connection status
        char status;

        // Socket descriptor
        int socket_descriptor;

        // Socket hint 
        void* hint;

        // Semaphore to execute functions synchronize
        sem_t sem;

        // Connection Data
        void* data;

        ~Connection(); 
        
        // Gets Version and End Point
        Connection( char, void* );
        
        // Gets Version, End Point, Connection Status, Socket Descriptor and Hint
        Connection( char, void*, char, int, void* );

        // Set a server up with end point information
        // Returns true if it set the server up, or false
        bool set_server( int );

        // Connect to end point information
        // Returns true if it establish a connection with end point information
        bool connect();

        // send a packet to this socket descriptor
        bool send_packet( p2p::Packet* );

        // Print a string representing all fields of this connection
        void print_representation();

        // Returns the data of end point and ip version length of this conneciton
        unsigned long long get_data_size();

        // Returns the data of end point and ip version of this connection
        // malloc() data
        void* get_data();

        static Connection* get_connection_by_data( void* );

        static Connection* get_new_connection_manually_hex( char );

    };

} 

#endif