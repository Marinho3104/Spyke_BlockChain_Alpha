
#include "utils_functions.h" // convert_bytes_hex()
#include "ed25519.h" // Ed25519 cryptography    
#include "end_point_ipv4.h" // End Point IP v4
#include "end_point_ipv6.h" // End Point IP v6
#include "connection.h" // struct Conneciton;
#include "socket_exceptions.h" // struct Unknow_Connection_Version_Exception;
#include "transaction.h"
#include "socket_functions.h"
#include "share_protocol.h"
#include "wallet.h"

#include <unistd.h>
#include <iostream>
#include <string.h>

wallet::Wallet_Settings::~Wallet_Settings() {  
    
    for ( char _ = 0; _ < node_connections_count; _++ ) {
        
        ( *( node_connections + _ ) )->~Connection(); free( *( node_connections + _) ); 

    }
        
    free( node_connections ); 
    
}

wallet::Wallet_Settings::Wallet_Settings() : node_connections( 0 ), node_connections_count( 0 ) {}

wallet::Wallet_Settings::Wallet_Settings( p2p::Connection** __node_connections, char __node__connections_count ) 
    : node_connections( __node_connections ), node_connections_count( __node__connections_count ) {}

void wallet::Wallet_Settings::add_new_node_connection( p2p::Connection* __new_connection ) {

    // Reallocates the memory with additional pointer
    node_connections = 
        ( p2p::Connection** ) realloc(
            node_connections,
            sizeof( p2p::Connection* ) * ++node_connections_count
        );

    // Copies the pointer into the new address
    *( node_connections + node_connections_count - 1 ) = __new_connection;

}

void wallet::Wallet_Settings::remove_node_connection( char __index ) {

    p2p::Connection* _to_remove = 
        *( node_connections + __index );

    _to_remove->~Connection(); free( _to_remove );

    for ( char _ = __index; _ < node_connections_count - 1; _++ ) *( node_connections + _ ) = *( node_connections + _ + 1 );

    node_connections = ( p2p::Connection** ) realloc( node_connections, sizeof( p2p::Connection* ) * --node_connections_count );

}

void wallet::Wallet_Settings::run() {

    std::string _option;

    system("clear");

    std::cout << "\n\n\t\t --> Wallet Settings <--\n\n" << std::endl;
    std::cout << "0: Exit" << std::endl;
    std::cout << "1: Modify Node Connections" << std::endl;

    std::cout << "\n --> "; std::cin >> _option;

    switch ( *_option.c_str() ) {

        case '0': break;
        case '1': option_1(); break;

        default: break;

    }

}

void wallet::Wallet_Settings::option_1() {

    std::cout << std::endl;

    std::cout << "\tCurrent Node Connections:\n" << std::endl;

    for ( char _ = 0; _ < node_connections_count; _++ ) {

        std::cout << "\t\t[" << ( int ) _ << "]: ";

        ( *( node_connections + _ ) )->print_representation(); std::cout << std::endl;

    }

    std::cout << std::endl;

    std::cout << "\t0: Add new End Point" << std::endl;
    std::cout << "\t1: Remove a Connection" << std::endl;

    std::cout << "\n\t --> ";

    std::string _option; std::cin >> _option;

    switch ( *_option.c_str() )
    {
    case '0': 
    
        add_new_node_connection(
            p2p::Connection::get_new_connection_manually_hex( 0 )
        );

        break;

    case '1': 

        {
            std::cout << std::endl;
            std::cout << "\t\tIndex to remove: ";
            std::string _index; std::cin >> _index;

            remove_node_connection( atoi( _index.c_str() ) ); 

        }

        break; 

    default: return; break;
    }

    std::string _stop;

    std::cin >> _stop;

}

unsigned long long wallet::Wallet_Settings::get_data_length() {

    unsigned long long _length = WALLET_WALLET_WALLET_SETTINGS_NODE_CONNECTIONS_COUNT_LENGTH;

    for ( unsigned long long _ = 0; _ < node_connections_count; _++ ) _length += ( *( node_connections + _ ) )->get_data_size();

    return _length;

}

