

#ifndef P2P_SOCKET_SOCKET_FUNCTIONS_H
#define P2P_SOCKET_SOCKET_FUNCTIONS_H

#include "socket_definitions.h"

namespace p2p {

    // Checks the connection internet protocol version and executes the correct connect function
    // Returns the other function return
    bool connect_to( Connection* ); 

    // Executes a disconnection with given socket descriptor 
    void disconnect_from( Connection* );

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

    // Accept a new connection request 
    // Returns a struct Connection with all information set about the connectionº
    p2p::Connection* accept_new_connection_request( int );

    // Send a packet to a socket descriptor
    // Return 1 if succes or 0
    bool send_packet_to( p2p::Packet*, int );

    // Get a packet from a socket descriptor
    // Return the packet or 0, if some error occur malloc()
    p2p::Packet* get_packet_from( int );

}

#endif
