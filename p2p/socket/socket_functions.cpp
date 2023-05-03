

#include "end_point_ipv4.h" // struct End_Point_IPv4;
#include "end_point_ipv6.h" // struct End_Point_IPv6;
#include "connection.h" // struct Connection;
#include "socket_exceptions.h" // struct Unknow_Connection_Version_Exception;
#include "packet.h" // struct Packet;
 
#include "socket_functions.h" 

#include <iostream>
#include <unistd.h>
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

void p2p::disconnect_from( Connection* __connection ) { close( __connection->socket_descriptor ); }

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
            sizeof( *_hint )
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
            sizeof( *_hint )
        ) == -1
    ) return 0;

    // Listen 
    if (
        listen(
            __connection->socket_descriptor,
            __max_connections
        )
    )return 0;

    return 1;

}

bool p2p::set_server_IPv6( Connection* __connection, int __max_connections ) { return 0; }

p2p::Connection* p2p::accept_new_connection_request( int __server_socket_descriptor ) {

    socklen_t _new_connection_size = sizeof( sockaddr_in );
    sockaddr_in _accept_new_connection_hint;

    int _new_connection_socket_descriptor = 

        accept(
            __server_socket_descriptor,
            ( sockaddr* ) &_accept_new_connection_hint,
            &_new_connection_size
        );

    if ( _new_connection_socket_descriptor == -1 ) return 0;

    void* _end_point; char _version;

    sockaddr_in* _new_connection_hint = (sockaddr_in*) malloc( sizeof( sockaddr_in ) );
    memcpy( _new_connection_hint, &_accept_new_connection_hint, sizeof( sockaddr_in ) );

    if ( _accept_new_connection_hint.sin_family == AF_INET ) {

        _version = P2P_SOCKET_CONNECTION_VERSION_IPV4;

        _end_point = 
            malloc( sizeof( End_Point_IPv4 ) );

        new ( _end_point ) End_Point_IPv4(
            ntohl( _accept_new_connection_hint.sin_addr.s_addr ),
            ntohs( _accept_new_connection_hint.sin_port )
        );

    } 

    else {

        std::cout << "To implement get endpoint to ipv6" << std::endl; exit( 1 );

    }

    p2p::Connection* _new_connection = 
        ( p2p::Connection* ) malloc( sizeof( p2p::Connection ) );

    new ( _new_connection ) p2p::Connection(
        _version,
        _end_point,
        P2P_SOCKET_CONNECTION_STATUS_CONNECTED,
        _new_connection_socket_descriptor,
        _new_connection_hint
    );

    return _new_connection;

}

bool p2p::send_packet_to( p2p::Packet* __packet, int __socket_descriptor ) {

    void* _data_representation =
        __packet->get_data_representation();

    int _sts = 
        send(
            __socket_descriptor,
            _data_representation,
            __packet->get_data_representation_length(),
            0
        );

    free( _data_representation );

    return _sts;

}

p2p::Packet* p2p::get_packet_from( int __socket_descriptor ) {

    unsigned char
        _header[ P2P_SOCKET_PACKET_PROTOCOL_ID_LENGTH + P2P_SOCKET_PACKET_DATA_SIZE_LENGTH ];

    long long _sts = 
        recv(
            __socket_descriptor,
            _header,
            P2P_SOCKET_PACKET_PROTOCOL_ID_LENGTH + P2P_SOCKET_PACKET_DATA_SIZE_LENGTH,
            0
        );

    if ( _sts != P2P_SOCKET_PACKET_PROTOCOL_ID_LENGTH + P2P_SOCKET_PACKET_DATA_SIZE_LENGTH ) return 0;

    long long _data_size =
        *( ( unsigned long long* ) ( _header + 1 ) );

    if ( ! _data_size )

        return 
            p2p::Packet::get_packet_from_data(
                _header, 0
            );

    // Should be malloc for bigger sizes TODO
    void* _body = malloc( _data_size );

    _sts = 
        recv(
            __socket_descriptor,
            _body,
            _data_size,
            0
        );

    if ( _sts != _data_size ) return 0;
    
    return 
        p2p::Packet::get_packet_from_data(
            _header, _body
        );

}

