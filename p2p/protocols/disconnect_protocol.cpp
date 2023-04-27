

#include "disconnect_protocol.h" 
#include "protocol_ids.h" // Protocols Ids
#include "packet.h" // struct Packet;

#include <iostream>


p2p::Packet* p2p::Disconnect_Protocol::get_packet() {

    p2p::Packet* _packet = 
        ( p2p::Packet* ) malloc( sizeof( p2p::Packet ) );

    new ( _packet ) 
        p2p::Packet(
            P2P_PROTOCOLS_PROTOCOL_ID_DISCONNECT_PROTOCOL,
            0, 0
        );

    return _packet;

}

