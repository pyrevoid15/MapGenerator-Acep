#pragma once
#include "files.h"

namespace vectors {
	struct Vector {
		double x, y, z;

		Vector();
		Vector(double, double);
		Vector(double, double, double);
		~Vector();

		Vector operator+(Vector); //vector addition
		Vector operator-(Vector); //vector subtraction
		Vector operator*(double); //vector scalar multiplication
		Vector operator/(double); //vector scalar multiplication
		Vector operator^(Vector); //vector cross product
		void operator+=(Vector); //vector addition
		void operator-=(Vector); //vector subtraction
		void operator*=(double); //vector scalar multiplication
		void operator/=(double); //vector scalar multiplication
		void operator=(Vector);

		Vector cross_product(Vector&);
		double dot_product(Vector&);

		double operator*(Vector); //vector dot product
		double get_rel_angle(Vector&);
		double get_angle();
		double magnitude();
		double direction();
		Vector get_unitvector();
		void normalize();

		void rotate_pi(double);
		void rotate_phi(double);
	};

	struct OffsetVector : public Vector { //describes a vector with a starting point not at origin.
		double x0, y0, z0;
		OffsetVector();
		OffsetVector(double, double, double, double);
		OffsetVector(double, double, double, double, double, double);
	};
}