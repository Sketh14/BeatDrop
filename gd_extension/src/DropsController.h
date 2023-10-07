#ifndef DROPSCONTROLLER_H
#define DROPSCONTROLLER_H

#include <godot_cpp/classes/node.hpp>

using namespace godot;

class DropsController :public Node
{
	GDCLASS(DropsController, Node);

public:
	unsigned short dropID;

protected:
	static void _bind_methods() {};

private:
	DropsController();
};

#endif
