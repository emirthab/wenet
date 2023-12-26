#ifndef EVENT_CONNECTION_REQUEST_H
#define EVENT_CONNECTION_REQUEST_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "../packet.h"

namespace godot
{
    class PacketAuthenticationRequest : public Packet
    {

    public:
        PacketAuthenticationRequest(){
            
        };

        virtual void handle() override
        {
            UtilityFunctions::print("Test");
        };
    };
}
#endif
