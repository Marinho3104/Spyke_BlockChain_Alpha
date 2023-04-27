

#ifndef P2P_NODE_NODE_EXCEPTIONS_H
#define P2P_NODE_NODE_EXCEPTIONS_H

#include <exception>

namespace p2p {

    struct Unable_Set_Server_Up : std::exception {

        char* what() const throw(); 

    };

}

#endif