void* wallet::Wallet_Settings::get_data() {

    void* _data = malloc( get_data_length() ), * _connection_data, *_rtr = _data;

    memcpy(
        _data,
        &node_connections_count,
        WALLET_WALLET_WALLET_SETTINGS_NODE_CONNECTIONS_COUNT_LENGTH
    ); _data = _data + WALLET_WALLET_WALLET_SETTINGS_NODE_CONNECTIONS_COUNT_LENGTH;

    for ( unsigned long long _ = 0; _ < node_connections_count; _++ ) {

        _connection_data = ( *( node_connections + _  ) )->get_data();

        memcpy(
            _data,
            _connection_data,
            ( *( node_connections + _  ) )->get_data_size()
        );

        free( _connection_data );

        _data = _data + ( *( node_connections + _  ) )->get_data_size();

    }

    return _rtr;

}

wallet::Wallet_Settings* wallet::Wallet_Settings::get_wallet_settings_by_data( void* __data ) {

    char _node_connections_count = *( ( char* ) __data ); __data = __data + 1;

    p2p::Connection** _node_connections = 
        ( p2p::Connection** ) malloc( sizeof( p2p::Connection* ) * _node_connections_count );

    for ( char _ = 0; _ < _node_connections_count; _++ ) {

        ( *( _node_connections + _ ) ) = 
            p2p::Connection::get_connection_by_data( __data );

        __data = __data + ( *( _node_connections + _ ) )->get_data_size();

    }

    wallet::Wallet_Settings* _wallet_settings = 
        ( wallet::Wallet_Settings* ) malloc( sizeof( wallet::Wallet_Settings ) );

    new ( _wallet_settings ) 
        wallet::Wallet_Settings(
            _node_connections,
            _node_connections_count
        );

    return _wallet_settings;

}





wallet::Wallet::~Wallet() { settings->~Wallet_Settings(); free( settings ); }

wallet::Wallet::Wallet( unsigned char* __private_key, unsigned char* __public_key, Wallet_Settings* __settings ) 
    : settings( __settings ) {

        memcpy(
            private_key,
            __private_key,
            WALLET_PRIVATE_KEY_LENGTH
        );

        memcpy(
            public_key,
            __public_key,
            WALLET_PUBLIC_KEY_LENGTH
        );

    }

unsigned char* wallet::Wallet::sign_text( unsigned char* __text_to_sign, unsigned long long __size ) {

    unsigned char* _signature = 
        ( unsigned char* ) malloc( WALLET_SIGNATURE_LENGTH );

    // Performs the signature
    ed25519_sign(
        _signature,
        __text_to_sign,
        __size,
        public_key,
        private_key
    );

    return _signature;
 
}

bool wallet::Wallet::verify_signature( unsigned char* __signature, unsigned char* __original_text, unsigned long long __original_text_size ) {

    // Performs signature verification
    return 
        ed25519_verify(
            __signature,
            __original_text,
            __original_text_size,
            public_key
        );

}

void wallet::Wallet::sign_transaction( void* __transaction_data ) {

    void* _data_to_sign = 
        __transaction_data + WALLET_SIGNATURE_LENGTH;

    unsigned char* _signature = 
        sign_text(
            ( unsigned char* ) _data_to_sign,
            TYPES_TRANSACTION_TRANSACTION_FULL_SIZE - TYPES_TRANSACTION_TRANSACTION_FROM_BALANCE_LENGTH - TYPES_TRANSACTION_TRANSACTION_TO_BALANCE_LENGTH -
                WALLET_SIGNATURE_LENGTH
        );

    std::cout << "Transaction Verification: " << 
        verify_signature( _signature, ( unsigned char* ) _data_to_sign, TYPES_TRANSACTION_TRANSACTION_FULL_SIZE - TYPES_TRANSACTION_TRANSACTION_FROM_BALANCE_LENGTH - TYPES_TRANSACTION_TRANSACTION_TO_BALANCE_LENGTH - WALLET_SIGNATURE_LENGTH )
            << std::endl;

    memcpy(
        __transaction_data,
        _signature,
        WALLET_SIGNATURE_LENGTH
    );

    free( _signature );

}

