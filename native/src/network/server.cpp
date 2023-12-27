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
    ClassDB::bind_method(D_METHOD("get_packet_handler"), &Server::get_packet_handler);
    ClassDB::bind_method(D_METHOD("_add_to_tree"), &Server::_add_to_tree);
    ClassDB::bind_method(D_METHOD("load_chunks", "chunk_size", "chunk_cell_radius"), &Server::load_chunks);

    /** Properties */
    ClassDB::bind_method(D_METHOD("get_auth_timeout"), &Server::get_auth_timeout);
    ClassDB::bind_method(D_METHOD("set_auth_timeout", "timeout"), &Server::set_auth_timeout);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "auth_timeout"), "set_auth_timeout", "get_auth_timeout");
}

Server::Server()
{
    singleton = this;
    packet_handler = memnew(PacketHandler);
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
    tcp_server = memnew(TCPServer);

    Ref<TLSOptions> options = TLSOptions::server(key, cert);
    udp_server->listen(port);
    dtls_server->setup(options);

    tcp_server->listen(port);

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

        udp_server->poll();
        connector_udp();
        connector_tcp();

        /** Packet Receiver For Unauthorized Peers */
        for (auto it = unauthorized_peers.begin(); it != unauthorized_peers.end();)
        {
            AuthStruct _struct = it->second;

            Ref<PacketPeerDTLS> dtls_peer = _struct.dtls_peer;
            Ref<StreamPeerTCP> tcp_peer = _struct.tcp_peer;

            long long peer_created_at = _struct.timestamp;

            if (GET_TIME() >= peer_created_at + auth_timeout)
            {
                /** Authentication Timeout */
                disconnect_from_peers(dtls_peer, tcp_peer);
                it = unauthorized_peers.erase(it);
                UtilityFunctions::print("Timeout! Disconnected");
                continue;
            }

            /** DTLS */
            if (!dtls_peer.is_null())
            {
                dtls_peer->poll();
                if (dtls_peer->get_status() == PacketPeerDTLS::STATUS_CONNECTED)
                {
                    while (dtls_peer->get_available_packet_count() > 0)
                    {
                        Array packet = UtilityFunctions::bytes_to_var(dtls_peer->get_packet());
                        if (packet[0] == "AUTH")
                        {
                            String auth_token = packet[1];
                            String user_name = packet[2];

                            if (!_struct.auth_token.is_empty() && !_struct.user_name.is_empty() && !_struct.tcp_peer.is_null())
                            {
                                if (_struct.auth_token == auth_token && _struct.user_name == user_name)
                                {
                                    authenticate(_struct);
                                }
                                else
                                {
                                    disconnect_from_peers(dtls_peer, tcp_peer);
                                }
                            }
                            else
                            {
                                _struct.auth_token = auth_token;
                                _struct.user_name = user_name;
                            }
                        }
                    }
                }
            }

            /** TCP */
            if (!tcp_peer.is_null())
            {
                tcp_peer->poll();
                if (tcp_peer->get_status() == StreamPeerTCP::STATUS_CONNECTED)
                {
                    while (tcp_peer->get_available_bytes() > 0)
                    {
                        Array packet = UtilityFunctions::str_to_var(tcp_peer->get_utf8_string());
                        if (packet[0] == "AUTH")
                        {
                            String auth_token = packet[1];
                            String user_name = packet[2];

                            if (!_struct.auth_token.is_empty() && !_struct.user_name.is_empty() && !_struct.dtls_peer.is_null())
                            {
                                if (_struct.auth_token == auth_token && _struct.user_name == user_name)
                                {
                                    authenticate(_struct);
                                }
                                else
                                {
                                    disconnect_from_peers(dtls_peer, tcp_peer);
                                }
                            }
                            else
                            {
                                _struct.auth_token = auth_token;
                                _struct.user_name = user_name;
                            }
                        }
                    }
                }
            }

            it++;
        }
    }
}

void disconnect_from_peers(Ref<PacketPeerDTLS> dtls_peer, Ref<StreamPeerTCP> tcp_peer)
{
    if (!dtls_peer.is_null())
    {
        dtls_peer->disconnect_from_peer();
    }
    if (!tcp_peer.is_null())
    {
        tcp_peer->disconnect_from_host();
    }
}

void handle_unauthorized_packet_udp()
{
}

void handle_unauthorized_packet_tcp()
{
}

void Server::connector_tcp()
{
    while (tcp_server->is_connection_available())
    {
        Ref<StreamPeerTCP> tcp_peer = tcp_server->take_connection();

        if (tcp_peer->get_status() != StreamPeerTCP::STATUS_CONNECTING)
        {
            continue;
        }

        String ip_address = tcp_peer->get_connected_host();

        UtilityFunctions::print("Peer[TCP] connected with ip: " + ip_address);

        auto it = unauthorized_peers.find(ip_address.utf8().get_data());
        if (it != unauthorized_peers.end())
        {
            /** override */
            AuthStruct _struct = it->second;
            _struct.tcp_peer = tcp_peer;
        }
        else
        {
            /** init */
            unauthorized_peers[ip_address.utf8().get_data()] = AuthStruct(tcp_peer, GET_TIME());
        }
    }
}

void Server::connector_udp()
{
    while (udp_server->is_connection_available())
    {
        Ref<PacketPeerUDP> udp_peer = udp_server->take_connection();
        Ref<PacketPeerDTLS> dtls_peer = dtls_server->take_connection(udp_peer);

        if (dtls_peer->get_status() != PacketPeerDTLS::STATUS_HANDSHAKING)
        {
            continue;
        }
        String ip_address = udp_peer->get_packet_ip();

        UtilityFunctions::print("Peer[UDP/DTLS] connected with ip: " + ip_address);

        auto it = unauthorized_peers.find(ip_address.utf8().get_data());
        if (it != unauthorized_peers.end())
        {
            /** override */
            AuthStruct _struct = it->second;
            _struct.dtls_peer = dtls_peer;
            _struct.udp_peer = udp_peer;
        }
        else
        {
            /** init */
            unauthorized_peers[ip_address.utf8().get_data()] = AuthStruct(dtls_peer, udp_peer, GET_TIME());
        }
    }
}

void authenticate(AuthStruct _struct)
{
}

void Server::_process(double delta)
{
}
