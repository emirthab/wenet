#ifndef PACKET_BASE_H
#define PACKET_BASE_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/node3d.hpp>

namespace godot
{
    class PacketBase : public RefCounted
    {
        GDCLASS(PacketBase, RefCounted)

    private:
    protected:
        static void _bind_methods();

    public:
        PacketBase(){};
        ~PacketBase(){};
        virtual void _handle(){};
    };
}
#endif
