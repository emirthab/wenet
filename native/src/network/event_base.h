#ifndef EVENT_BASE_H
#define EVENT_BASE_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/packet_peer.hpp>

namespace godot
{
    class EventBase : public RefCounted
    {
        GDCLASS(EventBase, RefCounted)

    private:
        
    protected:
        static void _bind_methods();

    public:
        EventBase(){};
        ~EventBase(){};
        
        virtual void _handle(Array packet,  Ref<PacketPeer> peer){};
        String header;
    };
}
#endif
