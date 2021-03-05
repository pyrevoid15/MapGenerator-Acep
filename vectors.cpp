#include "vectors.h"
using namespace vectors;

Vector::Vector() {
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

Vector::Vector(double x, double y) {
	this->x = x;
	this->y = y;
	this->z = 0;
}

Vector::Vector(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector::~Vector() {

}

Vector Vector::operator+(Vector v) {
	return Vector(x + v.x, y + v.y, z + v.z);
}

Vector Vector::operator-(Vector v) {
	return Vector(x - v.x, y - v.y, z - v.z);
}

Vector Vector::operator*(double s) {
	return Vector(x * s, y * s, z * s);
}

Vector Vector::operator/(double s) {
	return (s != 0 ? Vector(x / s, y / s, z / s) : *this);
}

double Vector::operator*(Vector v) {
	return v.x * x + v.y * y + v.z * z;
}

Vector Vector::operator^(Vector v) {
	return Vector(y * v.z - z * v.y, (x * v.z - z * v.x) * -1, x * v.y - y * v.x);
}

void Vector::operator=(Vector v) {
	x = v.x;
	y = v.y;
	z = v.z;
}

void Vector::operator+=(Vector v) {
	x += v.x;
	y += v.y;
	z += v.z;
}

void Vector::operator-=(Vector v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

void Vector::operator*=(double s) {
	x *= s;
	y *= s;
	z *= s;
}

void Vector::operator/=(double s) {
	if (s != 0) {
		x /= s;
		y /= s;
		z /= s;
	}
}

double Vector::magnitude() {
	return sqrt(x * x + y * y + z * z);
}

double Vector::get_angle() {
	return atan2(y, x);
}

double Vector::direction() {
	return get_angle();
}

double Vector::get_rel_angle(Vector& v) {
	return std::acos(operator*(v) / v.magnitude() / magnitude());
}

Vector Vector::get_unitvector() {
	double m = magnitude();
	return Vector(x / m, y / m, z / m);
}

void Vector::normalize() {
	double s = magnitude();
	if (s != 0) {
		x /= s;
		y /= s;
		z /= s;
	}
}

Vector Vector::cross_product(Vector& v) {
	return Vector(y * v.z - z * v.y, (x * v.z - z * v.x) * -1, x * v.y - y * v.x);
}

double Vector::dot_product(Vector& v) {
	return v.x * x + v.y * y + v.z * z;
}

OffsetVector::OffsetVector() {
	this->x0 = 0;
	this->x = 0;
	this->y0 = 0;
	this->y = 0;
	this->z0 = 0;
	this->z = 0;
}

OffsetVector::OffsetVector(double x0, double x, double y0, double y) {
	this->x0 = x0;
	this->x = x;
	this->y0 = y0;
	this->y = y;
	this->z0 = 0;
	this->z = 0;
}

OffsetVector::OffsetVector(double x0, double x, double y0, double y, double z0, double z) {
	this->x0 = x0;
	this->x = x;
	this->y0 = y0;
	this->y = y;
	this->z0 = z0;
	this->z = z;
}
