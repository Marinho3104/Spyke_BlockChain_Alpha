

#ifndef TYPES_BLOCK_BLOCK_H
#define TYPES_BLOCK_BLOCK_H

#define TYPES_BLOCK_BLOCK_CREATION_TIME_LENGTH 8
#define TYPES_BLOCK_BLOCK_BLOCK_PARTS_COUNT 1

namespace types {

    struct Block {

        unsigned long long creation_time;

        unsigned char block_parts_count; 
        
        void* block_parts;

        ~Block();

        Block( unsigned long long, unsigned char, void* );

        unsigned long long get_data_representation_length();

    };

}

#endif
