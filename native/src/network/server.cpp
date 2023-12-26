#include "server.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/property_info.hpp>

using namespace godot;

Server *Server::singleton;

void Server::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("authenticator"), &Server::authenticator);
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
    udp_server = memnew(UDPServer);
    dtls_server = memnew(DTLSServer);

    Ref<TLSOptions> options = TLSOptions::server(key, cert);
    udp_server->listen(port);
    dtls_server->setup(options);

    auth_thread.instantiate();
    Callable callable_auth = Callable(this, "authenticator");
    auth_thread->start(callable_auth, Thread::PRIORITY_LOW);

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

void Server::authenticator()
{
    while (true)
    {
        if (!handling)
        {
            continue;
        }

        /** UDP Connection Implementations*/

        udp_server->poll();

        while (udp_server->is_connection_available())
        {
            Ref<PacketPeerUDP> udp_peer = udp_server->take_connection();
            Ref<PacketPeerDTLS> dtls_peer = dtls_server->take_connection(udp_peer);

            if (dtls_peer->get_status() != PacketPeerDTLS::STATUS_HANDSHAKING)
            {
                continue;
            }

            UtilityFunctions::print("Peer[UDP/DTLS] connected with ip: " + udp_peer->get_packet_ip());

            unauthorized_dtls_peers.push_back(*dtls_peer);
        }

        /** Packet Receiver For Unauthorized Peers */

        
        for (int i = 0, size = unauthorized_dtls_peers.size(); i < size; i++)
        {
            Ref<PacketPeerDTLS> dtls_peer = unauthorized_dtls_peers[i];

            dtls_peer->poll();
            if (dtls_peer->get_status() == PacketPeerDTLS::STATUS_CONNECTED)
            {
                while (dtls_peer->get_available_packet_count() > 0)
                {
                    Array packet = UtilityFunctions::str_to_var(dtls_peer->get_packet().get_string_from_utf8());
                    UtilityFunctions::print("RECEIVED DATA");
                }
            }
        }
    }
}

void Server::authenticate(Ref<PacketPeerDTLS> peer, const String &auth_token, const String &user_name)
{

}

void Server::_process(double delta)
{
    
}
