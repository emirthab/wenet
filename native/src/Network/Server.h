#ifndef SERVER_H
#define SERVER_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/udp_server.hpp>
#include <godot_cpp/classes/packet_peer_udp.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>

#include "Chunk.h"
#include "Utils/Common.h"
#include "GamePacketHandler.h"

namespace godot
{
    class Server : public Node
    {
        GDCLASS(Server, Node)

    private:
        UDPServer *server;
        GamePacketHandler *packet_handler;

        int chunk_cell_radius = 10;
        int chunk_size = 128;
        bool handling = false;

        std::unordered_map<int, std::unordered_map<int, Chunk *>> chunk_map;

    protected:
        static Server *singleton;
        static void _bind_methods();

    public:
        Server()
        {
            singleton = this;
            packet_handler = memnew(GamePacketHandler);
        }

        void init();
        void _add_to_tree();
        virtual void _process(double delta) override;
        static Server *get_singleton() { return singleton; };

        void start_server(int port);

        Chunk *get_chunk(int x, int y);

        void load_chunks(int chunk_size, int chunk_cell_radius);

        GamePacketHandler *get_packet_handler() { return this->packet_handler; };
    };
}

#endif