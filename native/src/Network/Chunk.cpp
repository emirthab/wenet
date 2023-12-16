#include "Chunk.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void Chunk::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("get_x"), &Chunk::get_x);
	ClassDB::bind_method(D_METHOD("set_x", "x"), &Chunk::set_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "x"), "set_x", "get_x");

	ClassDB::bind_method(D_METHOD("get_y"), &Chunk::get_y);
	ClassDB::bind_method(D_METHOD("set_y", "y"), &Chunk::set_y);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "y"), "set_y", "get_y");

	ClassDB::bind_method(D_METHOD("_to_string"), &Chunk::_to_string);


}
