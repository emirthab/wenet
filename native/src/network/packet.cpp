#include "packet.h"

using namespace godot;

Packet::Packet(const PackedByteArray &packet, Ref<PacketPeerDTLS> peer)
{
    this->sender = peer;
    this->packet_builder(packet);
};

Packet::Packet(const PackedByteArray &packet, Client *client)
{
    this->sender = client;
    this->packet_builder(packet);
};

Packet::Packet(const bool &reliable, const String &packet_uid = String(), const String &header = String(), const Array &data = Array())
{
    this->reliable = reliable;
    this->packet_uid = packet_uid;
    this->header = header;
    this->data = data;
};

void Packet::packet_builder(const PackedByteArray &packet)
{
    Array _packet = UtilityFunctions::bytes_to_var(packet);

    if (int(_packet[0]) == 0)
    {
        this->reliable = false;
    }
    else
    {
        this->reliable = true;
        _packet.pop_front();

        this->packet_uid = String(_packet[0]);
        _packet.pop_front();
    }

    this->header = String(_packet[0]);
    _packet.pop_front();

    this->data = _packet;
}
