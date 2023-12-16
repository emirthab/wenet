#include "WeNetServer.h"

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void WeNetServer::_bind_methods()
{
	
}

WeNetServer::WeNetServer() {}
WeNetServer::~WeNetServer() {}

void WeNetServer::_process(double delta)
{
	if (!handling) {
		return;
	}

	this->poll();

	if (this->is_connection_available()) {
		Ref<PacketPeerUDP> peer = Ref<PacketPeerUDP>(this->take_connection());
		String ip_address = peer->get_packet_ip();
	 	PackedByteArray packet = peer->get_packet();
		 
		UtilityFunctions::print("Accepted Peer: " + ip_address);
		UtilityFunctions::print("Received Data: " + packet.get_string_from_utf8());

		peer->put_packet(packet);
	}
}

