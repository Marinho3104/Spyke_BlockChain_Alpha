
#include "socket_functions.h" // accept_new_connection_request();
#include "node_exceptions.h" // struct Unable_Set_Server_Up;
#include "connection.h" // struct Connection;
#include "share_protocol.h"
#include "disconnect_protocol.h" // sturct Disconnect_Protocol;
#include "sync_protocol.h" // Sync Protocol
#include "protocol_ids.h" // Protocol ids
#include "end_point_ipv4.h"
#include "memory_pool.h"
#include "packet.h"
#include "node.h"

#include <iostream>
#include <string.h>
#include <unistd.h>
#include <thread>


p2p::Node_Settings::~Node_Settings() { 
    
    if( server_connection ) server_connection->~Connection(); free( server_connection ); 

    for ( short _ = 0; _ < initial_connections_count; _++ )
        { ( *( initial_connections + _ ) )->~Connection(); free( *( initial_connections + _ ) ); }

    free( initial_connections );
    
}

p2p::Node_Settings::Node_Settings() : server_connection( 0 ), max_server_connections( 0 ), max_ordinary_connections( 0 ), max_stable_connections( 0 ), memory_pool_objects( 0 ), initial_connections( 0 ), initial_connections_count( 0 ) {}

p2p::Node_Settings::Node_Settings(p2p::Connection* __server_connection, unsigned short __max_server_connections, unsigned short __max_ordinary_connections, unsigned long long __memory_pool_object, unsigned short __max_stable_connections, p2p::Connection** __initial_connections, unsigned short __initial_connections_count ) 
    : server_connection( __server_connection ), max_server_connections( __max_server_connections ), max_ordinary_connections( __max_ordinary_connections ), 
        max_stable_connections( __max_stable_connections), memory_pool_objects( __memory_pool_object ), initial_connections( __initial_connections ), initial_connections_count( __initial_connections_count ) {}

void p2p::Node_Settings::print_node_settings_info() {

    std::cout << "\t\t --> Node Settings <--\n" << std::endl;

    std::cout << "\t-> Server Connection: ";

    if ( server_connection ) server_connection->print_representation();

    std::cout << "\n" << std::endl;

    std::cout << "\t-> Max Server Connections: " << max_server_connections << " -> Max Ordinary Connections: " << max_ordinary_connections << " -> Max Stable Connections: " << max_stable_connections << "-> Memory Pool Objects: " << memory_pool_objects << std::endl;

    std::cout << "\n\tInitial Connections (" << initial_connections_count << "):" << std::endl;

    if ( ! initial_connections_count ) std::cout << "\n\t\t --- --- ---" << std::endl;

    else 

        for ( short _ = 0; _ < initial_connections_count; _++ ) 
            { std::cout << "\n\t\t[" << _ << "]: "; ( *( initial_connections + _ ) )->print_representation(); std::cout << std::endl; }

    std::cout << std::endl;

}

void p2p::Node_Settings::run_interface() {
beginning:
    system("clear");

    print_node_settings_info();

    std::cout << "0: Exit" << std::endl;
    std::cout << "1: Change Server Connection" << std::endl;
    std::cout << "2: Change Max Server Connections" << std::endl;
    std::cout << "3: Change Max Ordinary Connections" << std::endl;
    std::cout << "4: Change Max Stable Connections" << std::endl;
    std::cout << "5: Change Memory Pool Objects" << std::endl;
    std::cout << "6: Change Initial Connections" << std::endl;

    std::cout << std::endl;

    std::cout << " --> ";

    std::string _option; std::cin >> _option;

    switch ( *_option.c_str() )
    {
    case '0': break;
    case '1': option_1(); goto beginning; break; 
    case '2': std::cout << "\n\tNew Max Server Connections: "; scanf("%hd", &max_server_connections); goto beginning; break; 
    case '3': std::cout << "\n\tNew Max Ordinary Connections: "; scanf("%hd", &max_ordinary_connections); goto beginning; break; 
    case '4': std::cout << "\n\tNew Max Stable Connections: "; scanf("%hd", &max_stable_connections); goto beginning; break; 
    case '5': std::cout << "\n\tNew Memory Pool Objects: "; scanf("%hd", &memory_pool_objects); goto beginning; break; 
    case '6': option_6(); goto beginning; break;
    default: break;
    }

}

void p2p::Node_Settings::option_1() {

    if ( server_connection ) server_connection->~Connection(); free( server_connection );

    server_connection = 
        p2p::Connection::get_new_connection_manually_hex(2);

}

