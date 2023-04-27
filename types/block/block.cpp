
#include "block_part.h"
#include "block.h"

#include <iostream>


types::Block::~Block() {}

types::Block::Block( unsigned long long __creation_time, unsigned char __block_parts_count, void* __block_parts ) 
    : creation_time( __creation_time ), block_parts_count( __block_parts_count ), block_parts( __block_parts ) {}

unsigned long long types::Block::get_data_representation_length() {

    unsigned long long _block_parts_length = 0;

    void* _current_block_part = block_parts;

    for ( char _ = 0; _ < block_parts_count; _++ ) {

        _block_parts_length += ( ( types::Block_Part* ) &_current_block_part )->get_data_representation_length();

        _current_block_part = _current_block_part + ( ( types::Block_Part* ) &_current_block_part )->get_data_representation_length();

    }

    return TYPES_BLOCK_BLOCK_CREATION_TIME_LENGTH + TYPES_BLOCK_BLOCK_BLOCK_PARTS_COUNT + _block_parts_length;

}

