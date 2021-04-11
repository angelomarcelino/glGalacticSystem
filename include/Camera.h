#ifndef CAMERA_H
#define CAMERA_H

#include <cmath>
#include <iostream>
using namespace std;

// Default camera values
const double YAW = -90.0f;
const double PITCH = 0.0f;
const double SPEED = 0.3;
const double SENSITIVITY = 0.1f;
const double ZOOM = 45.0f;

class Vec3 {

   public:
	double x, y, z;

	// Construtor default
	inline Vec3() : x(), y(), z() {}
	// Construtor por copia
	Vec3(const Vec3 &V);
	// Construtor especifico a partir de um inteiro (dimensao do vetor)
	explicit Vec3(double x, double y, double z);
	// Operador de atribuicao
	void operator=(const Vec3 &V);

	// Norma euclidiana
	double norm() const { return sqrt(x*x + y*y + z*z); }
	// Normaliza o Vetor
	Vec3 normalize() const;

	// Soma de vetores
	Vec3 operator+(const Vec3 &V) const;
	// Subtracao de vetores
	Vec3 operator-(const Vec3 &V) const;
	// Produto escalar (utilizando o *)
	double operator*(const Vec3 &V) const;
	// Produto por escalar (utilizando o *)
	Vec3 operator*(const double num) const;
	// Produto vetorial (utilizando o %)
	Vec3 operator%(const Vec3 &V) const;
};

class Camera {
   public:
	Vec3 eye,       // Position
         center,    // Front
         up,        // Up
         forwardVec;// 

	double speed;

	inline Camera() : eye(), center(), up(), forwardVec(), speed() {}
	Camera(Vec3 *eye, Vec3 *center, Vec3 *up, double speed) : eye(*eye), center(*center), 
                                                            up(*up), speed(speed) {
		forwardVec = this->center - this->eye;
		forwardVec = forwardVec.normalize();
	}

	void forward();
	void backward();
	void left();
	void right();

};

#endif	//CAMERA_H