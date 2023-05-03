

#ifndef TYPES_BLOCK_BLOCK_PART_H
#define TYPES_BLOCK_BLOCK_PART_H

#include "wallet_definitions.h"

#define TYPES_BLOCK_BLOCK_PART_TRANSACTION_COUNT_LENGTH 4

namespace types {

    struct Block_Part {

        unsigned char signature[ WALLET_SIGNATURE_LENGTH ];

        unsigned char miner[ WALLET_PUBLIC_KEY_LENGTH ];

        unsigned int transactions_count;

        void* transactions;

        ~Block_Part(); 


        static unsigned long long get_data_representation_length( int );

        static void* get_block_part_manually( unsigned char*, int, void* );

        static Block_Part* get_by_data( void*, unsigned long long );

    } __attribute__((packed));

}

#endif