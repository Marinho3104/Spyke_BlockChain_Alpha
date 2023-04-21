
#include "utils_functions.h" // convert_bytes_hex()
#include "ed25519.h" // Ed25519 cryptography    
#include "end_point_ipv4.h" // End Point IP v4
#include "end_point_ipv6.h" // End Point IP v6
#include "wallet.h"

#include <unistd.h>
#include <iostream>
#include <string.h>

wallet::Wallet_Settings::~Wallet_Settings() { free( node_connections ); }

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

        std::cout << "\t\tI am a connection" << std::endl;

    }

    std::cout << std::endl;

    std::cout << "\t0: Add new End Point" << std::endl;
    std::cout << "\t1: Remove a Connection" << std::endl;

    std::cout << "\n\t --> ";

    std::string _option; std::cin >> _option;

    switch ( *_option.c_str() )
    {
    case '0': 

        {

            std::cout << std::endl;
    
            std::cout << "\t\tIp version [ 4 / 6 ]: ";
            std::string _version; std::cin >> _version;

            std::cout << "\t\tAddress: 0x";
            std::string _address; std::cin >> _address;

            std::cout << "\t\tPort: ";
            std::string _port; std::cin >> _port;

            if ( *_version.c_str() == '6' ) {

                p2p::End_Point_IPv6* _end_point = 
                    ( p2p::End_Point_IPv6* ) malloc( sizeof( p2p::End_Point_IPv6 ) );

                new ( _end_point ) p2p::End_Point_IPv6(
                    ( void* ) _address.c_str(), atoi( _port.c_str() )
                );

                add_new_node_connection(
                    _end_point->get_connection()
                );

            }

            else {

                p2p::End_Point_IPv4* _end_point = 
                    ( p2p::End_Point_IPv4* ) malloc( sizeof( p2p::End_Point_IPv4 ) );

                new ( _end_point ) p2p::End_Point_IPv4(
                    ( void* ) _address.c_str(), atoi( _port.c_str() )
                );

                add_new_node_connection(
                    _end_point->get_connection()
                );

            }
            
        }

        break;

    case '1': std::cout << "Remove" << std::endl; break; 
    default: return; break;
    }

    std::string _stop;

    std::cin >> _stop;

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

void wallet::Wallet::run() {

    std::string _option;

    while( 1 ) {

        system("clear");

        std::cout << "\n\n\t\t --> Wallet <--\n\n" << std::endl;

        std::cout << "0: Exit" << std::endl;
        std::cout << "1: Get Key pair information" << std::endl;
        std::cout << "2: Wallet Settings Menu" << std::endl;

        std::cout << "\n --> "; std::cin >> _option;

        switch ( *_option.c_str() ) {

            case '0': goto out;
            case '1': option_1(); break;
            case '2': settings->run(); break;

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



void wallet::Wallet::save_into_file( char* __path ) {



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


