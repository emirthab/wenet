#ifndef CLIENT_H
#define CLIENT_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/packet_peer_udp.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>

#include "Utils/Common.h"

namespace godot
{
    class Client : public Node
    {
        GDCLASS(Client, Node)

    private:
        PacketPeerUDP *client = memnew(PacketPeerUDP);

    protected:
        static Client *singleton;
        void _add_to_tree();
        static void _bind_methods();

    public:
        Client(){
            singleton = this;
        };

        bool connected = false;
        void init();
        virtual void _process(double delta) override;
        virtual void _ready() override;

        static Client *get_singleton() { return singleton; };

        void connect_to_server(String ip_address, int port);
    };
}
#endif