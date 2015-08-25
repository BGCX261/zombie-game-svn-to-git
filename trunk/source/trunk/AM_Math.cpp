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

#include <cmath>
#include "AM_Math.hpp"

namespace AutoMatic
{
//****************************************************************************//
	
	Degrees::Degrees(void) : _val(0) {}
	Degrees::Degrees(float deg) : _val(deg) {}
	Degrees::Degrees(Degrees const & copy) : _val(copy._val)	{}
	Degrees::Degrees(Radians const & rad) : _val(rad._val * Math::_180_PI) {}

	Degrees Degrees::operator*(float val_) const
	{
		return _val * val_;
	}

	Degrees Degrees::operator*(Degrees const & val_) const
	{
		return _val * val_._val;
	}

	Radians::Radians(void) : _val(0) {}
	Radians::Radians(float deg) : _val(deg) {}
	Radians::Radians(Radians const & copy) : _val(copy._val)	{}
	Radians::Radians(Degrees const & deg) : _val(deg._val * Math::PI_180) {}

//****************************************************************************//

	Vector::Vector(float const * array_)
	{
		x(array_[0]); y(array_[1]); z(array_[2]);
	}

	Vector::Vector(float x_, float y_, float z_)
	{
		x(x_); y(y_); z(z_);
	}

	Vector::Vector(Vector const & copy_)
	{
		x(copy_.x()); y(copy_.y()); z(copy_.z());
	}

	float Vector::x(void) const
	{
		return _vector[X];
	}

	float Vector::y(void) const
	{
		return _vector[Y];
	}

	float Vector::z(void) const
	{
		return _vector[Z];
	}

	void Vector::x(float x_)
	{
		_vector[X] = x_;
	}

	void Vector::y(float y_)
	{
		_vector[Y] = y_;
	}

	void Vector::z(float z_)
	{
		_vector[Z] = z_;
	}

	Vector Vector::operator+(Vector const & rhs_) const
	{
		return Vector(x() + rhs_.x(), y() + rhs_.y(), z() + rhs_.z());
	}

	Vector Vector::operator-(Vector const & rhs_) const
	{
		return Vector(x() - rhs_.x(), y() - rhs_.y(), z() - rhs_.z());
	}

	float Vector::operator*(Vector const & rhs_) const
	{
		return (x() * rhs_.x() + y() * rhs_.y() + z() * rhs_.z());
	}

	Vector Vector::operator*(float rhs_) const
	{
		return Vector(x() * rhs_, y() * rhs_, z() * rhs_);
	}

	Vector operator*(float lhs_, Vector const & rhs_)
	{
		return Vector(rhs_.x() * lhs_, rhs_.y() * lhs_, rhs_.z() * lhs_);
	}

	float Vector::dot(Vector const & rhs_) const
	{
		return x()*rhs_.x()+y()*rhs_.y()+z()*rhs_.z();
	}

	Vector Vector::cross(Vector const & rhs_) const { return Cross(*this,rhs_); }

	float Vector::length2(void) const
	{
		return x()*x() + y()*y() + z()*z();
	}

	float Vector::length(void) const 
	{
		return sqrt(length2());
	}

	Vector Vector::normalise(void) const
	{
		float len = length2();
		if(len > 0.0f)
		{
			return (*this) * (1/sqrt(len));
		}

		return (*this);
	}

	float const * Vector::array(void) const
	{
		return _vector;
	}

	Vector Cross(Vector const & lhs_, Vector const & rhs_) 
	{
		return Vector(
			lhs_.y()*rhs_.z()-lhs_.z()*rhs_.y(),
			lhs_.z()*rhs_.x()-lhs_.x()*rhs_.z(),
			lhs_.x()*rhs_.y()-lhs_.y()*rhs_.x()
			);
	}

//****************************************************************************//
	
	Vector Intersect(	float Ax1, float Ay1, float Ax2, float Ay2,
		float Bx1, float By1, float Bx2, float By2)
	{
		float m = (Ax2 - Ax1);
		if(m != 0.0f){ m = (Ay2 - Ay1) / m; }
		float c = Ay1 - (m * Ax1);

		float n = (Bx2 - Bx1);
		if(n != 0.0f){ n = (By2 - By1) / n; }
		float d = By1 - (n * Bx1);

		Vector out;
		if( (n-m) != 0){ out.x( (c - d) / (n - m) ); }

		out.y(m*out.x() + c);

		return out;
	}


//****************************************************************************//

