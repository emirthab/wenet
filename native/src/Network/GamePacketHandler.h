#ifndef GAME_PACKET_HANDLER_H
#define GAME_PACKET_HANDLER_H

#include <godot_cpp/classes/object.hpp>
#include "PacketBase.h"

#include "ClientPackets/PacketConnectionRequest.h"


#include <functional>
#include <iostream>
#include <unordered_map>

namespace godot
{
    class GamePacketHandler : public Object
    {
        GDCLASS(GamePacketHandler, Object)

    private:
        std::unordered_map<std::string, std::function<PacketBase *()>> packet_factory;

    protected:
        static void _bind_methods();

    public:
        GamePacketHandler();

        template <typename T>
        void register_packet(const std::string &header);

        PacketBase *create_packet(const std::string &header);

        void handle_packet(Array data);
    };
}

#endif