void wallet::Wallet::sign_block_part( void* __block_part_data, unsigned long long __block_part_sign_data_size ) {

    void* _data_to_sign = 
        __block_part_data + WALLET_SIGNATURE_LENGTH;

    unsigned char* _signature = 
        sign_text(
            ( unsigned char* ) _data_to_sign,
            __block_part_sign_data_size
        );

    std::cout << "Block Part Verification: " << 
        verify_signature( _signature, ( unsigned char* ) _data_to_sign, __block_part_sign_data_size )
            << std::endl;

    memcpy(
        __block_part_data,
        _signature,
        WALLET_SIGNATURE_LENGTH
    );

}

void wallet::Wallet::run() {

    std::string _option;

    while( 1 ) {

        system("clear");

        std::cout << "\n\n\t\t --> Wallet <--\n\n" << std::endl;

        std::cout << "0: Exit" << std::endl;
        std::cout << "1: Get Key pair information" << std::endl;
        std::cout << "2: Wallet Settings Menu" << std::endl;
        std::cout << "3: Save wallet info into a file" << std::endl;
        std::cout << "4: Create, sign and send Transaction" << std::endl;

        std::cout << "\n --> "; std::cin >> _option;

        switch ( *_option.c_str() ) {

            case '0': goto out;
            case '1': option_1(); break;
            case '2': settings->run(); break;
            case '3': option_3(); break;
            case '4': option_4(); break;

            default: break;

        }

    }

    out:;

}

void wallet::Wallet::option_1() {

    unsigned char* _private_key_hexadecimal = 
        ( unsigned char* ) utils::convert_bytes_hex( private_key, WALLET_PRIVATE_KEY_LENGTH );

    unsigned char* _public_key_hexadecimal = 
        ( unsigned char* ) utils::convert_bytes_hex( public_key, WALLET_PUBLIC_KEY_LENGTH );

    std::cout << std::endl;

    std::cout << "\tPrivate Key:\n" << std::endl;
    std::cout << "\t\t0x" << _private_key_hexadecimal << std::endl;
    std::cout << "\t\t[ "; 
    for ( int _ = 0; _ < WALLET_PRIVATE_KEY_LENGTH; _++ ) std::cout << ( int ) private_key[ _ ] << " "; 
    std::cout << "]" << std::endl;
    std::cout << std::endl;

    std::cout << "\tPublic Key:\n" << std::endl;
    std::cout << "\t\t0x" << _public_key_hexadecimal << std::endl;
    std::cout << "\t\t[ "; 
    for ( int _ = 0; _ < WALLET_PUBLIC_KEY_LENGTH; _++ ) std::cout << ( int ) public_key[ _ ] << " "; 
    std::cout << "]" << std::endl;

    free( _private_key_hexadecimal ); free( _public_key_hexadecimal );

    std::string _stop;

    std::cin >> _stop;

} 

void wallet::Wallet::option_3() {

    std::cout << std::endl;

    std::cout << "\tPath of file: ";

    std::string _path; std::cin >> _path;

    save_into_file( ( char* ) _path.c_str() );

} 

