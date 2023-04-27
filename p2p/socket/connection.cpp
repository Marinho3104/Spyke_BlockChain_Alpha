

#include "end_point_ipv4.h" // End Point Internet Protocol Version 4
#include "end_point_ipv6.h" // End Point Internet Protocol Version 6
#include "socket_functions.h" // connect_to()
#include "socket_exceptions.h" // struct Unknow_Connection_Version_Exception;
#include "packet.h" // struct Packet;
#include "connection.h"

#include <semaphore.h>
#include <iostream>
#include <string.h>


p2p::Connection::~Connection() { free( hint ); free( end_point ); }

p2p::Connection::Connection( char __version, void* __end_point ) : version( __version ), end_point( __end_point ), hint( 0 ), socket_descriptor( -1 ),
    status( P2P_SOCKET_CONNECTION_STATUS_NOT_CONNECTED ), data( 0 ) { sem_init( &sem, 0, 1 ); }

p2p::Connection::Connection( char __version, void* __end_point, char __status, int __socket_descriptor, void* __hint )
    : version( __version ), end_point( __end_point ), status( __status ), socket_descriptor( __socket_descriptor ), hint( __hint ), data( 0 ) { sem_init( &sem, 0, 1 ); }

bool p2p::Connection::set_server( int __max_connections ) {

    status = p2p::set_server( this, __max_connections ) ? P2P_SOCKET_CONNECTION_STATUS_SERVER : P2P_SOCKET_CONNECTION_STATUS_NOT_CONNECTED;
    
    return status;

}

bool p2p::Connection::connect() {

    status = p2p::connect_to( this ) ? P2P_SOCKET_CONNECTION_STATUS_CONNECTED : P2P_SOCKET_CONNECTION_STATUS_NOT_CONNECTED;
    
    return status;

}

bool p2p::Connection::send_packet( p2p::Packet* __packet ) 
    { return p2p::send_packet_to( __packet, socket_descriptor ); }


void p2p::Connection::print_representation() {

    std::cout << "IP version: " << ( int ) version << " / ";

    std::cout << "End Point: ";

    switch ( version )
    {
    case P2P_SOCKET_CONNECTION_VERSION_IPV4: ( ( End_Point_IPv4* ) end_point )->print_representation(); break;
    case P2P_SOCKET_CONNECTION_VERSION_IPV6: ( ( End_Point_IPv6* ) end_point )->print_representation(); break;
    default: throw Unknow_Connection_Version_Exception(); break;
    }

    std::cout << " / ";

    std::cout << "Current Connection status: " << ( int ) status << " / ";

    std::cout << "Socket Descriptor: " << socket_descriptor;

}

unsigned long long p2p::Connection::get_data_size() {

    switch ( version )
    {
    case P2P_SOCKET_CONNECTION_VERSION_IPV4: return P2P_SOCKET_VERSION_LENGTH + P2P_SOCKET_END_POINT_IPV4_ADDRESS_LENGTH + P2P_SOCKET_END_POINT_IPV4_PORT_LENGTH; break;
    case P2P_SOCKET_CONNECTION_VERSION_IPV6: return P2P_SOCKET_VERSION_LENGTH + P2P_SOCKET_END_POINT_IPV6_ADDRESS_LENGTH + P2P_SOCKET_END_POINT_IPV6_PORT_LENGTH; break;        
    default: break;
    }

    throw p2p::Unknow_Connection_Version_Exception(); 

    return -1;

}


void* p2p::Connection::get_data() {

    void* _data = 
        malloc( get_data_size() );

    memcpy(
        _data,
        &version,
        P2P_SOCKET_VERSION_LENGTH
    );

    void* _end_point_data;

    switch ( version )
    {
    case P2P_SOCKET_CONNECTION_VERSION_IPV4: 

        _end_point_data = ( ( End_Point_IPv4* ) end_point )->get_data();

        memcpy(
            _data + P2P_SOCKET_VERSION_LENGTH,
            _end_point_data,
            P2P_SOCKET_END_POINT_IPV4_ADDRESS_LENGTH + P2P_SOCKET_END_POINT_IPV4_PORT_LENGTH
        );

        break;

    case P2P_SOCKET_CONNECTION_VERSION_IPV6: 
    
        _end_point_data = ( ( End_Point_IPv6* ) _end_point_data )->get_data(); 
        
        memcpy(
            _data + P2P_SOCKET_VERSION_LENGTH,
            _end_point_data,
            P2P_SOCKET_END_POINT_IPV6_ADDRESS_LENGTH + P2P_SOCKET_END_POINT_IPV6_PORT_LENGTH
        );
        
        break;

    default: throw p2p::Unknow_Connection_Version_Exception(); break;
    }

    free( _end_point_data );

    return _data;

}

p2p::Connection* p2p::Connection::get_connection_by_data( void* __data ) {

    char _version = *( ( char* ) __data ); __data = __data + 1;

    void* _end_point;

    switch ( _version )
    {
    case P2P_SOCKET_CONNECTION_VERSION_IPV4: 
    
        _end_point = 
            malloc( sizeof( End_Point_IPv4 ) );

        new( _end_point ) End_Point_IPv4( __data );

        break;

    case P2P_SOCKET_CONNECTION_VERSION_IPV6: 
    
        _end_point = 
            malloc( sizeof( End_Point_IPv6 ) );
        
        new( _end_point ) End_Point_IPv6( __data );

        break;

    default: throw p2p::Unknow_Connection_Version_Exception(); break;
    }

    p2p::Connection* _connection = 
        ( p2p::Connection* ) malloc( sizeof( p2p::Connection ) );

    new ( _connection ) p2p::Connection( _version, _end_point );

    return _connection;

}

p2p::Connection* p2p::Connection::get_new_connection_manually_hex( char _back_slash_t) {

    std::cout << std::endl;
    
    for ( char _ = 0; _ < _back_slash_t; _++ ) std::cout << "\t";

    std::cout << "Ip version [ 4 / 6 ]: ";
    std::string _version; std::cin >> _version;

    for ( char _ = 0; _ < _back_slash_t; _++ ) std::cout << "\t";

    std::cout << "Address: 0x";
    std::string _address; std::cin >> _address;

    for ( char _ = 0; _ < _back_slash_t; _++ ) std::cout << "\t";

    std::cout << "Port: ";
    std::string _port; std::cin >> _port;

    if ( *_version.c_str() == '6' ) {

        p2p::End_Point_IPv6* _end_point = 
            ( p2p::End_Point_IPv6* ) malloc( sizeof( p2p::End_Point_IPv6 ) );

        new ( _end_point ) p2p::End_Point_IPv6(
            ( void* ) _address.c_str(), atoi( _port.c_str() )
        );

        return _end_point->get_connection();

    }

    else {

        p2p::End_Point_IPv4* _end_point = 
            ( p2p::End_Point_IPv4* ) malloc( sizeof( p2p::End_Point_IPv4 ) );

        new ( _end_point ) p2p::End_Point_IPv4(
            ( void* ) _address.c_str(), atoi( _port.c_str() )
        );

        return _end_point->get_connection();

    }

    // Never
    return 0;

}

