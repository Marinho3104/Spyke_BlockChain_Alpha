
#include "transaction.h"

#include <iostream>
#include <string.h>

types::Transaction::~Transaction() {}

types::Transaction::Transaction( unsigned char* __signature, unsigned char* __from, unsigned char* __to, unsigned long long __amount, unsigned long long __fee, unsigned long long __nonce, unsigned long long __from_balance, unsigned long long __to_balance ) 
    : amount( __amount ), fee( __fee ), nonce( __nonce ), from_balance( __from_balance ), to_balance( __to_balance ) {

        memcpy( signature, __signature, WALLET_SIGNATURE_LENGTH );
        memcpy( from, __from, WALLET_PUBLIC_KEY_LENGTH );
        memcpy( to, __to, WALLET_PUBLIC_KEY_LENGTH );

}

types::Transaction::Transaction( unsigned char* __from, unsigned char* __to, unsigned long long __amount, unsigned long long __fee, unsigned long long __nonce ) 
    : amount( __amount ), fee( __fee ), nonce( __nonce ) {
    
        memcpy( from, __from, WALLET_PUBLIC_KEY_LENGTH );
        memcpy( to, __to, WALLET_PUBLIC_KEY_LENGTH );

}
