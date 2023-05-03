
#include "utils_functions.h"
#include "block_part.h"
#include "block.h"

#include <iostream>
#include <string.h>

types::Block::~Block() {}

void types::Block::save_block() {

    // Get path to file
    // Miss full path 
    char* _path =  ( char* ) utils::convert_bytes_hex( &creation_time, 8 );

    utils::set_file_data(
        _path,
        this,
        get_data_representation_length(
            block_parts_count,
            &block_parts
        )
    );

}


unsigned long long types::Block::get_data_representation_length( unsigned char __block_parts_count, void* __block_parts ) {

    unsigned long long _block_parts_data_size = 0, _temp;

    for ( int _ = 0; _ < __block_parts_count; _++ ) {

        _temp = types::Block_Part::get_data_representation_length(
            ( ( types::Block_Part* ) __block_parts )->transactions_count
        );

        _block_parts_data_size += _temp; __block_parts = __block_parts + _temp;


    }

    return TYPES_BLOCK_BLOCK_CREATION_TIME_LENGTH + TYPES_BLOCK_BLOCK_BLOCK_PARTS_COUNT + _block_parts_data_size;

}

void* types::Block::get_block_manually( unsigned long long __creation_time, unsigned char __block_parts_count, void* __block_parts ) {

    unsigned long long _block_data_size = 
        get_data_representation_length( __block_parts_count, __block_parts );

    std::cout << "Block size -> " << _block_data_size << std::endl;

    void* _block_data = 
        malloc( _block_data_size );

    memcpy(
        _block_data,
        &__creation_time,
        TYPES_BLOCK_BLOCK_CREATION_TIME_LENGTH
    );

    memcpy(
        _block_data + TYPES_BLOCK_BLOCK_CREATION_TIME_LENGTH,
        &__block_parts_count,
        TYPES_BLOCK_BLOCK_BLOCK_PARTS_COUNT
    );

    memcpy(
        _block_data + TYPES_BLOCK_BLOCK_CREATION_TIME_LENGTH + TYPES_BLOCK_BLOCK_BLOCK_PARTS_COUNT,
        __block_parts,
        _block_data_size - TYPES_BLOCK_BLOCK_CREATION_TIME_LENGTH - TYPES_BLOCK_BLOCK_BLOCK_PARTS_COUNT
    );

    return _block_data;

}


types::Block* types::Block::get_by_data( void* __data, unsigned long long __data_size ) {

    void* _cpy = __data;

    if (
        __data_size < TYPES_BLOCK_BLOCK_CREATION_TIME_LENGTH + TYPES_BLOCK_BLOCK_BLOCK_PARTS_COUNT
    ) return 0;

    __data_size -= TYPES_BLOCK_BLOCK_CREATION_TIME_LENGTH - TYPES_BLOCK_BLOCK_BLOCK_PARTS_COUNT;

    __data = __data + TYPES_BLOCK_BLOCK_CREATION_TIME_LENGTH + TYPES_BLOCK_BLOCK_BLOCK_PARTS_COUNT;

    for ( char _ = 0; _ <  *( ( unsigned char* ) ( __data - TYPES_BLOCK_BLOCK_BLOCK_PARTS_COUNT ) ); _++ ) {

        types::Block_Part* _block_part = 
            types::Block_Part::get_by_data( __data, __data_size );

        if ( ! _block_part ) return 0;
        unsigned long long _block_part_size = types::Block_Part::get_data_representation_length( _block_part->transactions_count );

        __data = __data + _block_part_size; __data_size -= _block_part_size;

    }

    return 
        ( types::Block* ) _cpy;

}

types::Block* types::Block::get_by_creation_time( unsigned long long* __creation_time ) {

    char* _hex_creation_time = 
        ( char* ) utils::convert_bytes_hex( __creation_time, TYPES_BLOCK_BLOCK_CREATION_TIME_LENGTH );

    long _size;

    void* _file_data = 
        utils::get_file_data( _hex_creation_time, _size );

    if ( ! _file_data ) return 0;

    return 
        get_by_data(
            _file_data, _size
        );

}


