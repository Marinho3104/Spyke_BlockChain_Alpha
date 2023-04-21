
#include "utils_functions.h" // convert_bytes_hex()
#include "ed25519.h" // Ed25519 cryptography    
#include "wallet.h"

#include <unistd.h>
#include <iostream>
#include <string.h>

wallet::Wallet_Settings::~Wallet_Settings() {}

wallet::Wallet_Settings::Wallet_Settings() {}





wallet::Wallet::~Wallet() {}

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

        std::cout << " --> "; std::cin >> _option;

        switch ( *_option.c_str() ) {

            case '0': goto out;
            case '1': option_1(); break;

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

    wallet::Wallet* _wallet = 
        ( wallet::Wallet* ) malloc( sizeof( wallet::Wallet ) );

    new ( _wallet ) wallet::Wallet(
        _private_key,
        _public_key,
        0
    );

    return _wallet;

}


