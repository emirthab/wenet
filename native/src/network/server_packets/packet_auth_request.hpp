#ifndef PACKET_CONNECTION_REQUEST_H
#define PACKET_CONNECTION_REQUEST_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "../packet.hpp"

namespace godot
{
    class PacketAuthenticationRequest : public Packet
    {
        GDCLASS(PacketAuthenticationRequest, Packet)

    public:
        /** Constructor for sending packet */
        PacketAuthenticationRequest(){

        };

        virtual void read() override{

        };

        virtual PackedByteArray write() override
        {
            return Packet::write();
        };
    };
}
#endif
