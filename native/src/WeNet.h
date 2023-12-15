#ifndef WENET_H
#define WENET_H

#include <godot_cpp/classes/object.hpp>
#include "Network/WeNetServer.h"

namespace godot
{
	class WeNet : public Object
	{
		GDCLASS(WeNet, Object)
	
	private:
		int port = 3636;

	protected:
		static void _bind_methods();

	public:
		WeNet();
		~WeNet();

		WeNetServer server = WeNetServer();

		void handleServer();
		void _init();
		void _process(double delta);

		void set_port(int p);
		int get_port();
	};

}

#endif