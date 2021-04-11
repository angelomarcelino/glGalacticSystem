#include "../include/Camera.h"

// Construtor por copia
Vec3::Vec3(const Vec3 &V) {
	x = V.x;
	y = V.y;
	z = V.z;
}

// Construtor especifico a partir de um inteiro (dimensao do vetor)
Vec3::Vec3(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

// Operador de atribuicao
void Vec3::operator=(const Vec3 &V) {
	x = V.x;
	y = V.y;
	z = V.z;
}

// Normaliza o Vetor
Vec3 Vec3::normalize() const {
	double n = norm();
	if (n > 0) {
		return Vec3(x/n, y/n, z/n);
	} else {
		cerr << "Vetor Nulo" << endl;
	}
}

// Soma de vetores
Vec3 Vec3::operator+(const Vec3 &V) const {
	double x1 = x + V.x;
	double y1 = y + V.y;
	double z1 = z + V.z;
	Vec3 prov(x1, y1, z1);
	return prov;
}

// Subtracao de vetores
Vec3 Vec3::operator-(const Vec3 &V) const {
	double x1 = x - V.x;
	double y1 = y - V.y;
	double z1 = z - V.z;
	Vec3 prov(x1, y1, z1);
	return prov;
}

// Produto escalar (utilizando o *)
double Vec3::operator*(const Vec3 &V) const {
	double prov(0.0);
	prov += x * V.x;
	prov += y * V.y;
	prov += z * V.z;
	return prov;
}

// Produto por escalar (utilizando o *)
Vec3 Vec3::operator*(const double num) const {
	double x1 = x * num;
	double y1 = y * num;
	double z1 = z * num;
	Vec3 prov(x1, y1, z1);
	return prov;
}

// Produto vetorial (utilizando o %)
Vec3 Vec3::operator%(const Vec3 &V) const {
	double x1 = y * V.z - z * V.y;
	double y1 = z * V.x - x * V.z;
	double z1 = x * V.y - y * V.x;
	Vec3 prov(x1, y1, z1);
	return prov;
}

// Move pra frente
void Camera::forward() {
	forwardVec = center - eye;
	forwardVec = forwardVec.normalize();

	eye = eye + (forwardVec * speed);
	center = center + (forwardVec * speed);
	return;
}

// Move pra trás
void Camera::backward() {
	forwardVec = center - eye;
	forwardVec = forwardVec.normalize();

	eye = eye - (forwardVec * speed);
	center = center - (forwardVec * speed);
	return;
}

// Move pra esquerda
void Camera::left() {
	Vec3 leftVec = up % forwardVec;
	leftVec = leftVec.normalize();

	eye = eye + (leftVec * speed);
	center = center + (leftVec * speed);
	return;
}

// Move pra direita
void Camera::right() {
	Vec3 rightVec = forwardVec % up;
	rightVec = rightVec.normalize();

	eye = eye + (rightVec * speed);
	center = center + (rightVec * speed);
	return;
}