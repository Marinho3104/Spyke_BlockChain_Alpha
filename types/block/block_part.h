

#ifndef TYPES_BLOCK_BLOCK_PART_H
#define TYPES_BLOCK_BLOCK_PART_H

#include "wallet_definitions.h"

#define TYPES_BLOCK_BLOCK_PART_TRANSACTION_COUNT_LENGTH 4

namespace types {

    struct Block_Part {

        unsigned char miner[ WALLET_PUBLIC_KEY_LENGTH ];

        unsigned char signature[ WALLET_SIGNATURE_LENGTH ];

        int transactions_count;

        void* transactions;

        ~Block_Part(); Block_Part( unsigned char*, unsigned char*, int, void* );

        int get_data_representation_length();

        void* get_data_representation();

    };

}

#endif