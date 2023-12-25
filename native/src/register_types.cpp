#include "register_types.h"
#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "network/chunk.h"
#include "network/client.h"
#include "network/server.h"
#include "network/event_handler.h"

#include "network/event_base.h"
#include "network/client_events/event_connection_request.h"

#include "utils/common.h"

using namespace godot;

Server *ServerPtr = nullptr;

void initialize_example_module(ModuleInitializationLevel p_level)
{

	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
	{
		return;
	}

	ClassDB::register_class<Chunk>();
	ClassDB::register_class<Client>();
	ClassDB::register_class<Server>();
	ClassDB::register_class<EventHandler>();

	ClassDB::register_class<EventBase>();
	ClassDB::register_class<EventConnectionRequest>();

	ServerPtr = memnew(Server);
	ServerPtr->init();
}

void uninitialize_example_module(ModuleInitializationLevel p_level)
{
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
	{
		return;
	}
}

extern "C"
{
	// Initialization.
	GDExtensionBool GDE_EXPORT example_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization)
	{
		godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

		init_obj.register_initializer(initialize_example_module);
		init_obj.register_terminator(uninitialize_example_module);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

		return init_obj.init();
	}
}