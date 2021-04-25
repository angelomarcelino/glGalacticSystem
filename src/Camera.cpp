#include "../include/Camera.h"

/* -------- Vec3 -------- */
// Construtor por copia
Vec3::Vec3(const Vec3 &V) {
	x = V.x;
	y = V.y;
	z = V.z;
}

// Construtor especifico a partir de valores
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
		return Vec3(x / n, y / n, z / n);
	} else {
		cerr << "Vetor Nulo" << endl;
	}
}

/// Impressao de um vetor
ostream &operator<<(ostream &X, const Vec3 &V) {
	X << '[' << V.x << ';' << V.y << ';' << V.z << ']';
	return X;
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

// divisor por escalar (utilizando o /)
Vec3 Vec3::operator/(const double num) const {
	double x1 = x / num;
	double y1 = y / num;
	double z1 = z / num;
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

/* -------- Camera -------- */

void Camera::UpdateVectors() {
	// Update Front
	Vec3 nFront;
	nFront.x = cos(radians(yawValue)) * cos(radians(pitchValue));
	nFront.y = sin(radians(pitchValue));
	nFront.z = sin(radians(yawValue)) * cos(radians(pitchValue));
	front = nFront.normalize();
	// Update Right
	rightVec = front % worldUp;
	rightVec = rightVec.normalize();
	// Update Up
	up = rightVec % front;
	up = up.normalize();
}

void Camera::UpdateCenter() {
	center = pos + front;
	offset = (center + (front / speed)) / speed;
	offset.x = (int)offset.x;
	offset.y = (int)offset.y;
	offset.z = (int)offset.z;
	//cout << offset << endl;
}

Camera::Camera(Vec3 *pos, Vec3 *center, Vec3 *up, double speed) : pos(*pos),
																  center(*center),
																  up(*up),
																  worldUp(*up),
																  speed(speed) {
	this->yawValue = YAW;
	this->pitchValue = PITCH;
	UpdateVectors();
}

// Move pra frente
void Camera::forward() {
	pos = pos + (front * speed);
	UpdateCenter();
}

// Move pra trás
void Camera::backward() {
	pos = pos - (front * speed);
	UpdateCenter();
}

// Move pra esquerda
void Camera::left() {
	pos = pos - (rightVec * speed);
	UpdateCenter();
}

// Move pra direita
void Camera::right() {
	pos = pos + (rightVec * speed);
	UpdateCenter();
}

// Pitch Up and Down
void Camera::pitch(bool dir, double speed) {
	if (dir) {
		pitchValue += speed;
		if (pitchValue > 89.0f) pitchValue = 89.0f;
	} else {
		pitchValue -= speed;
		if (pitchValue < -89.0f) pitchValue = -89.0f;
	}
	UpdateVectors();
	UpdateCenter();
}

// Yaw Left and Right
void Camera::yaw(bool dir, double speed) {
	if (dir) {
		yawValue += speed;
	} else {
		yawValue -= speed;
	}
	UpdateVectors();
	UpdateCenter();
}