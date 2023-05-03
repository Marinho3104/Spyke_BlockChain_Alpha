
#include <iostream>
#include <string.h>

#include "utils_functions.h"
#include "transaction.h"
#include "block_part.h"
#include "wallet.h"
#include "block.h"

unsigned long long _count_transaction = 0; 

types::Transaction* get_transaction( wallet::Wallet* __wallet ) {

    unsigned char __to[ WALLET_PUBLIC_KEY_LENGTH ];

    for ( int _ = 0; _ < WALLET_PUBLIC_KEY_LENGTH; _++ ) __to[ _ ] = 0;

    types::Transaction* _transaction = 
        new types::Transaction(
            __wallet->public_key,
            __to,
            100, 5, _count_transaction++
        );

     __wallet->sign_transaction(
        _transaction
    );

    _transaction->from_balance = 100;
    _transaction->to_balance = 100;

    return _transaction;


}

types::Block_Part* get_block_part( wallet::Wallet* __miner_wallet, wallet::Wallet* __transactions_wallet ) {

    int _transactions_count = 2;

    void* _transactions = 
        malloc( sizeof( types::Transaction ) * _transactions_count );

    for ( int _ = 0; _ < _transactions_count; _++ ) {

        types::Transaction* _trans = 
            get_transaction( __transactions_wallet );

        memcpy(
            _transactions + _ * sizeof( types::Transaction ),
            _trans,
            sizeof( types::Transaction )
        );

    }

    types::Block_Part* _block_part = 
        ( types::Block_Part* ) types::Block_Part::get_block_part_manually(
            __miner_wallet->public_key,
            _transactions_count,
            _transactions
        );
    
    __miner_wallet->sign_block_part(
        _block_part,
        _block_part->get_data_representation_length( _transactions_count ) - WALLET_SIGNATURE_LENGTH
    );

    return _block_part;

}

types::Block* get_block( wallet::Wallet* __miner_wallet, wallet::Wallet* __transactions_wallet ) {

    int _block_parts_count = 1;

    types::Block_Part* _block_parts =
        get_block_part( __miner_wallet, __transactions_wallet );

    types::Block* _block = 
        ( types::Block* ) types::Block::get_block_manually(
            0, _block_parts_count, _block_parts
        );

    return _block;

}

int main() {

    
    // wallet::Wallet* _miner_wallet = 
    //     wallet::Wallet::get_wallet_by_file( "./wallet_settings" );


    // long _size = 0;

    // void* _block_data = 
    //     utils::get_file_data( "0000000000000000", _size );

    // types::Block* _block = 
    //     types::Block::get_by_data( _block_data, _size );

    // std::cout << _block << std::endl;

    // std::cout << 
    //     types::Block::get_data_representation_length(
    //         _block->block_parts_count,
    //         &_block->block_parts
    //     )

    // << std::endl;

    // types::Block_Part* _block_parts = 
    //     ( types::Block_Part* ) &_block->block_parts;

    // std::cout << _block_parts->transactions_count << std::endl;

    // void* _sign_data = (( void* ) &_block->block_parts) + WALLET_SIGNATURE_LENGTH;

    // std::cout << _block_parts->get_data_representation_length( _block_parts->transactions_count ) - WALLET_SIGNATURE_LENGTH << std::endl;

    // std::cout << 
    //     _miner_wallet->verify_signature(
    //         _block_parts->signature,
    //         ( unsigned char* ) _sign_data,
    //         _block_parts->get_data_representation_length( _block_parts->transactions_count ) - WALLET_SIGNATURE_LENGTH
    //     ) << std::endl;
    
    wallet::Wallet* _miner_wallet = 
        wallet::Wallet::get_wallet_by_file( "./wallet_settings" );


    wallet::Wallet* _transactions_wallet = 
        wallet::Wallet::get_new_wallet();



    types::Transaction* _transaction = 
        get_transaction( _transactions_wallet );

    types::Block_Part* _block_parts = 
        get_block_part( _miner_wallet, _transactions_wallet );

    types::Block* _block = 
        get_block( _miner_wallet, _transactions_wallet );

    utils::set_file_data(
        ( char* ) utils::convert_bytes_hex( &_block->creation_time, 8 ),
        _block,
        types::Block::get_data_representation_length(
            _block->block_parts_count,
            &_block->block_parts
        )
    );

}

