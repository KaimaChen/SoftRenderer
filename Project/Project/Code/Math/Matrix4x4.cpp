#include "Matrix4x4.h"

Matrix4x4 Matrix4x4::zero = Matrix4x4(
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0
);

Matrix4x4 Matrix4x4::identity = Matrix4x4(
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
);

Matrix4x4::Matrix4x4(float v00, float v01, float v02, float v03,
								float v10, float v11, float v12, float v13,
								float v20, float v21, float v22, float v23,
								float v30, float v31, float v32, float v33)
{
	m[0][0] = v00; m[0][1] = v01; m[0][2] = v02; m[0][3] = v03;
	m[1][0] = v10; m[1][1] = v11; m[1][2] = v12; m[1][3] = v13;
	m[2][0] = v20; m[2][1] = v21; m[2][2] = v22; m[2][3] = v23;
	m[3][0] = v30; m[3][1] = v31; m[3][2] = v32; m[3][3] = v33;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4 &rhs) const
{
	Matrix4x4 result;
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
		{
			result[row][col] = m[row][0] * rhs[0][col] +
										m[row][1] * rhs[1][col] +
										m[row][2] * rhs[2][col] +
										m[row][3] * rhs[3][col];
		}
	}

	return result;
}

Matrix4x4 Matrix4x4::Transposed() const
{
	Matrix4x4 result;
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
		{
			result[row][col] = m[col][row];
		}
	}
	return result;
}

Matrix4x4 Matrix4x4::Transpose()
{
	Matrix4x4 result;
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
		{
			result[row][col] = m[col][row];
		}
	}
	*this = result;
	return result;
}

Matrix4x4 Matrix4x4::Inverse() const
{
	int i, j, k;
	Matrix4x4 s;
	Matrix4x4 t(*this);

	for (i = 0; i < 3; ++i)
	{
		int pivot = i;
		float pivotSize = t[i][i];

		if (pivotSize < 0)
			pivotSize = -pivotSize;

		for (j = i + 1; j < 4; ++j)
		{
			float tmp = t[j][i];

			if (tmp < 0)
				tmp = -tmp;

			if (tmp > pivotSize)
			{
				pivot = j;
				pivotSize = tmp;
			}
		}

		if (pivotSize == 0)
		{
			return Matrix4x4();
		}

		if (pivot != i)
		{
			for (j = 0; j < 4; ++j) {
				float tmp;
				
				tmp = t[i][j];
				t[i][j] = t[pivot][j];
				t[pivot][j] = tmp;

				tmp = s[i][j];
				s[i][j] = s[pivot][j];
				s[pivot][j] = tmp;
			}
		}

		for (j = i + 1; j < 4; ++j)
		{
			float f = t[j][i] / t[i][i];

			for (k = 0; k < 4; ++k)
			{
				t[j][k] -= f * t[i][k];
				s[j][k] -= f * s[i][k];
			}
		}
	}

	for (i = 3; i >= 0; --i)
	{
		float f;

		if ((f = t[i][i]) == 0) 
		{
			return Matrix4x4();
		}

		for (j = 0; j < 4; ++j)
		{
			t[i][j] /= f;
			s[i][j] /= f;
		}

		for (j = 0; j < i; ++j)
		{
			f = t[j][i];

			for (k = 0; k < 4; ++k)
			{
				t[j][k] -= f * t[i][k];
				s[j][k] -= f * s[i][k];
			}
		}
	}

	return s;
}

Matrix4x4 Matrix4x4::InverseTranspose() const
{
	return this->Inverse().Transposed();
}

Matrix4x4 Matrix4x4::Scale(float sx, float sy, float sz)
{
	Matrix4x4 result = identity;
	result[0][0] = sx;
	result[1][1] = sy;
	result[2][2] = sz;
	return result;
}

Matrix4x4 Matrix4x4::Translate(float tx, float ty, float tz)
{
	Matrix4x4 result = identity;
	result[3][0] = tx;
	result[3][1] = ty;
	result[3][2] = tz;
	return result;
}

Matrix4x4 Matrix4x4::RotateX(float radians)
{
	float c = cosf(radians);
	float s = sinf(radians);
	Matrix4x4 result = identity;
	result[1][1] = c;
	result[1][2] = s;
	result[2][1] = -s;
	result[2][2] = c;
	return result;
}

Matrix4x4 Matrix4x4::RotateY(float radians)
{
	float c = cosf(radians);
	float s = sinf(radians);
	Matrix4x4 result = identity;
	result[0][0] = c;
	result[0][2] = -s;
	result[2][0] = s;
	result[2][2] = c;
	return result;
}

Matrix4x4 Matrix4x4::RotateZ(float radians)
{
	float c = cosf(radians);
	float s = sinf(radians);
	Matrix4x4 result = identity;
	result[0][0] = c;
	result[0][1] = s;
	result[1][0] = -s;
	result[1][1] = c;
	return result;
}

