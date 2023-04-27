

#include "utils_functions.h"
#include "sync_protocol.h"
#include "protocol_ids.h"
#include "connection.h"
#include "packet.h"
#include "block.h"

#include <iostream>
#include <string.h>

p2p::Sync_Protocol::~Sync_Protocol() {} p2p::Sync_Protocol::Sync_Protocol() {}

p2p::Sync_Protocol::Sync_Protocol( unsigned char __state, unsigned long long __data_size, void* __data ) 
    : state( __state ), data_size( __data_size ), data( __data ) {}

void p2p::Sync_Protocol::handle( p2p::Connection* __connection ) {

    p2p::Packet* _packet_to_send = 0;

    switch ( state )
    {
    case P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_REQUEST:

        std::cout << "Sync Protocol State Request" << std::endl;

        if ( data_size != TYPES_BLOCK_BLOCK_CREATION_TIME_LENGTH ) {

            p2p::Sync_Protocol _response_sync_protocol = 
                p2p::Sync_Protocol( P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_DONE, 0, 0 );

            _packet_to_send = 
                _response_sync_protocol.get_packet();

            free( __connection->data ); __connection->data = 0;

        }

        else {

            long _data_size;

            char* _file_name = 
                ( char* ) malloc( TYPES_BLOCK_BLOCK_CREATION_TIME_LENGTH + 1 );

            memcpy( _file_name, data, TYPES_BLOCK_BLOCK_CREATION_TIME_LENGTH ); _file_name[ TYPES_BLOCK_BLOCK_CREATION_TIME_LENGTH ] = '\0';

            void* _data =
                utils::get_file_data( _file_name, _data_size );

            if ( ! _data ) {

                p2p::Sync_Protocol _response_sync_protocol = 
                p2p::Sync_Protocol( P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_DONE, 0, 0 );

                _packet_to_send = 
                    _response_sync_protocol.get_packet();

                free( __connection->data ); __connection->data = 0; break;

            }

            free( _file_name );

            p2p::Sync_Protocol _response_sync_protocol = 
                p2p::Sync_Protocol( P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_DATA, 0, 0 );

            _packet_to_send = 
                _response_sync_protocol.get_packet();

            free( __connection->data );

            __connection->data = 
                malloc( P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_LENGTH + TYPES_BLOCK_BLOCK_CREATION_TIME_LENGTH );

            char _expected_state = P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_DATA;

            memcpy( _data, &_expected_state, P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_LENGTH );

            memcpy( _data + P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_LENGTH, &data, TYPES_BLOCK_BLOCK_CREATION_TIME_LENGTH );

        }

        break;

    case P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_DATA:

        std::cout << "Sync Protocol State Data" << std::endl;

        break;

    case P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_DONE:

        std::cout << "Sync Protocol State Done" << std::endl; return; break;

    default: break;
    }

    __connection->send_packet( _packet_to_send );

    _packet_to_send->~Packet(); free( _packet_to_send );

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

    if ( data_size )

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

