#pragma once

#include "Matrix.h"

MATRIX MATRIX::operator*(const float a[4][4])
{
	MATRIX result = MATRIX(NULL);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
				result.m[i][j] += m[i][k] * a[k][j];

	return result;
}

VECTOR3D MATRIX::operator*(const VECTOR3D v)
{
	VECTOR3D result;
	result.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z;
	result.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z;
	result.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z;
	return result;
}

MATRIX MATRIX::transpos()
{
	MATRIX result = MATRIX(NULL);

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			result.m[i][j] = m[j][i];
		}
	return result;
}