Matrix4x4 Matrix4x4::LookAtLH(const Vector4 &eye, const Vector4 &at, const Vector4 &up)
{
	Vector4 zaxis = (at - eye).Normalize();
	Vector4 xaxis = (up.Cross(zaxis)).Normalize();
	Vector4 yaxis = zaxis.Cross(xaxis);
	float tx = -xaxis.Dot(eye);
	float ty = -yaxis.Dot(eye);
	float tz = -zaxis.Dot(eye);

	return Matrix4x4(
		xaxis.x, yaxis.x, zaxis.x, 0,
		xaxis.y, yaxis.y, zaxis.y, 0,
		xaxis.z, yaxis.z, zaxis.z, 0,
		tx, ty, tz, 1
	);
}

Matrix4x4 Matrix4x4::LookAtRH(const Vector4 &eye, const Vector4 &at, const Vector4 &up)
{
	Vector4 zaxis = (eye - at).Normalize();
	Vector4 xaxis = (up.Cross(zaxis)).Normalize();
	Vector4 yaxis = zaxis.Cross(xaxis);
	float tx = -xaxis.Dot(eye);
	float ty = -yaxis.Dot(eye);
	float tz = -zaxis.Dot(eye);

	return Matrix4x4(
		xaxis.x, yaxis.x, zaxis.x, 0,
		xaxis.y, yaxis.y, zaxis.y, 0,
		xaxis.z, yaxis.z, zaxis.z, 0,
		tx, ty, tz, 1
	);
}

Matrix4x4 Matrix4x4::PerspectiveLH(float w, float h, float zn, float zf)
{
	return Matrix4x4(
		2 * zn / w, 0, 0, 0,
		0, 2 * zn / h, 0, 0,
		0, 0, zf / (zf - zn), 1,
		0, 0, zn * zf / (zn - zf), 0
	);
}

Matrix4x4 Matrix4x4::PerspectiveRH(float w, float h, float zn, float zf)
{
	return Matrix4x4(
		2 * zn / w, 0, 0, 0,
		0, 2 * zn / h, 0, 0,
		0, 0, zf / (zn - zf), -1,
		0, 0, zn * zf / (zn - zf), 0
	);
}

Matrix4x4 Matrix4x4::PerspectiveFovLH(float fov, float aspect, float zn, float zf)
{
	float yScale = 1 / tan(fov * 0.5f);
	float xScale = yScale / aspect;

	return Matrix4x4(
		xScale, 0, 0, 0,
		0, yScale, 0, 0,
		0, 0, zf / (zf - zn), 1,
		0, 0, zn * zf / (zn - zf), 0
	);
}

Matrix4x4 Matrix4x4::PerspectiveFovRH(float fov, float aspect, float zn, float zf)
{
	float yScale = 1 / tan(fov * 0.5f);
	float xScale = yScale / aspect;

	return Matrix4x4(
		xScale, 0, 0, 0,
		0, yScale, 0, 0,
		0, 0, zf / (zn - zf), -1,
		0, 0, zn * zf / (zn - zf), 0
	);
}

Matrix4x4 Matrix4x4::ScreenTransform(int screenWidth, int screenHeight)
{
	float halfWidth = screenWidth * 0.5f;
	float halfHeight = screenHeight * 0.5f;
	return Matrix4x4(
		halfWidth, 0, 0, 0,
		0, halfHeight, 0, 0,
		0, 0, 1, 0,
		halfWidth, halfHeight, 0, 1
	);
}

std::ostream &operator<<(std::ostream &s, const Matrix4x4 &m)
{
	std::ios_base::fmtflags oldFlags = s.flags();
	int width = 12;
	s.precision(5);
	s.setf(std::ios_base::fixed);

	s << "(" << std::setw(width) << m[0][0] <<
		" " << std::setw(width) << m[0][1] <<
		" " << std::setw(width) << m[0][2] <<
		" " << std::setw(width) << m[0][3] << "\n" <<

		" " << std::setw(width) << m[1][0] <<
		" " << std::setw(width) << m[1][1] <<
		" " << std::setw(width) << m[1][2] <<
		" " << std::setw(width) << m[1][3] << "\n" <<

		" " << std::setw(width) << m[2][0] <<
		" " << std::setw(width) << m[2][1] <<
		" " << std::setw(width) << m[2][2] <<
		" " << std::setw(width) << m[2][3] << "\n" <<

		" " << std::setw(width) << m[3][0] <<
		" " << std::setw(width) << m[3][1] <<
		" " << std::setw(width) << m[3][2] <<
		" " << std::setw(width) << m[3][3] << ")\n";

	s.flags(oldFlags);
	return s;
}