void p2p::Node_Settings::option_6() {

    std::cout << "\n\t0: Add new connection " << std::endl;
    std::cout << "\t1: Remove a connection " << std::endl;

    std::cout << "\n\t --> ";

    std::string _option; std::cin >> _option; std::cout << std::endl;

    switch ( *_option.c_str() )
    {
    case '0': add_initial_connection( p2p::Connection::get_new_connection_manually_hex(2) ); break;
    case '1': std::cout << "Index to remove: "; std::cin >> _option; remove_initial_connection( atoi( _option.c_str()) );  break; 
    default: break;
    }

}

void p2p::Node_Settings::add_initial_connection( p2p::Connection* __new_connection ) {

    initial_connections = 
        ( p2p::Connection** ) realloc( initial_connections, sizeof( p2p::Connection* ) * ++initial_connections_count );

    *( initial_connections + initial_connections_count - 1 ) = __new_connection;

}

void p2p::Node_Settings::remove_initial_connection( short __index ) {

    for (; __index < initial_connections_count - 1; __index++ ) *( initial_connections + __index ) = *( initial_connections + __index + 1 );

    initial_connections = 
        ( p2p::Connection** ) realloc( initial_connections, sizeof( p2p::Connection* ) * --initial_connections_count );

}

unsigned long long p2p::Node_Settings::get_data_length() {

    unsigned long long _size = 0;
    
    _size += P2P_NODE_NODE_SETTINGS_SERVER_CONNECTION_ENABLE_LENGTH;

    if ( server_connection ) { _size += server_connection->get_data_size(); _size += P2P_NODE_NODE_SETTINGS_MAX_SERVER_CONNECTIONS_LENGTH; }

    _size += P2P_NODE_NODE_SETTINGS_MAX_ORDINARY_CONNECTIONS_LENGTH;

    _size += P2P_NODE_NODE_SETTINGS_MAX_STABLE_CONNECTIONS_LENGTH;

    _size += P2P_NODE_NODE_SETTINGS_MEMORY_POOL_OBJECT_LENGTH;

    _size += P2P_NODE_NODE_SETTINGS_INITIAL_CONNECTIONS_COUNT_LENGTH;

    for ( short _ = 0; _ < initial_connections_count; _++ ) _size += ( *( initial_connections + _ ) )->get_data_size();

    return _size;

}

void* p2p::Node_Settings::get_data() {

    void* _data = 
        malloc( get_data_length() ), *_rtr = _data;

    // Enable server connection
    bool _enable = 
        server_connection;

    memcpy(
        _data,
        &_enable,
        P2P_NODE_NODE_SETTINGS_SERVER_CONNECTION_ENABLE_LENGTH
    ); _data = _data + P2P_NODE_NODE_SETTINGS_SERVER_CONNECTION_ENABLE_LENGTH;

    if ( server_connection ) {
            
        // Server Connection Data
        void* _server_connection_data =
            server_connection->get_data();

        memcpy(
            _data,
            _server_connection_data,
            server_connection->get_data_size()
        ); _data = _data + server_connection->get_data_size();

        free( _server_connection_data );

        memcpy(
            _data,
            &max_server_connections,
            P2P_NODE_NODE_SETTINGS_MAX_SERVER_CONNECTIONS_LENGTH
        ); _data = _data + P2P_NODE_NODE_SETTINGS_MAX_SERVER_CONNECTIONS_LENGTH;

    }

    memcpy(
        _data,
        &max_ordinary_connections,
        P2P_NODE_NODE_SETTINGS_MAX_ORDINARY_CONNECTIONS_LENGTH
    ); _data = _data + P2P_NODE_NODE_SETTINGS_MAX_ORDINARY_CONNECTIONS_LENGTH;

    memcpy(
        _data,
        &max_stable_connections,
        P2P_NODE_NODE_SETTINGS_MAX_STABLE_CONNECTIONS_LENGTH
    ); _data = _data + P2P_NODE_NODE_SETTINGS_MAX_STABLE_CONNECTIONS_LENGTH;

    memcpy(
        _data,
        &memory_pool_objects,
        P2P_NODE_NODE_SETTINGS_MEMORY_POOL_OBJECT_LENGTH
    ); _data = _data + P2P_NODE_NODE_SETTINGS_MEMORY_POOL_OBJECT_LENGTH;

    memcpy(
        _data,
        &initial_connections_count,
        P2P_NODE_NODE_SETTINGS_INITIAL_CONNECTIONS_COUNT_LENGTH
    ); _data = _data + P2P_NODE_NODE_SETTINGS_INITIAL_CONNECTIONS_COUNT_LENGTH;

    for ( short _ = 0; _ < initial_connections_count; _++ ) {

        void * _data_single = 
            ( *( initial_connections + _ ) )->get_data();

        memcpy(
            _data,
            _data_single,
            ( *( initial_connections + _ ) )->get_data_size()
        ); _data = _data + ( *( initial_connections + _ ) )->get_data_size();

        free( _data_single );

    }

    return _rtr;

}

