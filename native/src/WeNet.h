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

		static int chunk_cell_radius;
		static int chunk_size;

		WeNetServer server;
		std::unordered_map<int, std::unordered_map<int, Chunk *>> chunk_map;
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

		Chunk *get_chunk(int x, int y);
		void add_chunk(Chunk *chunk);

		void set_port(int port)
		{
			this->port = port;
		}
		int get_port()
		{
			return port;
		}

		WeNetServer* get_server() { return &server; }
		TypedArray<Chunk> get_chunks() const { return chunks; }

		static int get_chunk_cell_radius() { return WeNet::chunk_cell_radius; }
		static void set_chunk_cell_radius(int chunk_cell_radius) { WeNet::chunk_cell_radius = chunk_cell_radius; }
	};

}

#endif