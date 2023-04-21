

#ifndef UTILS_UTILS_FUNCTIONS_H
#define UTILS_UTILS_FUNCTIONS_H

namespace utils {

    // Converts a byte array into a hexadecimal byte array
    // Returns malloc() value with the hexadecimal
    void* convert_bytes_hex( void*, unsigned long long );

}

#endif