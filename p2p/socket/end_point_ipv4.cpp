

#include "connection.h" // struct Connection;
#include "utils_functions.h" // convert_hex_bytes();

#include "end_point_ipv4.h"

#include <iostream>
#include <string.h>

p2p::End_Point_IPv4::~End_Point_IPv4() {}

p2p::End_Point_IPv4::End_Point_IPv4( unsigned int __address, unsigned short __port ) : address( __address ), port( __port ) {}

p2p::End_Point_IPv4::End_Point_IPv4( void* __hex_value, unsigned short __port ) : port( __port ) {

    void* _bytes = utils::convert_hex_bytes( __hex_value, P2P_SOCKET_END_POINT_IPV4_ADDRESS_LENGTH * 2, 1 );

    address = *( ( unsigned int* ) _bytes );

    free( _bytes );

}

p2p::End_Point_IPv4::End_Point_IPv4( void* __data ) { 
    
    memcpy(
        &address,
        __data,
        P2P_SOCKET_END_POINT_IPV4_ADDRESS_LENGTH
    );

    memcpy(
        &port,
        __data + P2P_SOCKET_END_POINT_IPV4_ADDRESS_LENGTH,
        P2P_SOCKET_END_POINT_IPV4_PORT_LENGTH
    ); 
    
}

p2p::Connection* p2p::End_Point_IPv4::get_connection() {

    p2p::Connection* _connection = 
        ( p2p::Connection* ) malloc( sizeof( p2p::Connection ) );

    new ( _connection ) 
        p2p::Connection(
            P2P_SOCKET_CONNECTION_VERSION_IPV4, this
        );

    return _connection;

}   

void* p2p::End_Point_IPv4::get_data() {

    void* _data =   
        malloc( P2P_SOCKET_END_POINT_IPV4_ADDRESS_LENGTH + P2P_SOCKET_END_POINT_IPV4_PORT_LENGTH );

    memcpy(
        _data,
        &address,
        P2P_SOCKET_END_POINT_IPV4_ADDRESS_LENGTH
    );

    memcpy(
        _data + P2P_SOCKET_END_POINT_IPV4_ADDRESS_LENGTH,
        &port,
        P2P_SOCKET_END_POINT_IPV4_PORT_LENGTH
    );

    return _data;

}

void p2p::End_Point_IPv4::print_representation() {

    std::cout << ( address >> 24 & 0xff ) << "." << ( address >> 16 & 0xff ) << "." << ( address >> 8 & 0xff ) << "." << ( address & 0xff );

    std::cout << ":";

    std::cout << port; 

}