p2p::Node_Settings* p2p::Node_Settings::get_new_node_settings() {

    p2p::Node_Settings* _node_settings = 
        ( p2p::Node_Settings* ) malloc( sizeof( p2p::Node_Settings ) );

    new ( _node_settings ) p2p::Node_Settings();

    return _node_settings;

}

p2p::Node_Settings* p2p::Node_Settings::get_node_settings_by_data( void* __data ) {

    unsigned short _max_server_connections = 0, _max_ordinary_connections = 0, _max_stable_connections = 0, _initial_connections_count = 0;
    unsigned long long _memory_pool_object = 0;
    p2p::Connection** _initial_connections = 0;
    p2p::Connection* _server_connection = 0;
    bool _server_connection_enable;
    
    _server_connection_enable = 
        *( ( bool* ) __data );

    __data = __data + 1;

    if ( _server_connection_enable ) {

        _server_connection = 
            p2p::Connection::get_connection_by_data( __data );

        __data = __data + _server_connection->get_data_size();

        _max_server_connections = 
            *( ( short* ) __data );

        __data = __data + 2;

    }

    _max_ordinary_connections = 
        *( ( unsigned short* ) __data );

    __data = __data + 2;

    _max_stable_connections = 
        *( ( unsigned short* ) __data );

    __data = __data + 2;

    _memory_pool_object = 
        *( ( unsigned long long* ) __data ); 
        
    __data = __data + 8;

    _initial_connections_count = 
        *( ( unsigned short* ) __data ); 
        
    __data = __data + 2;

    _initial_connections = 
        ( p2p::Connection** ) malloc( sizeof( p2p::Connection* ) * _initial_connections_count );

    for ( unsigned short _ = 0; _ < _initial_connections_count; _++ ) {

        *( _initial_connections + _ ) = 
            p2p::Connection::get_connection_by_data( __data );

        __data = __data + _server_connection->get_data_size();

    }

    p2p::Node_Settings* _node_settings = 
        ( p2p::Node_Settings* ) malloc( sizeof( p2p::Node_Settings ) );

    new ( _node_settings ) p2p::Node_Settings(
        _server_connection,
        _max_server_connections,
        _max_ordinary_connections,
        _max_stable_connections,
        _memory_pool_object,
        _initial_connections,
        _initial_connections_count
    );

    return _node_settings;

}




p2p::Node::~Node() { settings->~Node_Settings(); free( settings ); }

p2p::Node::Node( Node_Settings* __settings ) : settings( __settings ), is_running( 0 ), ordinary_connections_count( 0 ), stable_connections_count( 0 ) {

    ordinary_connections = ( p2p::Connection** ) malloc( sizeof( p2p::Connection* ) * settings->max_ordinary_connections ); sem_init( &ordinary_connections_sem, 0, 1 ); 
    for ( unsigned short _ = 0; _ < settings->max_ordinary_connections; _++ ) *( ordinary_connections + _ ) = 0;

    stable_connections = ( p2p::Connection** ) malloc( sizeof( p2p::Connection* ) * settings->max_stable_connections ); sem_init( &stable_connections_sem, 0, 1 ); 
    for ( unsigned short _ = 0; _ < settings->max_stable_connections; _++ ) *( stable_connections + _ ) = 0;

    sem_init( &file_descriptors_sem, 0, 1 ); FD_ZERO( &file_descriptors );

    memory_pool = 
        ( Memory_Pool* ) malloc( sizeof( Memory_Pool* ) );

    new ( memory_pool ) Memory_Pool( settings->memory_pool_objects );


    // Setup all need variables

    // Initialize all need function/variables etc for share protocol execution 
    initialize_share_protocol_cuda();
    
}

