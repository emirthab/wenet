#ifndef CLIENT_H
#define CLIENT_H

#include <godot_cpp/classes/ref_counted.hpp>

#include <godot_cpp/classes/packet_peer_udp.hpp>
#include <godot_cpp/classes/packet_peer_dtls.hpp>

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

        void send_packet();
        void send_packet_reliable();

        String _to_string() const
        {
            return "<Client#" + itos(get_instance_id()) + ">";
        }
    };
}

#endif