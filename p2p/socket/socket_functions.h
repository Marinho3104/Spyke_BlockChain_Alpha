

#ifndef P2P_SOCKET_SOCKET_FUNCTIONS_H
#define P2P_SOCKET_SOCKET_FUNCTIONS_H

#include "socket_definitions.h"

namespace p2p {

    // Checks the connection internet protocol version and executes the correct connect function
    // Returns the other function return
    bool connect_to( Connection* ); 

    // Tries to establish a connection with given information IPv4
    // Returns true if it establish connection, or false 
    bool connect_to_IPv4( Connection* );

    // Tries to establish a connection with given information IPv6
    // Returns true if it establish connection, or false 
    bool connect_to_IPv6( Connection* );

    // Checks the connection internet protocol version and executes the correct set_server function
    // Returns the other function return 
    bool set_server( Connection*, int );

    // Tries to set a server with given information IPv4
    // Returns true if the server was set, or false 
    bool set_server_IPv4( Connection*, int );

    // Tries to set a server with given information IPv6
    // Returns true if the server was set, or false 
    bool set_server_IPv6( Connection*, int );

}

#endif
