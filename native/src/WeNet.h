#ifndef WENET_H
#define WENET_H

#include <godot_cpp/classes/object.hpp>
#include "Network/WeNetServer.h"
#include "Network/Chunk.h"
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot
{
	class WeNet : public Object
	{
		GDCLASS(WeNet, Object)

	private:
		int port = 3636;
		int chunk_cell_radius;
		int chunk_size;

		WeNetServer server;
		std::unordered_map<int, std::unordered_map<int, Chunk *>> chunk_map;
		TypedArray<Chunk> chunks;

	protected:
		static WeNet *singleton;
		static void _bind_methods();
		void _init();
		void _process(double delta);

	public:
		WeNet() { singleton = this; };

		static WeNet *get_singleton() { return singleton; };

		void handle_server();

		void load_chunks(int chunk_size, int cell_radius);

		void set_port(int port) { this->port = port; }
		int get_port() { return port; }

		int get_chunk_cell_radius() { return chunk_cell_radius; }
		void set_chunk_cell_radius(int chunk_cell_radius) { this->chunk_cell_radius = chunk_cell_radius; }

		WeNetServer *get_server() { return &server; }
		TypedArray<Chunk> get_chunks() const { return chunks; }

		Chunk *get_chunk(int x, int y);
		void add_chunk(Chunk *chunk);
	};

}

#endif