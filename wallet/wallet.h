
#ifndef WALLET_WALLET_H
#define WALLET_WALLET_H

#define WALLET_WALLET_WALLET_SETTINGS_NODE_CONNECTIONS_COUNT_LENGTH 1

#include "wallet_definitions.h" // struct End_Point

namespace wallet {


    // Holds all Wallet settings 
    struct Wallet_Settings {

        // Nodes to send new requests ( transactions, etc ... )
        p2p::Connection** node_connections; char node_connections_count;

        ~Wallet_Settings(); Wallet_Settings(); Wallet_Settings( p2p::Connection**, char );

        // Adds a new node connection to the array. 
        // Realloc new array 
        void add_new_node_connection( p2p::Connection* );

        // Removes a node by index and reorganizes the rest of the array 
        // Realloc new array
        void remove_node_connection( char );

        // Run all diferent wallet settings functions
        void run();
        
        // Executes the first option
        void option_1();

        // Gets the total data length
        unsigned long long get_data_length();

        // Returns all the data of this Wallet_Settings
        // malloc() data
        void* get_data();

        // Initialize a Wallet Settings by data
        static Wallet_Settings* get_wallet_settings_by_data( void* );

    };

    // Wallet representation 
    // Holds all needed arguments and all functions
    struct Wallet {

        // Private and public keys of wallet
        unsigned char public_key[ WALLET_PUBLIC_KEY_LENGTH ], private_key[ WALLET_PRIVATE_KEY_LENGTH ];

        // Settings
        Wallet_Settings* settings;

        ~Wallet(); Wallet( unsigned char*, unsigned char*, Wallet_Settings* );

        // Sign a given text with private and public key 
        // Returns a malloc() value with given signature
        unsigned char* sign_text( unsigned char*, unsigned long long );

        // Verify if given signature was sign by this wallet
        // Return true if it is, or false
        bool verify_signature( unsigned char*, unsigned char*, unsigned long long );

        // Infity loop to run the all diferent functions
        void run();

        // Option_1
        void option_1(); 

        // Option_3
        void option_3(); 

        // Save all wallet informations into a file
        void save_into_file( char* );

        // Creates a new wallet key pair
        static Wallet* get_new_wallet();

        // Initialize a wallet with data
        static Wallet* get_wallet_by_data( void* );

        // Initialize a wallet with data from a given file
        static Wallet* get_wallet_by_file( char* );

    };

}

#endif

