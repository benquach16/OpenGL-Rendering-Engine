#pragma once

#include "vector3.h"
#include <math.h>

//matrix 4x4
class Mat4 {
public:
    enum MATRIX_TYPES {
        IDENTITY,
    };
    Mat4();

    Mat4& translate(const Vector3& vector);
    Mat4& rotate(const Vector3& vector);
    Mat4& scale(const Vector3& vector);

    void lookAt(const Vector3& vector);

    //operators
    Mat4 operator*(const Mat4& rhs);
    Mat4& operator*=(const Mat4& rhs);
    void set(int i, int j, float val);
    float& get(int i, int j);
    float* data() { return &mat[0]; }
    Mat4& buildPerspectiveMatrix(float aspectRatio, float fov, float near, float far);

private:
    void clear();
    void setIdentity();
    inline int get1Dfrom2D(int i, int j);
    int n = 4;
    //this must be stored in COLUMN major for OpenGL, row major for directx
    float mat[16];
};
