#include "event_connection_request.h"

using namespace godot;

void EventConnectionRequest::_bind_methods()
{

}

void EventConnectionRequest::_handle(Array packet, Ref<PacketPeerUDP> peer)
{
    UtilityFunctions::print("test override");
}

