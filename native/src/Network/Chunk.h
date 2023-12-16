#ifndef CHUNK_H
#define CHUNK_H

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/node.hpp>

namespace godot
{
	class Chunk : public Node
	{
		GDCLASS(Chunk, Node)

	private:


	protected:
		static void _bind_methods();

	public:
		Chunk();
		~Chunk();

		bool sleeping = false;
		int x = 0;
		int y = 0;

		//TypedArray<Chunk> get_neighboors(int cell_radius);

		String _to_string() const {
			return "<Chunk#" + itos(get_instance_id()) + ">";
		}

		// Setter Getters
		void set_x(int x) {
			this->x = x;
		}
		int get_x() const {
			return x;
		}

		void set_y(int y) {
			this->y = y;
		}
		int get_y() const {
			return y;
		}
	};
}

#endif