
#include <iostream>
#include <string.h>
int main() {

    char* _name = 
        ( char* ) malloc( 4 );

    memcpy(
        _name,
        "opa",
        4
    );

    _name[ 0 ] = 2;

    FILE* _file = 
        fopen( _name, "wb" );

    fwrite(
        "ola",
        3, 1,
        _file
    );

    fclose( _file );

}