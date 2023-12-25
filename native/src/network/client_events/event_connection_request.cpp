#include "event_connection_request.h"
#include "../../utils/common.h"

#include <iostream>
#include <future>
#include <chrono>

using namespace godot;

void EventConnectionRequest::_bind_methods()
{
}

void EventConnectionRequest::_handle(Array packet, Client *client)
{
    UtilityFunctions::print("test override");

}
