#include "client.h"

using namespace godot;

void Client::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("_to_string"), &Client::_to_string);
}