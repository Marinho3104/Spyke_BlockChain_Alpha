
#include <iostream>

#include "transaction.h"
#include "wallet.h"

types::Transaction get_transaction( wallet::Wallet* __wallet ) {

    unsigned char __to[ WALLET_PUBLIC_KEY_LENGTH ];

    for ( int _ = 0; _ < WALLET_PUBLIC_KEY_LENGTH; _++ ) __to[ _ ] = 0;

    types::Transaction _transaction = 
        types::Transaction(
            __wallet->public_key,
            __to,
            100, 5, 0
        );

    __wallet->sign_transaction(
        *( ( void** ) &_transaction )
    );

    return _transaction;

}

int main() {

    wallet::Wallet* _wallet = 
        wallet::Wallet::get_new_wallet();

    std::cout << get_transaction( _wallet ).amount << std::endl;

}