
#include "connection.h" // struct Connection;

#include "end_point_ipv6.h" 

#include <iostream>
#include <string.h>


p2p::End_Point_IPv6::~End_Point_IPv6() {}

p2p::End_Point_IPv6::End_Point_IPv6( unsigned char* __address, short __port ) : port( __port ) {

    memcpy(
        address,
        __address,
        16
    );

}

p2p::End_Point_IPv6::End_Point_IPv6( void* __hex, short __port ) : port( __port ) {

    

} 

p2p::End_Point_IPv6::End_Point_IPv6( void* __data ) {

    memcpy(
        address,
        __data,
        P2P_SOCKET_END_POINT_IPV6_ADDRESS_LENGTH
    );

    memcpy(
        &port,
        __data + P2P_SOCKET_END_POINT_IPV6_ADDRESS_LENGTH,
        P2P_SOCKET_END_POINT_IPV6_PORT_LENGTH
    );

}

p2p::Connection* p2p::End_Point_IPv6::get_connection() {}

void* p2p::End_Point_IPv6::get_data() {

    void* _data =   
        malloc( P2P_SOCKET_END_POINT_IPV6_ADDRESS_LENGTH + P2P_SOCKET_END_POINT_IPV6_PORT_LENGTH );

    memcpy(
        _data,
        address,
        P2P_SOCKET_END_POINT_IPV6_ADDRESS_LENGTH
    );

    memcpy(
        _data + P2P_SOCKET_END_POINT_IPV6_ADDRESS_LENGTH,
        &port,
        P2P_SOCKET_END_POINT_IPV6_PORT_LENGTH
    );

    return _data;

}