void p2p::Node::print_node_info() {

    std::cout << "\t\t --> Node <--\n" << std::endl;

    std::cout << "\t-> Node is running: " << is_running << std::endl;

    std::cout << "\n\t-> Current Ordinary connections: " << ordinary_connections_count << std::endl;
    std::cout << "\t-> Current Stable connections: " << stable_connections_count << std::endl; 

    std::cout << std::endl;

}

bool p2p::Node::add_new_ordinay_connection( p2p::Connection* __new_connection ) {

    sem_wait( &ordinary_connections_sem );

    if ( ordinary_connections_count != settings->max_ordinary_connections ) {

        ordinary_connections_count++;

        for ( unsigned short _ = 0; _ < settings->max_ordinary_connections; _++ )

            if ( ! *( ordinary_connections + _ ) ) { 
                
                *( ordinary_connections + _ ) = __new_connection; add_new_file_descriptor( __new_connection->socket_descriptor ); break;
            
            }

    }

    sem_post( &ordinary_connections_sem );

}

void p2p::Node::remove_ordinary_connection( p2p::Connection* __ordinary_connection ) {

    sem_wait( &ordinary_connections_sem );

    for ( unsigned short _ = 0; _ < settings->max_ordinary_connections; _++ )
    
        if ( *( ordinary_connections + _ ) == __ordinary_connection ) {
            
            *( ordinary_connections + _ ) = 0; ordinary_connections_count --; remove_file_descriptor( __ordinary_connection->socket_descriptor );  break;

        }

    sem_post( &ordinary_connections_sem );

}

void p2p::Node::remove_connection( p2p::Connection* __connection, char __connection_type ) {

    switch ( __connection_type )
    {
    case P2P_NODE_NODE_CONNECTION_TYPE_ORDINARY_CONNECTION: remove_ordinary_connection( __connection ); break;
    default: break;
    }

}

void p2p::Node::add_new_file_descriptor( int __file_descriptor ) {

    sem_wait( &file_descriptors_sem ); FD_SET( __file_descriptor, &file_descriptors ); sem_post( &file_descriptors_sem );

}

void p2p::Node::remove_file_descriptor( int __file_descriptor ) {

    sem_wait( &file_descriptors_sem ); FD_CLR( __file_descriptor, &file_descriptors ); sem_post( &file_descriptors_sem );

}

void p2p::Node::run() {

    // Executes a connection to all initial connections in settings
    for ( unsigned short _ = 0; _ < settings->initial_connections_count; _++ ) 
        
        if ( ( *( settings->initial_connections + _ ) )->connect() )

            add_new_ordinay_connection( *( settings->initial_connections + _ ) );

    // Set the server up
    if ( settings->server_connection ) {

        std::cout << "Set the Server Up ... ";

        if (
            ! settings->server_connection->set_server( settings->max_server_connections )
        ) throw Unable_Set_Server_Up();

        add_new_file_descriptor( settings->server_connection->socket_descriptor );

        std::cout << "Done" << std::endl;

        std::thread(
            & p2p::Node::monitor_open_descriptors,
            this
        ).detach();

    }

    is_running = 1;

    if ( *ordinary_connections ) {

        p2p::Sync_Protocol* _sync_protocol = 
            ( p2p::Sync_Protocol* ) malloc( sizeof( p2p::Sync_Protocol ) );

        void* _data = 
            malloc( 8 );

        for ( int _ = 0; _ < 8; _++ ) *( ( char* ) ( _data + _ ) ) = 0;

        new ( _sync_protocol ) p2p::Sync_Protocol(
            0, 8, _data
        );

        p2p::Packet* _packet =  
            _sync_protocol->get_packet();

        (*ordinary_connections)->allocate_data( 8 + 1 );

        char _ = P2P_PROTOCOLS_SYNC_PROTOCOL_STATE_DATA;

        unsigned long long __ = 0;

        memcpy(
            (*ordinary_connections)->data,
            &_,
            1
        );    

        memcpy(
            (*ordinary_connections)->data + 1,
            &__,
            8
        );

        (*ordinary_connections)->send_packet( _packet ); 

    }

}

