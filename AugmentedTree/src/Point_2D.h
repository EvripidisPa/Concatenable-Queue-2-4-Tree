#pragma once
#include <iostream>

typedef double coord_t;

class Point_2D
{
	/**
	 * @author Evripidis Pavlidis
	 * @since 2020-5-12
	 */

	coord_t x, y;
public:

	// We are ordering by value of y coord
	bool operator <(const Point_2D& p) const {
		return y < p.y || (y == p.y && x < p.x);
	}

	bool operator == (const Point_2D& p) const {
		return x == p.x && y == p.y;
	}

	bool operator <= (const Point_2D& p) const {
		return  y < p.y || (y == p.y && x <= p.x);
	}

	Point_2D() { 
		this->x = NULL; 
		this->y = NULL; 
	};

	Point_2D(double x, double y) {
		this->x = x;
		this->y = y;
	}

	Point_2D(const Point_2D& other) {
		this->x = other.x;
		this->y = other.y;
	}

	friend std::ostream& operator<< (std::ostream& out, const Point_2D& point);

};

inline std::ostream& operator<< (std::ostream& out, const Point_2D& point)
{
	out << "(" << point.x << ", " << point.y << ")";

	return out;
}