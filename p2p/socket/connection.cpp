

#include "end_point_ipv4.h" // End Point Internet Protocol Version 4
#include "socket_functions.h" // connect_to()

#include "connection.h"

#include <semaphore.h>
#include <iostream>


p2p::Connection::~Connection() { free( hint ); }

p2p::Connection::Connection( char __version, void* __end_point ) : version( __version ), end_point( __end_point ), hint( 0 ),
    status( P2P_SOCKET_CONNECTION_STATUS_NOT_CONNECTED ) { sem_init( &sem, 0, 1 ); }

p2p::Connection::Connection( char __version, void* __end_point, char __status, int __socket_descriptor, void* __hint )
    : version( __version ), end_point( __end_point ), status( __status ), socket_descriptor( __socket_descriptor ), hint( __hint ) { sem_init( &sem, 0, 1 ); }

bool p2p::Connection::set_server( int __max_connections ) {

    status = p2p::set_server( this, __max_connections ) ? P2P_SOCKET_CONNECTION_STATUS_SERVER : P2P_SOCKET_CONNECTION_STATUS_NOT_CONNECTED;
    
    return status;

}

bool p2p::Connection::connect() {

    status = p2p::connect_to( this ) ? P2P_SOCKET_CONNECTION_STATUS_CONNECTED : P2P_SOCKET_CONNECTION_STATUS_NOT_CONNECTED;
    
    return status;

}



