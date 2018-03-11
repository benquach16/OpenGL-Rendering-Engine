#include "mat4.h"

Mat4::Mat4()
{
	clear();
}

Mat4& Mat4::translate(Vector3 vector)
{
	
}

void Mat4::clear()
{
	auto n2 = n*n;
	for(int i = 0; i < n2; ++i)
	{
		mat[i] = 0;
	}	
}

Mat4 Mat4::operator*(const Mat4 &rhs)
{	
	return *this;
}

Mat4 Mat4::operator*=(const Mat4 &rhs)
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

int Mat4::get1Dfrom2D(int i, int j)
{
	return i+j*n;
}


