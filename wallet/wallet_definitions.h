

#ifndef WALLET_WALLET_DEFINITIONS_H
#define WALLET_WALLET_DEFINITIONS_H

#define WALLET_PRIVATE_KEY_LENGTH 64 // Ed25519 public key
#define WALLET_PUBLIC_KEY_LENGTH 32 // Ed25519 private key

#define WALLET_SIGNATURE_LENGTH 64 // Ed25519 Signature lenght 

#define WALLET_SAVE_DEFAULT_PATH "./wallet_information" // Default path to save wallet file

namespace p2p {

    // Connection
    struct Connection;

    // End Point IP v4
    struct End_Point_IPv4;

    // End Point IP v6
    struct End_Point_IPv6;

}

#endif