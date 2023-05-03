
#include "memory_pool.h"

#include <iostream>


p2p::Memory_Pool::~Memory_Pool() {}

p2p::Memory_Pool::Memory_Pool( unsigned long long __objects_count ) : pool( malloc( sizeof( void* ) * __objects_count ) ) {}



