#include "Client.h"
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

Client *Client::singleton = nullptr;

void Client::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("connect_to_server", "ip_address", "port"), &Client::connect_to_server);
    ClassDB::bind_method(D_METHOD("_add_to_tree"), &Client::_add_to_tree);
}

void Client::connect_to_server(String ip_address, int port)
{
    client->connect_to_host(ip_address, port);
}

void Client::_add_to_tree()
{
    SCENE_ROOT()->add_child(this);
    SCENE_ROOT()->move_child(this, 0);
}

void Client::init()
{
    set_process_priority(INT32_MAX);
    Engine::get_singleton()->register_singleton("Client", this);
    call_deferred("_add_to_tree");
}

void Client::_process(double delta)
{
    if (client->get_available_packet_count() > 0)
    {
        UtilityFunctions::print(client->get_packet().get_string_from_utf8());
        connected = true;
    }
    if (!connected)
    {
        client->put_packet(String("Test packet").to_utf8_buffer());
    }
}

void Client::_ready()
{
}
