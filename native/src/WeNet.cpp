#include "WeNet.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/property_info.hpp>

using namespace godot;

WeNet * WeNet::singleton;

void WeNet::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("handle_server"), &WeNet::handle_server);

	ClassDB::bind_method(D_METHOD("set_port", "port"), &WeNet::set_port);
	ClassDB::bind_method(D_METHOD("get_port"), &WeNet::get_port);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "port"), "set_port", "get_port");

	ClassDB::bind_method(D_METHOD("get_server"), &WeNet::get_server);
	ClassDB::bind_method(D_METHOD("get_chunk", "x", "y"), &WeNet::get_chunk);
	ClassDB::bind_method(D_METHOD("add_chunk", "chunk"), &WeNet::add_chunk);
	ClassDB::bind_method(D_METHOD("load_chunks", "chunk"), &WeNet::load_chunks);
	ClassDB::bind_method(D_METHOD("get_chunks"), &WeNet::get_chunks);

	ClassDB::bind_method(D_METHOD("get_chunk_cell_radius"), &WeNet::get_chunk_cell_radius);
	ClassDB::bind_method(D_METHOD("set_chunk_cell_radius", "chunk_cell_radius"), &WeNet::set_chunk_cell_radius);

}


void WeNet::handle_server()
{
	server.listen(port);
	server.handling = true;

	UtilityFunctions::print("Server started successfully!");
}

void WeNet::load_chunks(int chunk_size, int cell_radius)
{
	int max_distance_squared = pow(cell_radius, 2) * 2;
	for (int x = -cell_radius; x < cell_radius + 1; x++)
	{
		int x_squared = (pow(x, 2));
		for (int y = -cell_radius; y < cell_radius + 1; y++)
		{
			if (x_squared + pow(y, 2) <= max_distance_squared)
			{
				Chunk *chunk = memnew(Chunk);
				chunk->x = x;
				chunk->y = y;
				this->add_chunk(chunk);
			}
		}
	}
}

void WeNet::_init()
{
	UtilityFunctions::print("initial");
}

void WeNet::_process(double delta)
{
}

Chunk *WeNet::get_chunk(int x, int y)
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

void WeNet::add_chunk(Chunk *chunk)
{
	chunks.push_back(chunk);
	int x = chunk->x;
	int y = chunk->y;
	chunk_map[x][y] = chunk;
}
