#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/packet_peer_dtls.hpp>
#include "event_base.h"

#include "client_events/event_connection_request.h"

#include <functional>
#include <iostream>
#include <unordered_map>

namespace godot
{
    class EventHandler : public Object
    {
        GDCLASS(EventHandler, Object)

    private:
        std::unordered_map<std::string, std::function<EventBase *()>> event_factory;

    protected:
        static void _bind_methods();

    public:
        EventHandler();

        template <typename T>
        void register_event(const std::string &header);

        EventBase *create_event(const std::string &header);

        void handle_event(PackedByteArray packet, Ref<PacketPeerDTLS> peer);
    };
}

#endif