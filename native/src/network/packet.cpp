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

Packet::Packet(const PacketType &packet_type, const String &ack_uid = String(), const String &header = String(), const Array &data = Array())
{
    this->packet_type = packet_type;
    this->ack_uid = ack_uid;
    this->header = header;
    this->data = data;
};

void Packet::packet_builder(const PackedByteArray &packet)
{
    Array _packet = UtilityFunctions::bytes_to_var(packet);

    this->packet_type = PacketType(int(_packet[0]));
    _packet.pop_front();

    if (this->packet_type == Packet::PACKET_TYPE_RELIABLE || this->packet_type == Packet::PACKET_TYPE_RETRANSMISSION)
    {
        this->ack_uid = String(_packet[0]);
        _packet.pop_front();
    }

    this->header = String(_packet[0]);
    _packet.pop_front();

    this->data = _packet;
}

PackedByteArray Packet::get_bytes()
{
    Array arr = Array();
    arr.push_back(int(this->packet_type));
    if (this->packet_type == Packet::PACKET_TYPE_RELIABLE || this->packet_type == Packet::PACKET_TYPE_RETRANSMISSION)
    {
        if (this->ack_uid.is_empty())
        {
            arr.push_back(String(GENERATE_UID(8).c_str()));
        }
        else
        {
            arr.push_back(this->ack_uid);
        }
    }
    arr.push_back(this->header);
    arr.append_array(this->data);

    PackedByteArray bytes = UtilityFunctions::var_to_bytes(arr);
    return bytes;
}