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

        /** A list of reliable packets received, contains ack_uid */
        // std::vector<String> ack_queue_recv;

        /** A list of reliable packets sended, contains ack_uid & byte(for retransmission) */
        // std::unordered_map<String, PackedByteArray> ack_queue_send;

        String _to_string() const
        {
            return "<Client#" + itos(get_instance_id()) + ">";
        }
    };
}

#endif