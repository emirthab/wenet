#include "event_handler.h"

using namespace godot;

void EventHandler::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("handle_event", "data"), &EventHandler::handle_event);
}

EventHandler::EventHandler()
{
    register_event<EventConnectionRequest>("CON_REQ");
}

template <typename T>
void EventHandler::register_event(const std::string &header)
{
    event_factory[header] = []() -> EventBase *
    { return new T(); };
}

EventBase *EventHandler::create_event(const std::string &header)
{
    auto it = event_factory.find(header);
    if (it != event_factory.end())
    {
        return (it->second)();
    }
    return nullptr;
}

void EventHandler::handle_event(PackedByteArray packet, Ref<PacketPeerUDP> peer)
{
    Array _packet = UtilityFunctions::str_to_var(packet.get_string_from_utf8());

    String event_header = _packet[0];
    
    EventBase *event = create_event(event_header.utf8().get_data());
    if (!event)
    {
        return;
    }
    event->_handle(packet, peer);
    memdelete(event);
    delete event;
}
