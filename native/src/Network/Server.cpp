#include "Server.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/property_info.hpp>

using namespace godot;

Server *Server::singleton;

void Server::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("start_server", "port"), &Server::start_server);
    ClassDB::bind_method(D_METHOD("get_chunk", "x", "y"), &Server::get_chunk);
    ClassDB::bind_method(D_METHOD("get_packet_handler"), &Server::get_packet_handler);
    ClassDB::bind_method(D_METHOD("_add_to_tree"), &Server::_add_to_tree);
    ClassDB::bind_method(D_METHOD("load_chunks", "chunk_size", "chunk_cell_radius"), &Server::load_chunks);
}

void Server::init()
{
    Engine::get_singleton()->register_singleton("Server", this);
    call_deferred("_add_to_tree");
}

void Server::_add_to_tree()
{
    SCENE_ROOT()->add_child(this);
    SCENE_ROOT()->move_child(this, 0);
}

void Server::start_server(int port)
{
    server = memnew(UDPServer);
    server->listen(port);
    handling = true;
    UtilityFunctions::print("Server started successfully!");
}

Chunk *Server::get_chunk(int x, int y)
{
    auto x_iter = chunk_map.find(x);
    if (x_iter != chunk_map.end())
    {
        auto y_iter = x_iter->second.find(y);
        if (y_iter != x_iter->second.end())
        {
            return y_iter->second;
        }
    }

    // Return nullptr if chunk is not found
    return nullptr;
}

void Server::load_chunks(int chunk_size, int chunk_cell_radius)
{
    this->chunk_size = chunk_size;
    this->chunk_cell_radius = chunk_cell_radius;

    int max_distance_squared = pow(chunk_cell_radius, 2) * 2;
    for (int x = -chunk_cell_radius; x < chunk_cell_radius + 1; x++)
    {
        int x_squared = (pow(x, 2));
        for (int y = -chunk_cell_radius; y < chunk_cell_radius + 1; y++)
        {
            if (x_squared + pow(y, 2) <= max_distance_squared)
            {
                Chunk *chunk = memnew(Chunk);
                chunk->x = x;
                chunk->y = y;
                chunk_map[x][y] = chunk;
            }
        }
    }
}

void Server::_process(double delta)
{
    if (!handling)
    {
        return;
    }

    server->poll();

    if (server->is_connection_available())
    {
        auto peer = server->take_connection();
        auto packet = peer->get_packet();

        String ip_address = peer->get_packet_ip();

        UtilityFunctions::print("Accepted Peer: " + ip_address);
        UtilityFunctions::print("Received Data: " + packet.get_string_from_utf8());

        peer->put_packet(packet);
    }
}