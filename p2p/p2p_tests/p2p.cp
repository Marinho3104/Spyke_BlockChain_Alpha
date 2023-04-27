
#include <iostream>

#include "connection.h"
#include "end_point_ipv4.h"
#include "node.h" 

p2p::Node* get_new_node() {

    p2p::Node* _node = ( p2p::Node* ) malloc( sizeof( p2p::Node ) );

    new ( _node ) p2p::Node(
        p2p::Node_Settings::get_new_node_settings()
    );

    return _node;    

}

int main() {

    p2p::Node* _node = 
        p2p::Node::get_node_by_file( "./node_settings" );

    _node->run_interface();

    _node->~Node(); free( _node );

    return 0;
    
}

/*

    p2p::End_Point_IPv4* _end_point = 
        new p2p::End_Point_IPv4( 0x7f000001, 54321 );

    p2p::Connection* _connection = 
        new p2p::Connection( P2P_SOCKET_CONNECTION_VERSION_IPV4, _end_point );

    std::cout << "Result -> " << _connection->set_server( 5 ) << std::endl;


*/