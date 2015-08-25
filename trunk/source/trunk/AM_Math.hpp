/*
Zombie Project.  A game.  With Zombies.
Copyright (C) 2007  Matthew Chudleigh

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef _AM_Math_hpp_
#define _AM_Math_hpp_

namespace AutoMatic
{
//****************************************************************************//

	namespace Math
	{
		const float DELTA	=	1e-6f;
		const float PI = 3.1415926f;
		const float PI_180 = 3.1416926f / 180.0f;
		const float _180_PI = 180.0f / 3.1416926f;
		const float HALF_PI = 1.570796326795f;
		const float TWO_PI = 6.28318530718f;
		const float LOGHALF	=	-0.6931471805599f;
		const float LOGHALFI = -1.442695040889f;
	}

	class Radians;
	class Degrees
	{
	public:
		float _val;
		Degrees(void);
		Degrees(float deg);
		Degrees(Degrees const & copy);
		Degrees(Radians const & rad);

		operator float()
		{
			return _val;
		}

		Degrees operator*(float val_) const;
		Degrees operator*(Degrees const & val_) const;
	};

//****************************************************************************//

	class Radians
	{
	public:
		float _val;
		Radians(void);
		Radians(float deg);
		Radians(Radians const & copy);
		Radians(Degrees const & deg);

		operator float()
		{
			return _val;
		}

		Radians const & operator+=(float add_){	_val += add_;	return *this; }
		Radians operator-(void){ return -_val; }
		bool operator<(Radians const & rhs_) const { return _val < rhs_._val; }
		bool operator>(Radians const & rhs_) const { return _val > rhs_._val; }
	};

//****************************************************************************//

	class Vector
	{
	public:
		enum Index { X = 0, Y = 1, Z = 2 };

		Vector(float const * array_);
		Vector(float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f);
		Vector(Vector const & copy_);

		float x(void) const;
		float y(void) const;
		float z(void) const;

		void x(float x_);
		void y(float y_);
		void z(float z_);

		Vector operator+(Vector const & rhs_) const;
		Vector operator-(Vector const & rhs_) const;
		float operator*(Vector const & rhs_) const;
		Vector operator*(float rhs_) const;

		float dot(Vector const & rhs_) const;
		Vector cross(Vector const & rhs_) const;

		float length2(void) const;

		float length(void) const;

		Vector normalise(void) const;

		float const * array(void) const;

	private:
		float _vector[3];
	};

	Vector operator*(float lhs_, Vector const & rhs_);
	Vector Cross(Vector const & lhs_, Vector const & rhs_);

//****************************************************************************//

	Vector Intersect(	float Ax1, float Ay1, float Ax2, float Ay2,
		float Bx1, float By1, float Bx2, float By2);

//****************************************************************************//

	class Matrix
	{
	public:
		enum Index {M11 = 0, M12, M13, M14, 
			M21 = 4, M22, M23, M24,
			M31 = 8, M32, M33, M34,
			M41 = 12, M42, M43, M44};

		Matrix(float m11 = 1, float m12 = 0, float m13 = 0, float m14 = 0,
			float m21 = 0, float m22 = 1, float m23 = 0, float m24 = 0,
			float m31 = 0, float m32 = 0, float m33 = 1, float m34 = 0,
			float m41 = 0, float m42 = 0, float m43 = 0, float m44 = 1);

		Matrix(Matrix const & matrix_);
		Matrix(float const * matrixArray_);

		Matrix(Vector const & v1_, Vector const & v2_, Vector const & v3_);

		void set(float m11 = 1, float m12 = 0, float m13 = 0, float m14 = 0,
			float m21 = 0, float m22 = 1, float m23 = 0, float m24 = 0,
			float m31 = 0, float m32 = 0, float m33 = 1, float m34 = 0,
			float m41 = 0, float m42 = 0, float m43 = 0, float m44 = 1);

		void set(Matrix const & matrix_);

		void set(float const * matrixArray_);

		float get(Index index_) const;
		void set(Index index_, float value_);

		Matrix exchangeYZX(void) const;

		Vector rotate(Vector const & rhs_) const;

		Vector operator*(Vector const & rhs_) const;

		Matrix operator*(Matrix const & rhs_) const;

		Matrix operator*(float rhs_) const;

		Matrix rotate(Matrix const & rhs_) const;	

		float const * array(void) const;

	private:
		float _matrix[16];
	};

//****************************************************************************//

	class RotationX: public Matrix
	{
	public:
		RotationX(Radians angle);
	};

	class RotationY: public Matrix
	{
	public:
		RotationY(Radians angle);
	};

	class RotationZ: public Matrix
	{
	public:
		RotationZ(Radians angle);
	};

//****************************************************************************//
}

#endif //_AM_Math_hpp_