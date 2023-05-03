
#include "share_protocol.h"
#include "protocol_ids.h"
#include "packet.h"

#include <iostream>
#include <string.h>

p2p::Share_Protocol::~Share_Protocol() {}

p2p::Share_Protocol::Share_Protocol( unsigned char __shared_object_type, unsigned char __objects_count, void* __data, unsigned long long __data_size ) 
    : shared_object_type( __shared_object_type ), objects_count( __objects_count ), data( __data ), data_size( __data_size ) {}

void p2p::Share_Protocol::handle( p2p::Connection* __connection ) {

    std::cout << "Share Protocol" << std::endl;

    switch ( shared_object_type )
    {
    case P2P_PROTOCOLS_SHARE_PROTOCOL_SHARED_OBJECT_TYPE_TRANSACTION: add_transaction_data_share_protocol_cuda( objects_count, data, data_size ); break;
    default: std::cout << "Unknow type" << std::endl; break;
    }

}

unsigned long long p2p::Share_Protocol::get_data_representation_size() {
    return P2P_PROTOCOLS_SHARE_PROTOCOL_SHARED_OBJECT_TYPE_LENGTH + P2P_PROTOCOLS_SHARE_PROTOCOL_SHARED_COUNT_LENGTH + P2P_PROTOCOLS_SHARE_PROTOCOL_DATA_SIZE_LENGTH + data_size; 
}

void* p2p::Share_Protocol::get_data_representation() {

    void* _data = 
        malloc( get_data_representation_size() ), *_rtr = _data;

    memcpy(
        _data,
        &shared_object_type,
        P2P_PROTOCOLS_SHARE_PROTOCOL_SHARED_OBJECT_TYPE_LENGTH
    ); _data = _data + P2P_PROTOCOLS_SHARE_PROTOCOL_SHARED_OBJECT_TYPE_LENGTH;

    memcpy(
        _data,
        &objects_count,
        P2P_PROTOCOLS_SHARE_PROTOCOL_SHARED_COUNT_LENGTH
    ); _data = _data + P2P_PROTOCOLS_SHARE_PROTOCOL_SHARED_COUNT_LENGTH;

    memcpy(
        _data,
        &data_size,
        P2P_PROTOCOLS_SHARE_PROTOCOL_DATA_SIZE_LENGTH
    ); _data = _data + P2P_PROTOCOLS_SHARE_PROTOCOL_DATA_SIZE_LENGTH;

    memcpy(
        _data,
        data,
        data_size
    );

    return _rtr;

}

p2p::Packet* p2p::Share_Protocol::get_packet() {

    void* _data = 
        get_data_representation();

    p2p::Packet* _packet = 
        ( p2p::Packet* ) malloc( sizeof( p2p::Packet ) );

    new ( _packet ) p2p::Packet(
        P2P_PROTOCOLS_PROTOCOL_ID_SHARE_PROTOCOL,
        get_data_representation_size(),
        _data
    );

    return _packet;

}

p2p::Share_Protocol* p2p::Share_Protocol::get_share_protocol_from_packet( p2p::Packet* __packet ) {

    p2p::Share_Protocol* _share_protocol = 
        ( p2p::Share_Protocol* ) malloc( sizeof( p2p::Share_Protocol ) );

    new ( _share_protocol ) 
        p2p::Share_Protocol(
            *( ( unsigned char* ) __packet->data ),
            *( ( unsigned char* ) ( __packet->data + P2P_PROTOCOLS_SHARE_PROTOCOL_SHARED_OBJECT_TYPE_LENGTH ) ),
            __packet->data + P2P_PROTOCOLS_SHARE_PROTOCOL_SHARED_OBJECT_TYPE_LENGTH + P2P_PROTOCOLS_SHARE_PROTOCOL_SHARED_COUNT_LENGTH + P2P_PROTOCOLS_SHARE_PROTOCOL_DATA_SIZE_LENGTH,
            *( ( unsigned long long* ) ( __packet->data + P2P_PROTOCOLS_SHARE_PROTOCOL_SHARED_OBJECT_TYPE_LENGTH + P2P_PROTOCOLS_SHARE_PROTOCOL_SHARED_COUNT_LENGTH ) )
        );
    
    return _share_protocol;

}


