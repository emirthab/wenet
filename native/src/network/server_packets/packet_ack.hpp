#ifndef PACKET_ACK_H
#define PACKET_ACK_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "../packet.h"

namespace godot
{
    class PacketAcknowledgement : public Packet
    {

    public:
        PacketAcknowledgement(){
            
        };

        virtual void handle() override
        {
            UtilityFunctions::print(this->get_bytes());
        };
    };
}
#endif
