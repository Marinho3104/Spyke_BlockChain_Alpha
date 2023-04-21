
#include <iostream>
#include "wallet.h"

int main() {

    wallet::Wallet* _wallet  = 
        wallet::Wallet::get_new_wallet();

    _wallet->run();

    _wallet->~Wallet(); free( _wallet );

}