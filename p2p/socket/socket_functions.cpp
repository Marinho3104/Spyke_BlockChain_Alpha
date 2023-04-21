

#include "end_point_ipv4.h" // struct End_Point_IPv4;
#include "end_point_ipv6.h" // struct End_Point_IPv6;
#include "connection.h" // struct Connection;
#include "socket_exceptions.h" // struct Unknow_Connection_Version_Exception;


#include "socket_functions.h" 

#include <iostream>
#include <string.h>


bool p2p::connect_to( Connection* __connection ) {

    switch ( __connection->version )
    {
    case P2P_SOCKET_CONNECTION_VERSION_IPV4: return connect_to_IPv4( __connection ); break;
    case P2P_SOCKET_CONNECTION_VERSION_IPV6: return connect_to_IPv4( __connection ); break;
    default: break;
    }

    throw Unknow_Connection_Version_Exception(); return 0;

}

bool p2p::connect_to_IPv4( Connection* __connection ) {

    // Makes a copy of the end point to avoid the need of casting every time we need it
    End_Point_IPv4* _end_point = 
        ( End_Point_IPv4* ) __connection->end_point;

    // Create the socket
    __connection->socket_descriptor = socket( AF_INET, SOCK_STREAM, 0 );

    if ( __connection->socket_descriptor < 0 ) return 0;

    // Allocates memory for hint
    sockaddr_in* _hint = 
        ( sockaddr_in* ) malloc( sizeof( sockaddr_in ) );

    // Set the socket information into hint
    _hint->sin_family = AF_INET;
    _hint->sin_port = htons( _end_point->port );
    _hint->sin_addr.s_addr = htonl( _end_point->address );

    // Connect to socket 
    if (
        connect(
            __connection->socket_descriptor,
            ( sockaddr* ) _hint,
            sizeof( _hint )
        ) < 0
    ) return 0;

    return 1;

}

bool p2p::connect_to_IPv6( Connection* __connection ) {

    // Makes a copy of the end point to avoid the need of casting every time we need it
    End_Point_IPv6* _end_point = 
        ( End_Point_IPv6* ) __connection->end_point;

    // Create the socket
    __connection->socket_descriptor = socket( AF_INET6, SOCK_STREAM, 0 );

    if ( __connection->socket_descriptor < 0 ) return 0;

    // Allocates memory for hint
    sockaddr_in6* _hint = 
        ( sockaddr_in6* ) malloc( sizeof( sockaddr_in6 ) );

    // Set the socket information into hint
    _hint->sin6_family = AF_INET6;
    _hint->sin6_port = htons( _end_point->port );
    memcpy( &_hint->sin6_addr, _end_point->address, sizeof( _end_point->address ) );

    // Connect to socket 
    if (
        connect(
            __connection->socket_descriptor,
            ( sockaddr* ) _hint,
            sizeof( _hint )
        ) < 0
    ) return 0;

    return 1;

}

bool p2p::set_server( Connection* __connection, int __max_connections ) {

    switch ( __connection->version )
    {
    case P2P_SOCKET_CONNECTION_VERSION_IPV4: return set_server_IPv4( __connection, __max_connections ); break;
    case P2P_SOCKET_CONNECTION_VERSION_IPV6: return set_server_IPv6( __connection, __max_connections ); break;
    default: break;
    }

    throw Unknow_Connection_Version_Exception(); return 0;

}

bool p2p::set_server_IPv4( Connection* __connection, int __max_connections ) {

    // Makes a copy of the end point to avoid the need of casting every time we need it
    End_Point_IPv4* _end_point = 
        ( End_Point_IPv4* ) __connection->end_point;

    __connection->socket_descriptor = 
        socket( AF_INET, SOCK_STREAM, 0 );

    if ( __connection->socket_descriptor < 0 ) return 0;

    // Just to dont have to wait for to free the address and port when it is retarted
    // After the testing we can remove it
    int _opt = 1;
    if (
        setsockopt(
            __connection->socket_descriptor,
            SOL_SOCKET,
            SO_REUSEADDR | SO_REUSEPORT,
            &_opt,
            sizeof( _opt )
        )
    ) return 0;

    // Allocates memory for hint
    sockaddr_in* _hint = 
        ( sockaddr_in* ) malloc( sizeof( sockaddr_in ) );

    // Set the socket information into hint
    _hint->sin_family = AF_INET;
    _hint->sin_port = htons( _end_point->port );
    _hint->sin_addr.s_addr = htonl( _end_point->address );

    // Bind the server
    if (
        bind(
            __connection->socket_descriptor,
            ( sockaddr* ) _hint,
            sizeof( sockaddr )
        )
    ) return 0;

    // Listen 
    if (
        listen(
            __connection->socket_descriptor,
            __max_connections
        )
    ) return 0;

    return 1;

}

bool p2p::set_server_IPv6( Connection* __connection, int __max_connections ) { return 0; }