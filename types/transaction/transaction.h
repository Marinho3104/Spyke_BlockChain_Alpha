

#ifndef TYPES_TRANSACTION_TRANSACTION_H
#define TYPES_TRANSACTION_TRANSACTION_H

#include "wallet_definitions.h"

#define TYPES_TRANSACTION_TRANSACTION_FULL_SIZE 168

namespace types {

    struct Transaction {

        unsigned char signature[ WALLET_SIGNATURE_LENGTH ];

        unsigned char from[ WALLET_PUBLIC_KEY_LENGTH ], to[ WALLET_PUBLIC_KEY_LENGTH ];

        unsigned long long amount, fee;

        unsigned long long nonce;

        unsigned long long from_balance, to_balance;

        ~Transaction(); 
        
        Transaction( unsigned char*, unsigned char*, unsigned char*, unsigned long long, unsigned long long, unsigned long long, unsigned long long, unsigned long long );

        Transaction( unsigned char*, unsigned char*, unsigned long long, unsigned long long, unsigned long long );

    };

}

#endif
