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

Mat4 Mat4::operator*(const Mat4 &rhs)
{
	Mat4 ret;
	for(int row = 0; row < 4; row++)
	{
		for(int col = 0; col <= 12; col+=4)
		{
			for(int inner = 0; inner <4; inner++)
			{
				ret.mat[row+col] += rhs.mat[row+(inner*4)] * mat[col+inner];
			}
		}
	}
	return ret;
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

void Mat4::set(int i, int j, float val)
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
	float h = 1.0/(tan(fov*0.5));
	float w = h / aspectRatio;

	mat[0] = h;
	mat[5] = w;
	mat[10] = far/(far-near);
	mat[14] = (-near*far/(far-near));
	mat[11] = 1;
	
	return *this;
}

int Mat4::get1Dfrom2D(int i, int j)
{
	//column major
	return i*n+j;
}