	Matrix::Matrix(float m11, float m12, float m13, float m14,
		float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34,
		float m41, float m42, float m43, float m44)
	{  
		set(m11,m12,m13,m14,m21,m22,m23,m24,m31,m32,m33,m34,m41,m42,m43,m44); 
	}

	Matrix::Matrix(Matrix const & matrix_){ set(matrix_);}
	Matrix::Matrix(float const * matrixArray_){ set( matrixArray_ ) ; }

	Matrix::Matrix(Vector const & v1_, Vector const & v2_, Vector const & v3_)
	{
		set(v1_.x(), v2_.x(), v3_.x(), 0,
			v1_.y(), v2_.y(), v3_.y(), 0,
			v1_.z(), v2_.z(), v3_.z(), 0,
			0, 0, 0, 1);
	}

	void Matrix::set(float m11, float m12, float m13, float m14,
		float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34,
		float m41, float m42, float m43, float m44) 
	{
		set(M11, m11); set(M12, m12); set(M13, m13); set(M14, m14);
		set(M21, m21); set(M22, m22); set(M23, m23); set(M24, m24);
		set(M31, m31); set(M32, m32); set(M33, m33); set(M34, m34);
		set(M41, m41); set(M42, m42); set(M43, m43); set(M44, m44);
	}

	void Matrix::set(Matrix const & matrix_) 
	{
		set(matrix_.get(M11), matrix_.get(M12), matrix_.get(M13), matrix_.get(M14),
			matrix_.get(M21), matrix_.get(M22), matrix_.get(M23), matrix_.get(M24),
			matrix_.get(M31), matrix_.get(M32), matrix_.get(M33), matrix_.get(M34),
			matrix_.get(M41), matrix_.get(M42), matrix_.get(M43), matrix_.get(M44));
	}

	void Matrix::set(float const * matrixArray_)
	{
		set(matrixArray_[M11], matrixArray_[M12], matrixArray_[M13], matrixArray_[M14],
			matrixArray_[M21], matrixArray_[M22], matrixArray_[M23], matrixArray_[M24],
			matrixArray_[M31], matrixArray_[M32], matrixArray_[M33], matrixArray_[M34],
			matrixArray_[M41], matrixArray_[M42], matrixArray_[M43], matrixArray_[M44]);
	}

	float Matrix::get(Index index_) const { return _matrix[index_]; }
	void Matrix::set(Index index_, float value_){ _matrix[index_] = value_; }

	Matrix Matrix::exchangeYZX(void) const
	{
		return Matrix
			(get(M21), get(M22), get(M23), get(M14),
			get(M31), get(M32), get(M33), get(M24),
			get(M11), get(M12), get(M13), get(M34),
			get(M41), get(M42), get(M43), get(M44));
	}

	Vector Matrix::rotate(Vector const & rhs_) const
	{
		return Vector(rhs_.x()*get(M11)+rhs_.y()*get(M21)+rhs_.z()*get(M31),
			rhs_.x()*get(M12)+rhs_.y()*get(M22)+rhs_.z()*get(M32),
			rhs_.x()*get(M13)+rhs_.y()*get(M23)+rhs_.z()*get(M33));
	}

	Vector Matrix::operator*(Vector const & rhs_) const
	{
		return Vector(
			rhs_.x()*get(M11)+rhs_.y()*get(M21)+rhs_.z()*get(M31)+get(M41),
			rhs_.x()*get(M12)+rhs_.y()*get(M22)+rhs_.z()*get(M32)+get(M42),
			rhs_.x()*get(M13)+rhs_.y()*get(M23)+rhs_.z()*get(M33)+get(M43)
			);
	}

