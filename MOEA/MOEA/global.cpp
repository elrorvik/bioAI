#include"global.h"
#include<vector>

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

bool operator==(pos& left, pos& right) {
	return (left.x == right.x) && (left.y == right.y);
}

bool operator==(RGB& left, RGB& right) {
	return (left.r == right.r) && (left.g == right.g) && (left.b == right.b);
}

bool operator<(const pos& left, const pos& right) {
	return left.x  < right.x;
}

pos operator+(pos& left, pos& right) {
	pos temp;
	temp.x = left.x + right.x;
	temp.y = left.y + right.y;
	return temp;
}

bool operator!=(pos& left, pos & right) {
	return !(left==right);
}



bool operator<(edge e1, edge e2) {
	return e1.RGBdist < e2.RGBdist;
}


direction get_neighbor_dir(pos& origin, pos& neighbor) {
	bool up = origin.y == neighbor.y + 1;
	bool down = origin.y == neighbor.y - 1;
	bool left = origin.x == neighbor.x + 1;
	bool right = origin.x == neighbor.x - 1;
	if (up) return UP;
	if (down) return DOWN;
	if (left) return LEFT;
	if (right) return RIGHT;
	return SELF; // If up + down + left + right != 1, return SELF because they are not neighbors
}

/*
struct edge_comperator {
	// members
	bool operator==(const monster& l, const monster& r) const
	{
		return l.id == r.id;
	}
};
*/