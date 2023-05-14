#pragma once

struct vector2
{
	float x, y;

	vector2(float X, float Y) : x(X), y(Y) {}
};

struct vector3
{
	double x, y, z, x0 = 0, y0 = 0, z0 = 0;

	vector3() : x(0), y(0), z(0) {}
	vector3(double X, double Y, double Z) : x(X), y(Y), z(Z) {}
	vector3(double X, double Y, double Z, double X0, double Y0, double Z0) : x(X), y(Y), z(Z), x0(X0), y0(Y0), z0(Z0) {}
	vector3(double X, vector2 VECTOR) : x(X), y(VECTOR.x), z(VECTOR.y) {}

	float size()
	{
		return sqrtf(x * x + y * y + z * z);
	}

	float length()
	{
		return sqrtf((x - x0) * (x - x0) + (y - y0) * (y - y0) + (z - z0) * (z - z0));
	}

	void normalize()
	{
		float l = size();
		x = x / l;
		y = y / l;
		z = z / l;
	}

	bool isNull()
	{
		return(x == 0 && y == 0 && z == 0);
	}

	vector3 cross(vector3 v)
	{
		vector3 result;
		result.x0 = x0;
		result.y0 = y0;
		result.z0 = z0;
		result.x = y * v.z - z * v.y;
		result.y = z * v.x - x * v.z;
		result.z = x * v.y - y * v.x;
		return result;
	}

	vector3 start()
	{
		return vector3(x0, y0, z0);
	}

	vector3 operator*(double a) { return vector3(x * a, y * a, z * a, x0, y0, z0); }

	vector3 operator+(vector3 VECTOR) { return vector3(x + VECTOR.x, y + VECTOR.y, z + VECTOR.z, x0, y0, z0); }
	vector3 operator-(vector3 VECTOR) { return vector3(x - VECTOR.x, y - VECTOR.y, z - VECTOR.z, x0, y0, z0); }
	vector3 operator*(vector3 VECTOR) { return vector3(x * VECTOR.x, y * VECTOR.y, z * VECTOR.z, x0, y0, z0); }
	vector3 operator/(vector3 VECTOR) { return vector3(x / VECTOR.x, y / VECTOR.y, z / VECTOR.z, x0, y0, z0); }
	vector3 operator-() { return vector3(-x, -y, -z, x0, y0, z0); }
};

float dot(vector3 v1, vector3 v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float cos_angle(vector3 v1, vector3 v2)
{
	float d = dot(v1, v2);
	float s = v1.size() * v2.size();
	if ((d == 0 && s == 0) || abs(d / s) > 1) return 0;
	return d / s;
}

vector3 getPlaneNormal(vector3 point1, vector3 point2, vector3 point3)
{
	vector3 v1 = point2 - point1;
	vector3 v2 = point3 - point2;
	vector3 planeNormal = v1.cross(v2);
	planeNormal.x0 = point1.x;
	planeNormal.y0 = point1.y;
	planeNormal.z0 = point1.z;
	planeNormal.normalize();
	return planeNormal;
}
