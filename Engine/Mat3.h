#pragma once

#include"Vec3.h"

template<typename T> 
class _Mat3
{
public: 
	_Vec3<T> operator * (const _Vec3<T>& v) const
	{
		_Vec3<T> vout; 

		vout.x = cells[0][0] * v.x + cells[0][1] * v.y + cells[0][2] * v.w; 
		vout.y = cells[1][0] * v.x + cells[1][1] * v.y + cells[1][2] * v.w;
		vout.w = cells[2][0] * v.x + cells[2][1] * v.y + cells[2][2] * v.w;

		return vout; 
	}

	static _Mat3 Scale(T factor)
	{
		return
		{
			factor, (T)0,   (T)0,
			(T)0,	factor, (T)0,
			(T)0, (T)0,     (T)1
		};
	}

	static _Mat3 Identity()
	{
		return Scale(1); 
	}

	static _Mat3 FlipY()
	{
		return
		{
			(T)1, (T)0, (T)0,
			(T)0, (T)-1,(T)0,
			(T)0, (T)0, (T)1
		};
	}

	static _Mat3 Rotation( T theta)
	{
		const auto cosTheta = std::cos(theta);
		const auto sinTheta = std::sin(theta);

		return
		{
			cosTheta, -sinTheta, (T)0,
			sinTheta, cosTheta,  (T)0,
			(T)0,	  (T)0,	     (T)1
		}; 
	}

	static _Mat3 Translation(T x, T y)
	{
		return
		{
			(T)1, (T)0, (T)x,
			(T)0, (T)1,(T)y,
			(T)0, (T)0, (T)1
		};
	}

	_Mat3 operator * (const _Mat3& rhs)
	{
		_Mat3 out; 
		for (int row_left = 0; row_left < 3; ++row_left)
		{
			for (int col_right = 0; col_right < 3; col_right++)
			{
				out.cells[row_left][col_right] = (T)0; 

				for (int i = 0; i < 3; ++i)
				{
					out.cells[row_left][col_right] +=
						cells[row_left][i] * rhs.cells[i][col_right];
				}
			}
		}

		return out; 
	}

	_Mat3& operator *= (const _Mat3& rhs)
	{
		return* this = *this * rhs; 
	}




public:
	//[row][col]
	T cells[3][3];

};

using Mat3 = _Mat3<float>; 
using Mad3 = _Mat3<double>;