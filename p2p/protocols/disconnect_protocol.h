

#ifndef NODE_PROTOCOLS_DISCONNECT_PROTOCOL_H
#define NODE_PROTOCOLS_DISCONNECT_PROTOCOL_H

#include "socket_definitions.h"

namespace p2p {

    struct Disconnect_Protocol {

        static p2p::Packet* get_packet();

    };

}

#endif