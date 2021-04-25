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
	double norm() const { return sqrt(x * x + y * y + z * z); }
	// Normaliza o Vetor
	Vec3 normalize() const;

	/// Impressao de um vetor
	friend ostream &operator<<(ostream &X, const Vec3 &V);

	// Soma de vetores
	Vec3 operator+(const Vec3 &V) const;
	// Subtracao de vetores
	Vec3 operator-(const Vec3 &V) const;
	// Produto escalar (utilizando o *)
	double operator*(const Vec3 &V) const;
	// Produto por escalar (utilizando o *)
	Vec3 operator*(const double num) const;
	// Divisor por escalar (utilizando o /)
	Vec3 operator/(const double num) const;
	// Produto vetorial (utilizando o %)
	Vec3 operator%(const Vec3 &V) const;
};

class Camera {
   public:
	Vec3 pos,	   // Eye (position of camera)
		center,	   // Center = pos + front (where the camera is looking at)
		front,	   // Front = center - pos (relative to the pos, its the direction to where the camera is looking at)
		rightVec,  // Right = up % (pos - center) (Cross-product between up vector and direction vector)
		up,		   // Up (camera's up)
		worldUp;

	Vec3 offset = Vec3(0, 0, 0);  // Determines the offset for rendering stars

	double yawValue, pitchValue, speed;

	Camera(Vec3 *pos, Vec3 *center, Vec3 *up, double speed = SPEED);

	void forward();
	void backward();
	void left();
	void right();
	void pitch(bool dir, double speed = SPEED);
	void yaw(bool dir, double speed = SPEED);

   private:
	double radians(double deg) { return deg * (M_PI / 180); }
	void UpdateVectors();
	void UpdateCenter();
};
#endif	//CAMERA_H