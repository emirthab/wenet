#include "packet_handler.h"

#include "server_packets/packet_auth_request.hpp"

using namespace godot;

void PacketHandler::_bind_methods()
{
}

PacketHandler::PacketHandler()
{
    register_packet<PacketAuthenticationRequest>("AUTH_REQ");
}

template <typename T>
void PacketHandler::register_packet(const std::string &header)
{
    packet_factory[header] = []() -> Packet *
    { return new T(); };
}

Packet *PacketHandler::create_packet(const std::string &header)
{
    auto it = packet_factory.find(header);
    if (it != packet_factory.end())
    {
        return (it->second)();
    }
    return nullptr;
}

void PacketHandler::handle_packet(PackedByteArray bytes, Client *client, Packet::Tunnel tunnel)
{
    Array _packet = UtilityFunctions::bytes_to_var(bytes);

    String header = _packet[0];
    _packet.pop_front();

    Packet *packet = create_packet(header.utf8().get_data());

    if (packet)
    {
        packet->tunnel = tunnel;
        packet->sender = client;
        packet->data = _packet;
        packet->read();
    }
}