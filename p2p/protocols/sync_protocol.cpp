

#include "block_chain_rules.h"
#include "socket_functions.h"
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

p2p::Packet* p2p::Sync_Protocol::handle_P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_REQUEST( p2p::Connection* __connection ) {

    std::cout << "Sync Protocol State Request" << std::endl;

    // Checks if connection data match with current connection
    if ( __connection->data ) {

        handle_P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_REQUEST_end_communication:
        // Here the given data is wrong 
        // So it ends the protocol communication

        p2p::Sync_Protocol _response_sync_protocol = 
                    p2p::Sync_Protocol( P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_DONE, 0, 0 );

        // Reset the connection data
        free( __connection->data ); __connection->data = 0;

        return _response_sync_protocol.get_packet();

    }

    // Checks if data given is 8 bytes ( creation time )
    if ( data_size != TYPES_BLOCK_BLOCK_CREATION_TIME_LENGTH ) goto handle_P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_REQUEST_end_communication;

    // Get block information
    types::Block* _block =
        types::Block::get_by_creation_time( ( unsigned long long* ) data );
        
    // If block does not exists it ends the communication
    if ( ! _block ) goto handle_P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_REQUEST_end_communication;

    // Creates the protocol communication back
    p2p::Sync_Protocol _response_sync_protocol = 
        p2p::Sync_Protocol( 
            P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_DATA, 
            types::Block::get_data_representation_length( _block->block_parts_count, &_block->block_parts ), 
            _block
        );

    p2p::Packet* _response_packet = 
        _response_sync_protocol.get_packet();

    // Updates the connection data
    free( __connection->data );

    __connection->allocate_data( P2P_PROTOCOLS_SYNC_EXPECTED_CONNECTION_DATA_SIZE );

    char _expected_state = P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_DATA_RECEIVED; // Expected state to receive
    unsigned long long _expected_creation_time = _block->creation_time + block_chain::block_generation_time; // Next block generation time TODO 

    memcpy( __connection->data + P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_LENGTH, &_expected_creation_time, TYPES_BLOCK_BLOCK_CREATION_TIME_LENGTH );
    memcpy( __connection->data, &_expected_state, P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_LENGTH );

    // Free block data
    free( _block );

    return _response_packet;

}

p2p::Packet* p2p::Sync_Protocol::handle_P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_DATA( p2p::Connection* __connection ) {

    std::cout << "Sync Protocol State Data" << std::endl;

    if ( __connection->data_size != P2P_PROTOCOLS_SYNC_EXPECTED_CONNECTION_DATA_SIZE || *( ( char* ) __connection->data ) != P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_DATA ) {

        handle_P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_DATA_end_communication:
        // Here the given data is wrong 
        // So it ends the protocol communication

        p2p::Sync_Protocol _response_sync_protocol = 
                    p2p::Sync_Protocol( P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_DONE, 0, 0 );

        // Reset the connection data
        free( __connection->data ); __connection->data = 0;

        return _response_sync_protocol.get_packet();

    }

    // Get block by given data
    types::Block* _block = 
        types::Block::get_by_data( data, data_size );

    // If given data is not correct
    if ( ! _block ) goto handle_P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_DATA_end_communication;

    if ( _block->creation_time != *( ( unsigned long long* ) ( __connection->data + P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_LENGTH ) ) ) 
        goto handle_P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_DATA_end_communication; 

    // Calls gpu thread to verify transactions data
    // TODO

    // Saves the block
    _block->save_block();

    // Creates the protocol communication back
    p2p::Sync_Protocol _response_sync_protocol = 
        p2p::Sync_Protocol( 
            P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_DATA_RECEIVED, 
            0, 0
        );

    p2p::Packet* _response_packet = 
        _response_sync_protocol.get_packet();

    // Updates the connection data
    free( __connection->data );

    __connection->allocate_data( P2P_PROTOCOLS_SYNC_EXPECTED_CONNECTION_DATA_SIZE );

    char _expected_state = P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_DATA; // Expected state to receive
    unsigned long long _expected_creation_time = _block->creation_time + block_chain::block_generation_time; // Next block generation time TODO 

    memcpy( __connection->data + P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_LENGTH, &_expected_creation_time, TYPES_BLOCK_BLOCK_CREATION_TIME_LENGTH );
    memcpy( __connection->data, &_expected_state, P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_LENGTH );

    return _response_packet;

}

p2p::Packet* p2p::Sync_Protocol::handle_P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_DATA_RECEIVED( p2p::Connection* __connection ) {

    std::cout << "Sync Protocol State Data Received" << std::endl;

    // Checks if connection data match with current connection
    if ( __connection->data_size != P2P_PROTOCOLS_SYNC_EXPECTED_CONNECTION_DATA_SIZE || *( ( char* ) __connection->data ) != P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_DATA_RECEIVED ) {

        handle_P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_DONE_end_communication:
        // Here the given data is wrong 
        // So it ends the protocol communication

        p2p::Sync_Protocol _response_sync_protocol = 
                    p2p::Sync_Protocol( P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_DONE, 0, 0 );

        // Reset the connection data
        free( __connection->data ); __connection->data = 0;

        return _response_sync_protocol.get_packet();

    }

    // Get block information
    types::Block* _block =
        types::Block::get_by_creation_time( ( unsigned long long* ) ( __connection->data + P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_LENGTH ) );

    if ( ! _block ) goto handle_P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_DONE_end_communication;

    // Creates the protocol communication back
    p2p::Sync_Protocol _response_sync_protocol = 
        p2p::Sync_Protocol( 
            P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_DATA, 
            types::Block::get_data_representation_length( _block->block_parts_count, &_block->block_parts ), 
            _block
        );

    p2p::Packet* _response_packet = 
        _response_sync_protocol.get_packet();

    // Updates the connection data
    free( __connection->data );

    __connection->allocate_data( P2P_PROTOCOLS_SYNC_EXPECTED_CONNECTION_DATA_SIZE );

    char _expected_state = P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_DATA_RECEIVED; // Expected state to receive
    unsigned long long _expected_creation_time = _block->creation_time + block_chain::block_generation_time; // Next block generation time TODO 

    memcpy( __connection->data + P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_LENGTH, &_expected_creation_time, TYPES_BLOCK_BLOCK_CREATION_TIME_LENGTH );
    memcpy( __connection->data, &_expected_state, P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_LENGTH );

    // Free block data
    free( _block );

    return _response_packet;

}

void p2p::Sync_Protocol::handle_P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_DONE( p2p::Connection* __connection ) {

    std::cout << "Sync Protocol State Done" << std::endl; free( __connection->data ); __connection->data = 0; 

}

void p2p::Sync_Protocol::handle( p2p::Connection* __connection ) {

    p2p::Packet* _packet_to_send = 0;

    switch ( state )
    {
        
        case P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_REQUEST: _packet_to_send = handle_P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_REQUEST( __connection ); break;
        case P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_DATA: _packet_to_send = handle_P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_DATA( __connection ); break;
        case P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_DATA_RECEIVED: _packet_to_send = handle_P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_DATA_RECEIVED( __connection ); break;
        case P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_DONE: handle_P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_DONE( __connection ); break;
        default: p2p::disconnect_from( __connection ); break;
    
    }

    if ( _packet_to_send ) {

        __connection->send_packet( _packet_to_send );

        _packet_to_send->~Packet(); free( _packet_to_send );

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

