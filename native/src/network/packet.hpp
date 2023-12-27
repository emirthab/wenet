#ifndef PACKET_H
#define PACKET_H

#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/classes/packet_peer_dtls.hpp>
#include <godot_cpp/classes/stream_peer_tcp.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/object.hpp>

#include "client.h"
#include "../utils/common.h"

namespace godot
{
    class Packet : public Object
    {
        GDCLASS(Packet, Object)

    protected:
        static void _bind_methods(){};

    public:
        Packet(){};

        enum Tunnel {
            TUNNEL_UDP = 0,
            TUNNEL_TCP = 1,
        };

        virtual ~Packet(){};

        Tunnel tunnel;
        String header;
        Array data;

        /** Empty if packet is initialized for server-side sending */
        std::variant<Ref<PacketPeerDTLS>, Ref<StreamPeerTCP>, Client *> sender;

        /** Using for received data */
        virtual void read(){};

        /** Using for outgoing data */
        virtual PackedByteArray write(){
            return PackedByteArray();
        };
    };
}

VARIANT_ENUM_CAST(Packet::Tunnel);

#endif