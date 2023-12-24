#include "server.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/property_info.hpp>

using namespace godot;

Server *Server::singleton;

void Server::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("start_server", "port", "key", "cert"), &Server::start_server);
    ClassDB::bind_method(D_METHOD("get_chunk", "x", "y"), &Server::get_chunk);
    ClassDB::bind_method(D_METHOD("get_event_handler"), &Server::get_event_handler);
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

void Server::start_server(int port, const Ref<CryptoKey> &key, const Ref<X509Certificate> &cert)
{
    server = memnew(UDPServer);
    dtls = memnew(DTLSServer);

    server->listen(port);
    Ref<TLSOptions> options = TLSOptions::server(key, cert);
    dtls->setup(options);

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

void Server::broadcast()
{
}

void Server::_process(double delta)
{
    if(!handling){
        return;
    }

    server->poll();

    while (server->is_connection_available())
    {
        Ref<PacketPeerUDP> peer = server->take_connection();
        Ref<PacketPeerDTLS> dtls_peer = dtls->take_connection(peer);
        if (dtls_peer->get_status() != PacketPeerDTLS::STATUS_HANDSHAKING)
        {
            continue;
        }
        UtilityFunctions::print("Peer connected with ip: " + peer->get_packet_ip());
        peer_list.push_back(*dtls_peer);
    }

    for (int i = 0; i < peer_list.size(); i++)
    {
        Ref<PacketPeerDTLS> peer = peer_list[i];
        peer->poll();
        if (peer->get_status() == PacketPeerDTLS::STATUS_CONNECTED)
        {
            while (peer->get_available_packet_count() > 0)
            {
                event_handler->handle_event(peer->get_packet(), peer);
            }
        }
    }




    // if (server->is_connection_available())
    // {
    //     Ref<PacketPeerUDP> peer = server->take_connection();
    // PackedByteArray packet = peer->get_packet();

    // UtilityFunctions::print(packet.get_string_from_utf8());
    // UtilityFunctions::print(peer);
    // peer->put_packet(packet);

    // peer_list.push_back(*peer);

    // for (int i = 0; i < peer_list.size(); i++)
    // {
    //     UtilityFunctions::print(peer_list[i]);
    // }

    // event_handler->handle_event(packet, peer);

    // String ip_address = peer->get_packet_ip();
    // }
}