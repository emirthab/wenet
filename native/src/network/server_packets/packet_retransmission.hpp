#ifndef PACKET_RETRANSMISSION_H
#define PACKET_RETRANSMISSION_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "../packet.h"

namespace godot
{
    class PacketRetransmission : public Packet
    {

    public:
        PacketRetransmission(){

        };

        virtual void handle() override
        {
            UtilityFunctions::print(this->get_bytes());
        };
    };
}
#endif
