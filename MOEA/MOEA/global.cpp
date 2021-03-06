#include"global.h"
#include<vector>

void RGB::operator=(RGB other) {
	this->r = other.r;
	this->g = other.g;
	this->b = other.b;
}

RGB operator+(RGB a, RGB b) {
	return RGB(a.r + b.r, a.g + b.g, a.b + b.b);
}
RGB operator/(RGB numerator, double denominator) {
	return RGB(numerator.r / denominator, numerator.g / denominator, numerator.b / denominator);
}
bool operator==(RGB& left, RGB& right) {
	return (left.r == right.r) && (left.g == right.g) && (left.b == right.b);
}

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

bool operator<(pos p1, pos p2) {
	if (p1.y == p2.y) {
		return p1.x < p2.x;
	}
	return p1.y < p2.y;
}

bool operator ==(edge e1, edge e2) {
	if (e1.p1 == e2.p2 && e1.p2 == e2.p1) {
		return true;
	}if (e1.p1 == e2.p1 && e1.p2 == e2.p2) {
		return true;
	}
	return false;
}


direction get_neighbor_dir(pos& origin, pos& neighbor) {
	bool up = origin.y - 1 == neighbor.y;
	bool down = origin.y + 1 == neighbor.y;
	bool left = origin.x - 1 == neighbor.x;
	bool right = origin.x + 1 == neighbor.x;
	if (up + down + left + right != 1) return SELF;
	if (up) return UP;
	if (down) return DOWN;
	if (left) return LEFT;
	if (right) return RIGHT;
	return SELF;
}
