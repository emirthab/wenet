#ifndef WENET_SERVER_H
#define WENET_SERVER_H

#include <godot_cpp/classes/udp_server.hpp>
#include <godot_cpp/classes/packet_peer_udp.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>

namespace godot
{
	class WeNetServer : public UDPServer
	{
		GDCLASS(WeNetServer, UDPServer)
	
	private:

	protected:
		static void _bind_methods();

	public:
		WeNetServer();
		~WeNetServer();

		bool handling = false;

		void _process(double delta);
	};

}
#endif