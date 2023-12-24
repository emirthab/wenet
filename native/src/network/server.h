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

#include "chunk.h"
#include "utils/common.h"
#include "event_handler.h"

namespace godot
{
    class Server : public Node
    {
        GDCLASS(Server, Node)

    private:
        UDPServer *server;
        DTLSServer *dtls;

        EventHandler *event_handler;

        int chunk_cell_radius = 10;
        int chunk_size = 128;
        bool handling = false;

        std::vector<Ref<PacketPeerDTLS>> peer_list;
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

        void broadcast();
    };
}

#endif