

#ifndef TYPES_BLOCK_BLOCK_H
#define TYPES_BLOCK_BLOCK_H

#define TYPES_BLOCK_BLOCK_CREATION_TIME_LENGTH 8
#define TYPES_BLOCK_BLOCK_BLOCK_PARTS_COUNT 1

namespace types {

    struct Block {

        unsigned long long creation_time;

        unsigned char block_parts_count; 
        
        void* block_parts;

        ~Block(); void save_block();


        static unsigned long long get_data_representation_length( unsigned char, void* );

        static void* get_block_manually( unsigned long long, unsigned char, void* );

        static Block* get_by_data( void* , unsigned long long );

        static Block* get_by_creation_time( unsigned long long* );

    } __attribute__((packed));

}

#endif
