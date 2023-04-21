

#ifndef P2P_SOCKET_CONNECTION_H
#define P2P_SOCKET_CONNECTION_H

// Internet protocol version
#define P2P_SOCKET_CONNECTION_VERSION_IPV4 0
#define P2P_SOCKET_CONNECTION_VERSION_IPV6 1

// Current Connection status 
#define P2P_SOCKET_CONNECTION_STATUS_NOT_CONNECTED 0
#define P2P_SOCKET_CONNECTION_STATUS_CONNECTED 1
#define P2P_SOCKET_CONNECTION_STATUS_SERVER 2


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

    };

} 

#endif