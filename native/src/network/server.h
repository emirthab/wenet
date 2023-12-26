#ifndef SERVER_H
#define SERVER_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/udp_server.hpp>
#include <godot_cpp/classes/packet_peer_udp.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>

#include <godot_cpp/classes/crypto_key.hpp>
#include <godot_cpp/classes/x509_certificate.hpp>
#include <godot_cpp/classes/dtls_server.hpp>
#include <godot_cpp/classes/tls_options.hpp>
#include <godot_cpp/classes/packet_peer_dtls.hpp>

#include <godot_cpp/classes/thread.hpp>

#include "chunk.h"
#include "client.h"
#include "utils/common.h"
#include "packet_handler.h"

namespace godot
{
    class Server : public Node
    {
        GDCLASS(Server, Node)

    private:
        struct AuthStruct
        {
            Ref<PacketPeerDTLS> peer;
            long long timestamp;
            AuthStruct(const Ref<PacketPeerDTLS> &_peer, const long long &_timestamp) : peer(_peer),
                                                                                        timestamp(_timestamp){};
        };

        Ref<Thread> auth_thread;

        UDPServer *udp_server;
        DTLSServer *dtls_server;

        PacketHandler *packet_handler;

        int chunk_cell_radius = 10;
        int chunk_size = 128;
        int auth_timeout = 5000;

        bool handling = false;

        std::vector<Client *> clients;

        std::vector<AuthStruct> unauthorized_dtls_peers;

        std::unordered_map<int, std::unordered_map<int, Chunk *>> chunk_map;



    protected:
        static Server *singleton;
        static void _bind_methods();

    public:
        Server()
        {
            singleton = this;
            packet_handler = memnew(PacketHandler);
        }

        void init();
        void _add_to_tree();
        virtual void _process(double delta) override;
        static Server *get_singleton() { return singleton; };

        void start_server(int port, const Ref<CryptoKey> &key, const Ref<X509Certificate> &cert);

        Chunk *get_chunk(int x, int y);

        void load_chunks(int chunk_size, int chunk_cell_radius);

        PacketHandler *get_packet_handler() { return this->packet_handler; };

        void authenticator();

        /** Property Getter Setters */
        void set_auth_timeout(int timeout) { this->auth_timeout = timeout; };
        int get_auth_timeout() { return this->auth_timeout; };
    };
}

#endif