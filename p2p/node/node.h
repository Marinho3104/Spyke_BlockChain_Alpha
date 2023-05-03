

#ifndef P2P_NODE_NODE_H
#define P2P_NODE_NODE_H

#define P2P_NODE_NODE_SETTINGS_SERVER_CONNECTION_ENABLE_LENGTH 1
#define P2P_NODE_NODE_SETTINGS_MAX_SERVER_CONNECTIONS_LENGTH 2

#define P2P_NODE_NODE_SETTINGS_MAX_ORDINARY_CONNECTIONS_LENGTH 2
#define P2P_NODE_NODE_SETTINGS_MAX_STABLE_CONNECTIONS_LENGTH 2

#define P2P_NODE_NODE_SETTINGS_MEMORY_POOL_OBJECT_LENGTH 8

#define P2P_NODE_NODE_SETTINGS_INITIAL_CONNECTIONS_COUNT_LENGTH 2

#define P2P_NODE_NODE_CONNECTION_TYPE_ORDINARY_CONNECTION 0
#define P2P_NODE_NODE_CONNECTION_TYPE_STABLE_CONNECTION 1

#include "node_definitions.h" // struct Connection;

namespace p2p {

    // Holds all node settings 
    struct Node_Settings {

        // Server Connection
        p2p::Connection* server_connection; unsigned short max_server_connections;

        // Max accepted connections
        unsigned short max_ordinary_connections;

        // Max stable connections
        unsigned short max_stable_connections;

        // Memory pool objects count
        unsigned long long memory_pool_objects;

        // Initial Connections
        p2p::Connection** initial_connections; unsigned short initial_connections_count;

        ~Node_Settings(); Node_Settings(); Node_Settings( p2p::Connection*, unsigned short, unsigned short, unsigned long long, unsigned short, p2p::Connection**, unsigned short );

        // Prints the current node settings information about its fields
        void print_node_settings_info();

        // Run interface of Node_Settings
        void run_interface();

        // Option 1
        void option_1();

        // Option 6
        void option_6();

        // Add new connection into a initial_connections
        void add_initial_connection( p2p::Connection* );

        // Remove a connection from initial_connections by index
        void remove_initial_connection( short );

        // Returns the data length of this Node_Settings
        unsigned long long get_data_length();

        // Returns the data of this Node_Settings
        // malloc() data
        void* get_data();

        // Returns a default new Node_Settings
        static Node_Settings* get_new_node_settings();

        // Returns a Node_Settings by data
        static Node_Settings* get_node_settings_by_data( void* );

    };

    // Represents a node with the basic functionalities ( connect with others nodes, send packets, etc ... )
    // It is intended to be used along with another functions to compose a full node, miner etc ...
    struct Node {

        // Node Settings
        Node_Settings* settings;

        // Is running
        bool is_running;

        // Ordinary connections
        p2p::Connection** ordinary_connections; unsigned short ordinary_connections_count; sem_t ordinary_connections_sem;

        // Stable connections
        p2p::Connection** stable_connections; unsigned short stable_connections_count; sem_t stable_connections_sem;

        // Hold all file descriptors
        fd_set file_descriptors; sem_t file_descriptors_sem;

        // Memory Pool
        Memory_Pool* memory_pool;

        ~Node(); Node( Node_Settings* );

        // Prints the current node information about its fields
        void print_node_info();

        // Adds new connection into ordinary_connections
        bool add_new_ordinay_connection( p2p::Connection* );

        // Remove a connection from ordinary_connections
        void remove_ordinary_connection( p2p::Connection* );

        // Remove a connection
        void remove_connection( p2p::Connection*, char );

        // Add a new file descriptor to file file_descriptors
        void add_new_file_descriptor( int );

        // Remove a file descriptor from file_descriptors
        void remove_file_descriptor( int );

        // Run node
        void run();

        // Monitor the open socket descriptors
        void monitor_open_descriptors();

        // Accept new connections requests
        void accept_new_connections_requests();

        // Get and handle new packet received from a socket descriptor
        void handle_packet( p2p::Connection*, char );

        // Run interface node
        void run_interface();

        // Run option 3 function
        void option_2();

        // Saves all node informations into a file
        void save_into_file( char* );

        // Returns a Node by data
        static Node* get_node_by_data( void* ); 

        // Returns a Node by file
        static Node* get_node_by_file( char* );

    };

}

#endif


