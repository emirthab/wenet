#ifndef PACKET_H
#define PACKET_H

#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/classes/packet_peer_dtls.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "client.h"
#include "../utils/common.h"

namespace godot
{
    class Packet
    {

    public:
        Packet(){};
        virtual ~Packet(){};

        enum PacketType
        {
            PACKET_TYPE_UNRELIABLE = 0,
            PACKET_TYPE_RELIABLE = 1,
            PACKET_TYPE_ACKNOWLEDGEMENT = 1,
            PACKET_TYPE_RETRANSMISSION = 1,
        };

        /** Constructor For receive from DTLS peer */
        Packet(const PackedByteArray &packet, Ref<PacketPeerDTLS> peer);

        /** Constructor For receive from Client */
        Packet(const PackedByteArray &packet, Client *client);

        /** Constructor For send to Client or peer*/
        Packet(const PacketType &packet_type, const String &ack_uid, const String &header, const Array &data);

        /** Build packet from constructor use only receive */
        void packet_builder(const PackedByteArray &packet);

        PackedByteArray get_bytes();

        PacketType packet_type;
        String ack_uid;
        String header;
        Array data;

        /** Empty if is packet initialized for sending */
        std::variant<Ref<PacketPeerDTLS>, Client *> sender;

        virtual void handle(){};
    };
}

VARIANT_ENUM_CAST(Packet::PacketType);

#endif