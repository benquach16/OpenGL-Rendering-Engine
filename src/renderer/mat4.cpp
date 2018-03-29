#include "mat4.h"

Mat4::Mat4()
{
	clear();
	setIdentity();
}

Mat4& Mat4::translate(const Vector3 &vector)
{
	return *this;
}

void Mat4::clear()
{
	auto n2 = n*n;
	for(int i = 0; i < n2; ++i)
	{
		mat[i] = 0;
	}	
}

Mat4& Mat4::operator*(const Mat4 &rhs)
{
	
	return *this;
}

Mat4& Mat4::operator*=(const Mat4 &rhs)
{
	
	return *this;
}

void Mat4::setIdentity()
{
	clear();
	mat[0]=1;
	mat[5]=1;
	mat[10]=1;
	mat[15]=1;
}

void Mat4::set(int i, int j, int val)
{
	mat[get1Dfrom2D(i,j)] = val;
}

float& Mat4::get(int i, int j)
{
	return mat[get1Dfrom2D(i,j)];
}

Mat4& Mat4::buildPerspectiveMatrix(float aspectRatio, float fov, float near, float far)
{
	clear();
	float h = tan(fov*0.5);
	float w = h / aspectRatio;

	mat[0] = w;
	mat[5] = h;
	mat[10] = far/(far-near);
	mat[14] = (-near*far)/(far-near);
	
	return *this;
}

int Mat4::get1Dfrom2D(int i, int j)
{
	//row major
	return i+j*n;
}

