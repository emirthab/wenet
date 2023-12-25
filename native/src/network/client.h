#ifndef CLIENT_H
#define CLIENT_H

#include <godot_cpp/classes/ref_counted.hpp>

#include <godot_cpp/classes/packet_peer_udp.hpp>
#include <godot_cpp/classes/packet_peer_dtls.hpp>
#include <godot_cpp/classes/stream_peer_tcp.hpp>

namespace godot
{
    class Client : public RefCounted
    {
        GDCLASS(Client, RefCounted)

    private:
    protected:
        static void _bind_methods();

    public:
        Ref<PacketPeerUDP> udp_client;
        Ref<PacketPeerDTLS> dtls_client;
        Ref<StreamPeerTCP> tcp_client;
        
    };
}

#endif