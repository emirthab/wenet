#ifndef PACKET_H
#define PACKET_H

#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/classes/packet_peer_dtls.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "client.h"

namespace godot
{
    class Packet
    {

    public:
        Packet(){};
        virtual ~Packet() {}

        /** Constructor For receive from DTLS peer */
        Packet(const PackedByteArray &packet, Ref<PacketPeerDTLS> peer)
        {
            this->sender = peer;
            this->packet_builder(packet);
        };

        /** Constructor For receive from Client */
        Packet(const PackedByteArray &packet, Client *client)
        {
            this->sender = client;
            this->packet_builder(packet);
        };

        /** Constructor For send to Client or peer*/
        Packet(const bool &reliable, const String &packet_uid = String(), const String &header = String(), const Array &data = Array())
        {
            this->reliable = reliable;
            this->packet_uid = packet_uid;
            this->header = header;
            this->data = data;
        };

        /** Build packet from constructor use only receive */
        void packet_builder(const PackedByteArray &packet)
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
        };

        bool reliable;
        String packet_uid;
        String header;
        Array data;

        /** Empty if is packet initialized for sending */
        std::variant<Ref<PacketPeerDTLS>, Client *> sender;

        virtual void handle(){};
    };
}
#endif