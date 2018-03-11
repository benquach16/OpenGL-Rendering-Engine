#pragma once

#include "vector3.h"


//matrix 4x4
class Mat4
{
	enum MATRIX_TYPES
	{
		IDENTITY,
	};
	Mat4();

	Mat4& translate(Vector3 vector);
    Mat4& rotate(Vector3 vector);
	Mat4& scale(Vector3 vector);
	//operators
	Mat4 operator*(const Mat4 &rhs);
	Mat4 operator*=(const Mat4 &rhs);
	void set(int i, int j, int val);
	float& get(int i, int j);
private:
	void clear();
	void setIdentity();
	inline int get1Dfrom2D(int i, int j);
	int n = 4;
	float mat[16];
};
