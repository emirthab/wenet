#ifndef WENET_H
#define WENET_H

#include <godot_cpp/classes/object.hpp>
#include "Network/WeNetServer.h"
#include "Network/Chunk.h"

namespace godot
{
	class WeNet : public Object
	{
		GDCLASS(WeNet, Object)
	
	private:
		int port = 3636;
		WeNetServer server;
		std::unordered_map<int, std::unordered_map<int, Chunk*>> chunk_map;
		TypedArray<Chunk> chunks;

	protected:
		static void _bind_methods();
		void _init();
		void _process(double delta);
	public:
		WeNet();
		~WeNet();
		
		void handle_server();

		void load_chunks(int chunk_size, int cell_radius);
		// Setter Getters
		void set_port(int port) {
			this->port = port;
		}
		int get_port() {
			return port;
		}

		WeNetServer* get_server() {
			return &server;
		}

		TypedArray<Chunk> get_chunks() {
			return chunks;
		}

		Chunk* get_chunk(int x, int y);
		void add_chunk(Chunk *chunk);
	};

}

#endif