

#include "connection.h" // struct Connection;


#include "end_point_ipv4.h"

#include <iostream>
#include <string.h>

p2p::End_Point_IPv4::~End_Point_IPv4() {}

p2p::End_Point_IPv4::End_Point_IPv4( int __address, short __port ) : address( __address ), port( __port) {}

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

p2p::Connection* p2p::End_Point_IPv4::get_connection() {}

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
