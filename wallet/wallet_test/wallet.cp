
#include <iostream>
#include "wallet.h"

int main() {

    wallet::Wallet* _wallet  = 
        wallet::Wallet::get_wallet_by_file( "./wallet_settings");

    _wallet->run();

    _wallet->~Wallet(); free( _wallet );

}