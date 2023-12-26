#include "packet_handler.h"

#include "server_packets/packet_auth_request.hpp"

using namespace godot;

void PacketHandler::_bind_methods()
{
}

PacketHandler::PacketHandler()
{
    register_event<PacketAuthenticationRequest>("AUTH_REQ");
}

template <typename T>
void PacketHandler::register_event(const std::string &header)
{
    packet_factory[header] = []() -> Packet *
    { return new T(); };
}

Packet *PacketHandler::create_packet(Packet *packet)
{
    auto it = packet_factory.find(packet->header.utf8().get_data());
    if (it != packet_factory.end())
    {
        Packet *new_packet = (it->second)();

        Packet *casted_packet = dynamic_cast<Packet *>(new_packet);

        if (casted_packet)
        {
            *casted_packet = *packet;
            return casted_packet;
        }
        else
        {
            // Type conversion failed, delete new packet to avoid memory leak
            delete new_packet;
            return nullptr;
        }
    }
    return nullptr;
}

void PacketHandler::handle_event(Ref<PacketPeerDTLS> peer)
{
    Packet *_packet = new Packet(peer->get_packet(), peer);
    Packet *packet = create_packet(_packet);

    delete _packet;
    _packet = nullptr;

    if (packet)
    {
        packet->handle();
    }
}