	Matrix Matrix::operator*(Matrix const & rhs_) const
	{
		return Matrix(
			get(M11)*rhs_.get(M11)+get(M12)*rhs_.get(M21)+get(M13)*rhs_.get(M31),
			get(M11)*rhs_.get(M12)+get(M12)*rhs_.get(M22)+get(M13)*rhs_.get(M32),
			get(M11)*rhs_.get(M13)+get(M12)*rhs_.get(M23)+get(M13)*rhs_.get(M33),
			0,
			get(M21)*rhs_.get(M11)+get(M22)*rhs_.get(M21)+get(M23)*rhs_.get(M31),
			get(M21)*rhs_.get(M12)+get(M22)*rhs_.get(M22)+get(M23)*rhs_.get(M32),
			get(M21)*rhs_.get(M13)+get(M22)*rhs_.get(M23)+get(M23)*rhs_.get(M33),
			0,
			get(M31)*rhs_.get(M11)+get(M32)*rhs_.get(M21)+get(M33)*rhs_.get(M31),
			get(M31)*rhs_.get(M12)+get(M32)*rhs_.get(M22)+get(M33)*rhs_.get(M32),
			get(M31)*rhs_.get(M13)+get(M32)*rhs_.get(M23)+get(M33)*rhs_.get(M33),
			0,
			get(M41)*rhs_.get(M11)+get(M42)*rhs_.get(M21)+get(M43)*rhs_.get(M31)+rhs_.get(M41),
			get(M41)*rhs_.get(M12)+get(M42)*rhs_.get(M22)+get(M43)*rhs_.get(M32)+rhs_.get(M42),
			get(M41)*rhs_.get(M13)+get(M42)*rhs_.get(M23)+get(M43)*rhs_.get(M33)+rhs_.get(M43),
			1
			);
	}

	Matrix Matrix::operator*(float rhs_) const
	{
		return Matrix(
			get(M11)*rhs_,get(M12)*rhs_,get(M13)*rhs_,0,
			get(M21)*rhs_,get(M22)*rhs_,get(M23)*rhs_,0,
			get(M32)*rhs_,get(M32)*rhs_,get(M33)*rhs_,0,
			get(M41)  ,get(M42)  ,get(M43)  ,1);
	}


	Matrix Matrix::rotate(Matrix const & rhs_) const
	{
		return Matrix(
			get(M11)*rhs_.get(M11)+get(M12)*rhs_.get(M21)+get(M13)*rhs_.get(M31),
			get(M11)*rhs_.get(M12)+get(M12)*rhs_.get(M22)+get(M13)*rhs_.get(M32),
			get(M11)*rhs_.get(M13)+get(M12)*rhs_.get(M23)+get(M13)*rhs_.get(M33),
			0,																													 
			get(M21)*rhs_.get(M11)+get(M22)*rhs_.get(M21)+get(M23)*rhs_.get(M31),
			get(M21)*rhs_.get(M12)+get(M22)*rhs_.get(M22)+get(M23)*rhs_.get(M32),
			get(M21)*rhs_.get(M13)+get(M22)*rhs_.get(M23)+get(M23)*rhs_.get(M33),
			0,																													 
			get(M31)*rhs_.get(M11)+get(M32)*rhs_.get(M21)+get(M33)*rhs_.get(M31),
			get(M31)*rhs_.get(M12)+get(M32)*rhs_.get(M22)+get(M33)*rhs_.get(M32),
			get(M31)*rhs_.get(M13)+get(M32)*rhs_.get(M23)+get(M33)*rhs_.get(M33),
			0,			 
			get(M41),get(M42),get(M43),1
			);			 
	}			

	float const * Matrix::array(void) const { return _matrix; }

//****************************************************************************//

	RotationX::RotationX(Radians angle_)
		:	Matrix(
		1,						0,						0,						0,  
		0,						cos(angle_),	sin(angle_),	0,  
		0,						-sin(angle_),	cos(angle_),	0)
	{}

	RotationY::RotationY(Radians angle_)
		:	Matrix(
		cos(angle_),	0,						-sin(angle_),	0,  
		0,						1,						0,						0,  
		sin(angle_),	0,						cos(angle_),	0)
	{}

	RotationZ::RotationZ(Radians angle_)
		:	Matrix(
		cos(angle_),	sin(angle_),	0,						0,  
		-sin(angle_),	cos(angle_),	0,						0,  
		0,						0,						1,						0)
	{}

//****************************************************************************//
}
