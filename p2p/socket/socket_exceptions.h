

#ifndef P2P_SOCKET_SOCKET_EXCEPTIONS_H
#define P2P_SOCKET_SOCKET_EXCEPTIONS_H

#include <exception>

namespace p2p {

    // Connection Exceptions 

    struct Unknow_Connection_Version_Exception : std::exception {

        const char* what() const throw();

    };

}

#endif