void p2p::Node::monitor_open_descriptors() {

    while ( 1 ) {

        fd_set rd_fd = file_descriptors;

        if (
            select(
                FD_SETSIZE,
                &rd_fd,
                NULL,
                NULL,
                NULL
            ) < 0 
        ) break;

        // Check if is a new connection request
        if ( FD_ISSET( settings->server_connection->socket_descriptor, &rd_fd ) ) accept_new_connections_requests();

        // Check if receive data from ordinary_connections
        for ( unsigned short _ = 0; _ < settings->max_ordinary_connections; _++ )

            if ( 
                *( ordinary_connections + _ ) && 
                FD_ISSET( ( *( ordinary_connections + _ ) )->socket_descriptor, &rd_fd )  
            ) handle_packet( *( ordinary_connections + _ ), P2P_NODE_NODE_CONNECTION_TYPE_ORDINARY_CONNECTION );

        // Check if receive data from stable_connections
        for ( unsigned short _ = 0; _ < settings->max_stable_connections; _++ )

            if ( 
                *( stable_connections + _ ) && 
                FD_ISSET( ( *( stable_connections + _ ) )->socket_descriptor, &rd_fd )  
            ) handle_packet( *( stable_connections + _ ), P2P_NODE_NODE_CONNECTION_TYPE_STABLE_CONNECTION );


    }

}

void p2p::Node::accept_new_connections_requests() {

    p2p::Connection* _accepted_connection =  
        p2p::accept_new_connection_request( settings->server_connection->socket_descriptor );

    if ( _accepted_connection ) 

        add_new_ordinay_connection(
            _accepted_connection
        );

}

void p2p::Node::handle_packet( p2p::Connection* __connection, char __connection_type ) {

    p2p::Packet* _packet_recv = 
        p2p::get_packet_from( __connection->socket_descriptor );

    if ( ! _packet_recv ) { remove_connection( __connection, __connection_type ); return; }

    switch ( _packet_recv->protocol_id )
    {

        case P2P_PROTOCOLS_PROTOCOL_ID_SYNC_PROTOCOL: 

            {

                p2p::Sync_Protocol* _sync_protocol = 
                    p2p::Sync_Protocol::get_sync_protocol_from_packet( _packet_recv );

                _sync_protocol->handle( __connection ); break;
            
            }

        case P2P_PROTOCOLS_PROTOCOL_ID_SHARE_PROTOCOL:

            {

                p2p::Share_Protocol* _share_protocol = 
                    p2p::Share_Protocol::get_share_protocol_from_packet( _packet_recv );

                _share_protocol->handle( __connection ); break;

            }

        case P2P_PROTOCOLS_PROTOCOL_ID_DISCONNECT_PROTOCOL: default: remove_connection( __connection, __connection_type ); break;
    
    }

}

void p2p::Node::run_interface() {

    while( 1 ) {

        system("clear");

        print_node_info();

        std::cout << std::endl;

        std::cout << "0: Exit" << std::endl;
        std::cout << "1: Node Settings Menu" << std::endl;
        std::cout << "2: Save Node info into a file" << std::endl;
        if ( ! is_running ) std::cout << "3: Run Node" << std::endl;

        std::cout << std::endl;

        std::cout << " --> ";

        std::string _option; std::cin >> _option;

        std::cout << std::endl;

        switch ( *_option.c_str() )
        {
        case '0': goto out; break;
        case '1': settings->run_interface(); break;
        case '2': option_2(); break;
        case '3': run(); break;
        
        default: break;
        }

    }

    out:;

}

void p2p::Node::option_2() {

    std::cout << "\tPath of file: ";
    std::string _path; std::cin >> _path;

    save_into_file( ( char* ) _path.c_str() );

}

void p2p::Node::save_into_file( char* __path ) {

    FILE* _node_file = 
        fopen( __path, "wb" );

    void* _node_settings_data = 
        settings->get_data();

    fwrite(
        _node_settings_data,
        settings->get_data_length(), 1,
        _node_file
    );

    free( _node_settings_data );

    fclose( _node_file );

}

p2p::Node* p2p::Node::get_node_by_data( void* __data ) {

    Node* _node = 
        ( Node* ) malloc( sizeof( Node ) );

    new ( _node ) Node(
        Node_Settings::get_node_settings_by_data( __data )
    );

    return _node;

} 

p2p::Node* p2p::Node::get_node_by_file( char* __path ) {

    FILE* _node_file =
        fopen( __path, "rb" );

    fseek( _node_file, 0, SEEK_END );

    long _size = ftell( _node_file );

    fseek( _node_file, 0, SEEK_SET );

    void* _data = 
        malloc( _size );

    fread(
        _data,
        _size, 1,
        _node_file
    );

    fclose( _node_file );

    Node* _node = 
        Node::get_node_by_data(
            _data
        );

    free( _data );

    return _node;

}




