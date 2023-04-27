

#include "transaction.h"
#include "block_part.h"

#include <iostream>
#include <string.h>


types::Block_Part::~Block_Part() {}

types::Block_Part::Block_Part( unsigned char* __miner, unsigned char* __signature, int __transactions_count, void* __transactions ) 
    : transactions_count( __transactions_count ), transactions( __transactions ) {

        memcpy( miner, __miner, WALLET_PUBLIC_KEY_LENGTH );
        memcpy( signature, __signature, WALLET_SIGNATURE_LENGTH );

}

int types::Block_Part::get_data_representation_length() { 
    
    return WALLET_PUBLIC_KEY_LENGTH + WALLET_SIGNATURE_LENGTH + TYPES_BLOCK_BLOCK_PART_TRANSACTION_COUNT_LENGTH + 
        TYPES_TRANSACTION_TRANSACTION_FULL_SIZE * transactions_count; 

}

void* types::Block_Part::get_data_representation() {

    void* _data = 
        malloc( get_data_representation_length() ), *_rtr = _data;

    memcpy(
        _data,
        this,
        WALLET_PUBLIC_KEY_LENGTH + WALLET_SIGNATURE_LENGTH + TYPES_BLOCK_BLOCK_PART_TRANSACTION_COUNT_LENGTH + TYPES_TRANSACTION_TRANSACTION_FULL_SIZE
    ); _data = _data + WALLET_PUBLIC_KEY_LENGTH + WALLET_SIGNATURE_LENGTH + TYPES_BLOCK_BLOCK_PART_TRANSACTION_COUNT_LENGTH + TYPES_TRANSACTION_TRANSACTION_FULL_SIZE;

    memcpy(
        _data,
        transactions,
        TYPES_TRANSACTION_TRANSACTION_FULL_SIZE * transactions_count
    );

    return _rtr;

}


