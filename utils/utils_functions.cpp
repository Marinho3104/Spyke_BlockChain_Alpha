

#include "utils_functions.h"

#include <iostream>


void* utils::convert_bytes_hex( void* __bytes, unsigned long long __size ) {

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

void* utils::convert_hex_bytes( void* __hex, unsigned long long __size, bool __invert ) {

    void* _bytes_value = 
        malloc( __size / 2 ), *_rtr = _bytes_value;

    if ( __invert ) _bytes_value = _bytes_value + __size / 2 - 1;

    for ( unsigned long long _ = 0; _ < __size; _ += 2 ) {

        unsigned char _f = *( ( unsigned char* ) __hex ); 
        _f = _f >= 'A' ? _f - 'A' + 10 : _f - '0';

        unsigned char _s = *( ( unsigned char* ) ( __hex + 1 ) );
        _s = _s >= 'A' ? _s - 'A' + 10 : _s - '0';

        *( ( char* ) _bytes_value ) = ( _f << 4 ) | _s;

        _bytes_value = _bytes_value + ( __invert ? -1 : 1 ); __hex = __hex + 2;

    }

    return _rtr;

}

void utils::set_file_data( char* __file_path, void* __data, unsigned long long __data_size ) {

    FILE* _file = 
        fopen( __file_path, "wb" );

    if ( ! _file ) return;

    fwrite(
        __data,
        __data_size, 1,
        _file
    );

    fclose( _file );

}

void* utils::get_file_data( char* __file_path, long& __file_size ) {

    FILE* _file = 
        fopen( __file_path, "rb" );

    if ( ! _file ) return 0;

    fseek( _file, 0, SEEK_END );

    __file_size = ftell( _file );

    fseek( _file, 0, SEEK_SET );

    void* _data = 
        malloc( __file_size );

    fread(
        _data,
        __file_size, 1,
        _file
    );

    fclose( _file );

    return _data;

}


