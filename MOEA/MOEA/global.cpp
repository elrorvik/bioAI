#include"global.h"

pos operator+(pos coord, direction dir) {
switch (dir) {
case SELF:
	return coord;
case UP:
	coord.y -= 1;
	return coord;
case DOWN:
	coord.y += 1;
	return coord;
case LEFT:
	coord.x -= 1;
	return coord;
case RIGHT:
	coord.x += 1;
	return coord;
};
return coord;
};

void operator+(node &n, direction d) {
	switch (d) {
	case SELF:
		return;
	case UP:
		n.up = true;
		return;
	case DOWN:
		n.down = true;
		return;
	case LEFT:
		n.left = true;
		return;
	case RIGHT:
		n.right = true;
		return;
	}
}

void operator-(node &n, direction d) {
	switch (d) {
	case SELF:
		return;
	case UP:
		n.up = false;
		return;
	case DOWN:
		n.down = false;
		return;
	case LEFT:
		n.left = false;
		return;
	case RIGHT:
		n.right = false;
		return;
	}
}