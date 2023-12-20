#include "GamePacketHandler.h"

using namespace godot;

void GamePacketHandler::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("handle_packet", "data"), &GamePacketHandler::handle_packet);
}

GamePacketHandler::GamePacketHandler()
{
    register_packet<PacketConnectionRequest>("0x00");
}

template <typename T>
void GamePacketHandler::register_packet(const std::string &header)
{
    packet_factory[header] = []() -> PacketBase *
    { return new T(); };
}

PacketBase *GamePacketHandler::create_packet(const std::string &header)
{
    auto it = packet_factory.find(header);
    if (it != packet_factory.end())
    {
        return (it->second)();
    }
    return nullptr;
}

void GamePacketHandler::handle_packet(Array data)
{
    godot::String packet_header = data[0];
    UtilityFunctions::print(packet_header);
}