void wallet::Wallet::option_4() {

    static unsigned long long _nonce = 0;

    unsigned char _to[ WALLET_PUBLIC_KEY_LENGTH ] = { 0 };
    unsigned long long _amount = 100, _fee = 1; 

    types::Transaction _transaction = 
        types::Transaction(
            public_key,
            _to,
            _amount, _fee, _nonce++
        );

    p2p::Share_Protocol _shared_protocol = 
        p2p::Share_Protocol(
            0, 1, &_transaction, TYPES_TRANSACTION_TRANSACTION_FULL_SIZE
        );
    
    p2p::Packet* _packet = 
        _shared_protocol.get_packet();

    for ( int _ = 0; _ < settings->node_connections_count; _++ ) {

        if ( ! ( *( settings->node_connections + _ ) )->connect() ) continue;

        ( *( settings->node_connections + _ ) )->send_packet( _packet );

        p2p::disconnect_from( *( settings->node_connections + _ ) );

        break;

    }


}



void wallet::Wallet::save_into_file( char* __path ) {

    FILE* _wallet_file = 
        fopen( __path, "wb" );

    fwrite(
        private_key,
        WALLET_PRIVATE_KEY_LENGTH, 1,
        _wallet_file
    );

    fwrite(
        public_key,
        WALLET_PUBLIC_KEY_LENGTH, 1,
        _wallet_file
    );

    void* _wallet_settings_data = 
        settings->get_data();

    fwrite(
        _wallet_settings_data,
        settings->get_data_length(), 1,
        _wallet_file
    );

    free( _wallet_settings_data );

    fclose( _wallet_file );

}

wallet::Wallet* wallet::Wallet::get_new_wallet() {

    // Keys pairs allocation 
    unsigned char _private_key[ WALLET_PRIVATE_KEY_LENGTH ], _public_key[ WALLET_PUBLIC_KEY_LENGTH ], _seed[32];

    // Set seed for the key pairs generation
    ed25519_create_seed(_seed); 

    // Generate key pairs    
    ed25519_create_keypair(_public_key, _private_key, _seed);

    // Generate a default Wallet_Settings
    wallet::Wallet_Settings* _wallet_settings = 
        ( wallet::Wallet_Settings* ) malloc( sizeof( wallet::Wallet_Settings ) );

    new ( _wallet_settings ) wallet::Wallet_Settings();    

    wallet::Wallet* _wallet = 
        ( wallet::Wallet* ) malloc( sizeof( wallet::Wallet ) );

    new ( _wallet ) wallet::Wallet(
        _private_key,
        _public_key,
        _wallet_settings
    );

    return _wallet;

}

wallet::Wallet* wallet::Wallet::get_wallet_by_data( void* __data ) {

    unsigned char private_key[ WALLET_PRIVATE_KEY_LENGTH ], public_key[ WALLET_PUBLIC_KEY_LENGTH ];

    memcpy(
        private_key,
        __data,
        WALLET_PRIVATE_KEY_LENGTH
    ); 

    memcpy(
        public_key,
        __data + WALLET_PRIVATE_KEY_LENGTH,
        WALLET_PUBLIC_KEY_LENGTH
    ); 

    wallet::Wallet* _wallet = 
        ( wallet::Wallet* ) malloc( sizeof( wallet::Wallet ) );

    new ( _wallet ) 
        wallet::Wallet(
            private_key, public_key,
            wallet::Wallet_Settings::get_wallet_settings_by_data( __data + WALLET_PRIVATE_KEY_LENGTH + WALLET_PUBLIC_KEY_LENGTH )
        );

    return _wallet;


}

wallet::Wallet* wallet::Wallet::get_wallet_by_file( char* __file_path ) {

    FILE* _wallet_file = 
        fopen( __file_path, "rb" );

    fseek( _wallet_file, 0L, SEEK_END );

    long _file_size = ftell( _wallet_file );

    fseek( _wallet_file, 0, SEEK_SET );

    void* _wallet_file_data = malloc( _file_size );

    fread(
        _wallet_file_data,
        _file_size, 1,
        _wallet_file
    );

    fclose( _wallet_file );

    wallet::Wallet* _wallet =   
        wallet::Wallet::get_wallet_by_data( _wallet_file_data );

    free( _wallet_file_data );

    return _wallet;

}

