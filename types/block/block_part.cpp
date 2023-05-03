

#include "transaction.h"
#include "block_part.h"

#include <iostream>
#include <string.h>


types::Block_Part::~Block_Part() {}

unsigned long long types::Block_Part::get_data_representation_length( int __transactions_count ) { 
    
    return WALLET_SIGNATURE_LENGTH + WALLET_PUBLIC_KEY_LENGTH + TYPES_BLOCK_BLOCK_PART_TRANSACTION_COUNT_LENGTH + 
        TYPES_TRANSACTION_TRANSACTION_FULL_SIZE * __transactions_count; 

}

void* types::Block_Part::get_block_part_manually( unsigned char* __miner, int __transactions_count, void* __transactions ) {

    std::cout << "Block Part size -> " << get_data_representation_length( __transactions_count ) << std::endl;

    void* _block_part_data = 
        malloc( get_data_representation_length( __transactions_count ) );

    for ( int _ = 0; _ < WALLET_SIGNATURE_LENGTH; _++ ) *( ( unsigned char* ) ( _block_part_data + _ ) ) = 0;

    memcpy(
        _block_part_data + WALLET_SIGNATURE_LENGTH,
        __miner,
        WALLET_PUBLIC_KEY_LENGTH
    );

    memcpy(
        _block_part_data + WALLET_SIGNATURE_LENGTH + WALLET_PUBLIC_KEY_LENGTH,
        &__transactions_count,
        TYPES_BLOCK_BLOCK_PART_TRANSACTION_COUNT_LENGTH
    );

    memcpy(
        _block_part_data + WALLET_SIGNATURE_LENGTH + WALLET_PUBLIC_KEY_LENGTH + TYPES_BLOCK_BLOCK_PART_TRANSACTION_COUNT_LENGTH,
        __transactions,
        TYPES_TRANSACTION_TRANSACTION_FULL_SIZE * __transactions_count
    );

    return _block_part_data;

}


types::Block_Part* types::Block_Part::get_by_data( void* __data, unsigned long long __data_size ) {

    if ( __data_size < WALLET_SIGNATURE_LENGTH + WALLET_PUBLIC_KEY_LENGTH + TYPES_BLOCK_BLOCK_PART_TRANSACTION_COUNT_LENGTH ) return 0;

    if (
        __data_size - WALLET_SIGNATURE_LENGTH - WALLET_PUBLIC_KEY_LENGTH - TYPES_BLOCK_BLOCK_PART_TRANSACTION_COUNT_LENGTH < 
            *( ( unsigned int* ) ( __data + WALLET_SIGNATURE_LENGTH + WALLET_PUBLIC_KEY_LENGTH ) ) * TYPES_TRANSACTION_TRANSACTION_FULL_SIZE
    ) return 0;

    return
        ( types::Block_Part* ) __data;

}



