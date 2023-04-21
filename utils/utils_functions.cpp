

#include "utils_functions.h"

#include <iostream>


void* utils::convert_bytes_hex( void* __bytes, unsigned long long __size ) {

    char hexadecimal_characters[ 16 ] = 
        { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

    void* _hexadecimal_value = 
        malloc( __size * 2 + 1 ), *_rtr = _hexadecimal_value;

    while( __size-- ) {

        *( ( char* ) _hexadecimal_value ) = hexadecimal_characters[ ( *( ( char* ) __bytes ) >> 4 ) & 0xf ];

        _hexadecimal_value = _hexadecimal_value + 1;

        *( ( char* ) _hexadecimal_value ) = hexadecimal_characters[ *( ( char* ) __bytes ) & 0xf ];

        __bytes = __bytes + 1; _hexadecimal_value = _hexadecimal_value + 1;

    }

    *(
        ( char* ) _hexadecimal_value
    ) = 0;

    return _rtr;

}
