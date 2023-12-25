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

#include <godot_cpp/classes/tcp_server.hpp>
#include <godot_cpp/classes/stream_peer_tcp.hpp>

#include <godot_cpp/classes/thread.hpp>

#include "chunk.h"
#include "client.h"
#include "utils/common.h"
#include "event_handler.h"

namespace godot
{
    class Server : public Node
    {
        GDCLASS(Server, Node)

    private:
        struct AuthStruct
        {
            Ref<PacketPeerDTLS> dtls_peer;
            Ref<StreamPeerTCP> tcp_peer;
            String auth_token;
            String user_name;
            long long timestamp;
        };

        Ref<Thread> auth_thread;
        Thread *tcp_thread;
        Thread *udp_thread;

        UDPServer *udp_server;
        DTLSServer *dtls_server;
        TCPServer *tcp_server;

        EventHandler *event_handler;

        int chunk_cell_radius = 10;
        int chunk_size = 128;
        int auth_timeout = 10000;

        bool handling = false;

        std::vector<Client *> clients;
        std::vector<AuthStruct> auth_pool;

        std::vector<Ref<PacketPeerDTLS>> unauthorized_dtls_peers;
        std::vector<Ref<StreamPeerTCP>> unauthorized_tcp_peers;

        std::unordered_map<int, std::unordered_map<int, Chunk *>> chunk_map;

    protected:
        static Server *singleton;
        static void _bind_methods();

    public:
        Server()
        {
            singleton = this;
            event_handler = memnew(EventHandler);
        }

        void init();
        void _add_to_tree();
        virtual void _process(double delta) override;
        static Server *get_singleton() { return singleton; };

        void start_server(int port, const Ref<CryptoKey> &key, const Ref<X509Certificate> &cert);

        Chunk *get_chunk(int x, int y);

        void load_chunks(int chunk_size, int chunk_cell_radius);

        EventHandler *get_event_handler() { return this->event_handler; };

        enum Tunnel
        {
            TUNNEL_TCP = 1,
            TUNNEL_DTLS = 0,
        };

        void authenticate(Ref<PacketPeerDTLS> peer, const String &auth_token, const String &user_name);
        void authenticate(Ref<StreamPeerTCP> peer, const String &auth_token, const String &user_name);

        void authenticator();
    };
}

VARIANT_ENUM_CAST(Server::Tunnel);

#endif