

#include "sync_protocol.h"
#include "protocol_ids.h"
#include "packet.h"

#include <iostream>
#include <string.h>

p2p::Sync_Protocol::~Sync_Protocol() {

}

p2p::Sync_Protocol::Sync_Protocol( unsigned char __state, unsigned long long __data_size, void* __data ) 
    : state( __state ), data_size( __data_size ), data( __data ) {}

void p2p::Sync_Protocol::handle() {

    switch ( state )
    {
    
    default: break;
    }

}

unsigned long long p2p::Sync_Protocol::get_data_representation_size() 
    { return P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_LENGTH + data_size; }

void* p2p::Sync_Protocol::get_data_representation() {

    void* _data = 
        malloc( get_data_representation_size() ), *_rtr = _data;

    memcpy(
        _data,
        &state,
        P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_LENGTH
    ); _data = _data + P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_LENGTH;

    memcpy(
        _data,
        data,
        data_size
    );

    return _rtr;

} 

p2p::Packet* p2p::Sync_Protocol::get_packet() {

    void* _data = 
        get_data_representation();

    p2p::Packet* _packet = 
        ( p2p::Packet* ) malloc( sizeof( p2p::Packet ) );

    new ( _packet ) p2p::Packet(
        P2P_PROTOCOLS_PROTOCOL_ID_SYNC_PROTOCOL,
        get_data_representation_size(),
        _data
    );

    return _packet;

}

p2p::Sync_Protocol* p2p::Sync_Protocol::get_sync_protocol_from_packet( p2p::Packet* __packet ) {

    p2p::Sync_Protocol* _sync_protocol = 
        ( p2p::Sync_Protocol* ) malloc( sizeof( p2p::Sync_Protocol ) );

    new ( _sync_protocol ) p2p::Sync_Protocol(
        *( ( unsigned char* ) ( __packet->data ) ),
        __packet->data_size - 1,
        __packet->data + 1
    );

    return _sync_protocol;

}

