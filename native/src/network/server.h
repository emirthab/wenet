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
#include "packet_handler.h"

namespace godot
{
    struct AuthStruct
    {
        Ref<PacketPeerDTLS> dtls_peer;
        Ref<PacketPeerUDP> udp_peer;
        Ref<StreamPeerTCP> tcp_peer;

        String auth_token;
        String user_name;
        long long timestamp;

        AuthStruct() : timestamp(GET_TIME()){};
        AuthStruct(const Ref<PacketPeerDTLS> &dtls_peer, const Ref<PacketPeerUDP> &udp_peer, const long long &timestamp)
        {
            this->dtls_peer = dtls_peer;
            this->udp_peer = udp_peer;
            this->timestamp = timestamp;
        };
        AuthStruct(const Ref<StreamPeerTCP> &tcp_peer, const long long &timestamp)
        {
            this->tcp_peer = tcp_peer;
            this->timestamp = timestamp;
        };
    };

    class Server : public Node
    {
        GDCLASS(Server, Node)

    private:
        Ref<Thread> auth_thread;

        UDPServer *udp_server;
        DTLSServer *dtls_server;
        TCPServer *tcp_server;

        PacketHandler *packet_handler;

        int chunk_cell_radius = 10;
        int chunk_size = 128;
        int auth_timeout = 5000;

        bool handling = false;

        std::vector<Client *> clients;

        /** IP address, Struct */
        std::unordered_map<std::string, AuthStruct> unauthorized_peers;

        std::unordered_map<int, std::unordered_map<int, Chunk *>> chunk_map;

    protected:
        static Server *singleton;
        static void _bind_methods();

    public:
        Server();

        void init();
        void _add_to_tree();
        virtual void _process(double delta) override;
        static Server *get_singleton() { return singleton; };

        void start_server(int port, const Ref<CryptoKey> &key, const Ref<X509Certificate> &cert);

        Chunk *get_chunk(int x, int y);

        void load_chunks(int chunk_size, int chunk_cell_radius);

        PacketHandler *get_packet_handler() { return this->packet_handler; };

        void authenticator();
        void connector_tcp();
        void connector_udp();
        void handle_unauthorized_packet_udp();
        void handle_unauthorized_packet_tcp();
        void disconnect_from_peers(Ref<PacketPeerDTLS> dtls_peer, Ref<StreamPeerTCP> tcp_peer);

        void authenticate(AuthStruct _struct);

        /** Property Getter Setters */
        void set_auth_timeout(int timeout) { this->auth_timeout = timeout; };
        int get_auth_timeout() { return this->auth_timeout; };
    };
}

#endif