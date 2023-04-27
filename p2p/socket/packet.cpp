
#include "packet.h"

#include <iostream>
#include <string.h>

p2p::Packet::~Packet() {

}

p2p::Packet::Packet( unsigned char __protocol_id, unsigned long long __data_size, void* __data )
    : protocol_id( __protocol_id ), data_size( __data_size ), data( __data ) {}

unsigned long long p2p::Packet::get_data_representation_length() 
    { return P2P_SOCKET_PACKET_PROTOCOL_ID_LENGTH + P2P_SOCKET_PACKET_DATA_SIZE_LENGTH + data_size; }

void* p2p::Packet::get_data_representation() {

    void* _data = 
        malloc( get_data_representation_length() ), *_rtr = _data;

    memcpy(
        _data,
        &protocol_id,
        P2P_SOCKET_PACKET_PROTOCOL_ID_LENGTH
    ); _data = _data + P2P_SOCKET_PACKET_PROTOCOL_ID_LENGTH;

    memcpy(
        _data,
        &data_size,
        P2P_SOCKET_PACKET_DATA_SIZE_LENGTH
    ); _data = _data + P2P_SOCKET_PACKET_DATA_SIZE_LENGTH;

    memcpy(
        _data,
        data,
        data_size
    );
    
    return _rtr;

}


p2p::Packet* p2p::Packet::get_packet_from_data( unsigned char* __header, void* __body ) {

    p2p::Packet* _packet = 
        ( p2p::Packet* ) malloc( sizeof( p2p::Packet ) );

    new( _packet ) p2p::Packet(
        *__header,
        *( ( unsigned long long* ) ( __header + 1 ) ),
        __body
    ); 

    return 
        _packet;

}