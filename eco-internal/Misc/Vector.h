#pragma once
#include <json.hpp>
class Vector2
{
public:
	Vector2();
	Vector2(float _x, float _y);
	~Vector2();

	float x, y;

	Vector2 operator *(Vector2 a);
	Vector2 operator /(Vector2 a);
	Vector2 operator +(Vector2 a);
	Vector2 operator -(Vector2 a);
	bool operator ==(Vector2 a);
	bool operator !=(Vector2 a);

	bool IsZero() const;

	static Vector2 Zero();

	static float Distance(Vector2 a, Vector2 b);
};

class Vector3
{
public:
	Vector3();
	Vector3(float _x, float _y, float _z);
	~Vector3();

	float x, y, z;

	Vector3 operator *(Vector3 a);
	Vector3 operator *(float f);
	Vector3 operator /(Vector3 a);
	Vector3 operator /(float f);
	Vector3 operator +(Vector3 a);
	Vector3 operator -(Vector3 a);
	bool operator ==(Vector3 a);
	bool operator !=(Vector3 a);

	bool IsZero() const;
	bool IsValid() const;

	static float Dot(Vector3 left, Vector3 right);
	static float Distance(Vector3 a, Vector3 b);
	static int FormattedDistance(Vector3 a, Vector3 b);
	static Vector3 Zero();
	static Vector3 Lerp(Vector3 a, Vector3 b, float t);

	float Length() const;
	float LengthSqr() const;

	Vector3 Clamp() const;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Vector3, x, y, z)
struct ViewMatrix
{
public:
	float matrix[4][4];

	Vector3 Transform(const Vector3 vector) const;
};

class Vector4
{
public:
	Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	Vector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}

	float x, y, z, w;
};

Vector4 operator*(const ViewMatrix& m, const Vector4& v);