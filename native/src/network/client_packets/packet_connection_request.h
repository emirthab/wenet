#ifndef PACKET_CONNECTION_REQUEST_H
#define PACKET_CONNECTION_REQUEST_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include "../packet_base.h"

namespace godot
{
    class PacketConnectionRequest : public PacketBase
    {
        GDCLASS(PacketConnectionRequest, PacketBase)

    private:
    protected:
        static void _bind_methods();

    public:
        PacketConnectionRequest(){};
        ~PacketConnectionRequest(){};
        
        virtual void _handle() override;
    };
}
#endif
