#ifndef PACKET_HANDLER_H
#define PACKET_HANDLER_H

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/packet_peer_dtls.hpp>

#include "client.h"
#include "packet.h"

#include <functional>
#include <iostream>
#include <unordered_map>

namespace godot
{
    class PacketHandler : public Object
    {
        GDCLASS(PacketHandler, Object)

    private:
        std::unordered_map<std::string, std::function<Packet *()>> packet_factory;

    protected:
        static void _bind_methods();

    public:
        PacketHandler();

        template <typename T>
        void register_packet(const std::string &header);

        Packet *create_packet(Packet *packet);

        void handle_packet(Ref<PacketPeerDTLS> peer);

        std::vector<String> unauthorized_available_headers = {
            "AUTH_REQ"
        };
    };
}

#endif