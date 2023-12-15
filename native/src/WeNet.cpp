#include "WeNet.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/property_info.hpp>

using namespace godot;

void WeNet::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("handle_server"), &WeNet::handleServer);

	ClassDB::bind_method(D_METHOD("set_port", "p"), &WeNet::set_port);
	ClassDB::bind_method(D_METHOD("get_port"), &WeNet::get_port);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "port"), "set_port", "get_port");
}

WeNet::WeNet() {}
WeNet::~WeNet() {}

void WeNet::handleServer()
{
	server.listen(port);
	server.handling = true;
	UtilityFunctions::print("Server started successfully!");
}

void WeNet::_init()
{
	UtilityFunctions::print("initial");
}

void WeNet::_process(double delta)
{



}

void godot::WeNet::set_port(int p)
{
	port = p;
}

int godot::WeNet::get_port()
{
	return port;
}

