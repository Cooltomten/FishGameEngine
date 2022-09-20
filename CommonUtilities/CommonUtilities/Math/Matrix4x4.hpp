#pragma once
#include "Vector4.hpp"
#include <Math.h>
#include <cassert>
#include <initializer_list>
#include <memory>
#include <vector>

namespace CommonUtilities
{
	template<typename T>
	class Matrix4x4
	{
	public:
		// Creates the identity matrix.
		Matrix4x4<T>();
		// Copy Constructor.
		Matrix4x4<T>(const Matrix4x4<T>& aMatrix);
		// () operator for accessing element (row, column) for read/write or read,respectively.
		T& operator()(const int aRow, const int aColumn);
		const T& operator()(const int aRow, const int aColumn) const;

		Matrix4x4<T>& operator=(const Matrix4x4<T>& aMatrix);
		Matrix4x4<T>& operator=(const std::initializer_list<T>& aList);
		Matrix4x4<T>& operator=(const T aArray[16]);
		Matrix4x4<T>& operator=(const T aArray[4][4]);

		
		T& operator[](const int aIndex);

		// Static functions for creating rotation matrices.
		static Matrix4x4<T> CreateRotationAroundX(T aAngleInRadians);
		static Matrix4x4<T> CreateRotationAroundY(T aAngleInRadians);
		static Matrix4x4<T> CreateRotationAroundZ(T aAngleInRadians);

		// Static function for creating a transpose of a matrix.
		static Matrix4x4<T> Transpose(const Matrix4x4<T>& aMatrixToTranspose);
		// Assumes aTransform is made up of nothing but rotations and translations.
		static Matrix4x4<T> GetFastInverse(const Matrix4x4<T>& aTransform);
		static Matrix4x4<T> GetInverse(const Matrix4x4<T>& aTransform);
		
	private:
		T myElements[16];
	};
	template<typename T>
	inline Matrix4x4<T>::Matrix4x4()
	{
		for (int row = 1; row < 5; row++)
		{
			for (int column = 1; column < 5; column++)
			{
				if (row == column)
				{
					(*this)(row, column) = 1;
				}
				else
				{
					(*this)(row, column) = 0;
				}
			}
		}
	}

	template<typename T>
	inline Matrix4x4<T>::Matrix4x4(const Matrix4x4<T>& aMatrix)
	{
		for (int i = 0; i < 16; i++)
		{
			this->myElements[i] = aMatrix.myElements[i];
		}
	}

	template<typename T>
	inline T& Matrix4x4<T>::operator()(const int aRow, const int aColumn)
	{
		assert(aRow < 5 && "aRow was out of bounds, has to be a value below or equals to 4");
		assert(aColumn < 5 && "aColumn was out of bounds, has to be a value below or equals to 4");

		assert(aRow > 0 && "aRow was out of bounds, has to be a value above or equals to 1");
		assert(aColumn > 0 && "aColumn was out of bounds, has to be a value above or equals to 1");

		return myElements[(aRow - 1) * 4 + (aColumn - 1)];
	}

	template<typename T>
	inline const T& Matrix4x4<T>::operator()(const int aRow, const int aColumn) const
	{
		assert(aRow < 5 && "aRow was out of bounds, has to be a value below or equals to 4");
		assert(aColumn < 5 && "aColumn was out of bounds, has to be a value below or equals to 4");

		assert(aRow > 0 && "aRow was out of bounds, has to be a value above or equals to 1");
		assert(aColumn > 0 && "aColumn was out of bounds, has to be a value above or equals to 1");

		return myElements[(aRow - 1) * 4 + (aColumn - 1)];
	}

	template<typename T>
	inline Matrix4x4<T>& Matrix4x4<T>::operator=(const Matrix4x4<T>& aMatrix)
	{
		memcpy(myElements, aMatrix.myElements, sizeof(aMatrix.myElements));
		return *this;
	}

	template<typename T>
	inline Matrix4x4<T>& Matrix4x4<T>::operator=(const std::initializer_list<T>& aList)
	{
		assert(aList.size() == 16 && "Initializer list was not size 16");

		for (int i = 0; i < 16; i++)
		{
			myElements[i] = aList[i];
		}
		return *this;
	}

	template<typename T>
	inline Matrix4x4<T>& Matrix4x4<T>::operator=(const T aArray[16])
	{
		memcpy(myElements, aArray, sizeof(T) * 16);
		return *this;
	}

	template<typename T>
	inline Matrix4x4<T>& Matrix4x4<T>::operator=(const T aArray[4][4])
	{
		memcpy(&myElements[0],  &aArray[0], sizeof(T) * 4);
		memcpy(&myElements[4],  &aArray[1], sizeof(T) * 4);
		memcpy(&myElements[8],  &aArray[2], sizeof(T) * 4);
		memcpy(&myElements[12], &aArray[3], sizeof(T) * 4);
		return *this;
	}
	
	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundX(T aAngleInRadians)
	{
		Matrix4x4 <T> xRotateMatrix;
		xRotateMatrix(2, 2) = cos(aAngleInRadians);
		xRotateMatrix(2, 3) = sin(aAngleInRadians);

		xRotateMatrix(3, 2) = (-sin(aAngleInRadians));
		xRotateMatrix(3, 3) = cos(aAngleInRadians);
		return xRotateMatrix;
	}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundY(T aAngleInRadians)
	{
		Matrix4x4 <T> yRotateMatrix;
		yRotateMatrix(1, 1) = cos(aAngleInRadians);
		yRotateMatrix(1, 3) = (-sin(aAngleInRadians));

		yRotateMatrix(3, 1) = sin(aAngleInRadians);
		yRotateMatrix(3, 3) = cos(aAngleInRadians);
		return yRotateMatrix;
	}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundZ(T aAngleInRadians)
	{
		Matrix4x4 <T> zRotateMatrix;
		zRotateMatrix(1, 1) = cos(aAngleInRadians);
		zRotateMatrix(1, 2) = sin(aAngleInRadians);

		zRotateMatrix(2, 1) = (-sin(aAngleInRadians));
		zRotateMatrix(2, 2) = cos(aAngleInRadians);
		return zRotateMatrix;
	}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::Transpose(const Matrix4x4<T>& aMatrixToTranspose)
	{
		Matrix4x4<T> transposedMatrix;

		for (int row = 1; row < 5; row++)
		{
			for (int column = 1; column < 5; column++)
			{
				transposedMatrix(row, column) = aMatrixToTranspose(column, row);
			}
		}
		return transposedMatrix;
	}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::GetFastInverse(const Matrix4x4<T>& aTransform)
	{
		Matrix4x4 matrixInverse(aTransform);
		CommonUtilities::Vector4f pos(aTransform(4, 1), aTransform(4, 2), aTransform(4, 3), aTransform(4, 4));
		matrixInverse(4, 1) = 0;
		matrixInverse(4, 2) = 0;
		matrixInverse(4, 3) = 0;

		matrixInverse =  Transpose(matrixInverse);

		pos = CommonUtilities::Vector4f(-pos.x, -pos.y, -pos.z, pos.w) * matrixInverse;
		matrixInverse(4, 1) = pos.x;
		matrixInverse(4, 2) = pos.y;
		matrixInverse(4, 3) = pos.z;

		return matrixInverse;
	}
	
	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::GetInverse(const Matrix4x4<T>& aTransform)
	{
		T m[4][4];
		Matrix4x4<T> matrix = Transpose(aTransform);
		memcpy(&m, &matrix, sizeof(T) * 16);

		T Coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
		T Coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
		T Coef03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

		T Coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
		T Coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
		T Coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

		T Coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
		T Coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
		T Coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

		T Coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
		T Coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
		T Coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

		T Coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
		T Coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
		T Coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

		T Coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
		T Coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
		T Coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

		Vector4<T> Fac0(Coef00, Coef00, Coef02, Coef03);
		Vector4<T> Fac1(Coef04, Coef04, Coef06, Coef07);
		Vector4<T> Fac2(Coef08, Coef08, Coef10, Coef11);
		Vector4<T> Fac3(Coef12, Coef12, Coef14, Coef15);
		Vector4<T> Fac4(Coef16, Coef16, Coef18, Coef19);
		Vector4<T> Fac5(Coef20, Coef20, Coef22, Coef23);

		Vector4<T> Vec0(m[1][0], m[0][0], m[0][0], m[0][0]);
		Vector4<T> Vec1(m[1][1], m[0][1], m[0][1], m[0][1]);
		Vector4<T> Vec2(m[1][2], m[0][2], m[0][2], m[0][2]);
		Vector4<T> Vec3(m[1][3], m[0][3], m[0][3], m[0][3]);

		Vector4<T> Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
		Vector4<T> Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
		Vector4<T> Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
		Vector4<T> Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);

		Vector4<T> SignA(+1, -1, +1, -1);
		Vector4<T> SignB(-1, +1, -1, +1);
		std::vector<Vector4<T>> inverse({ Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB });

		Vector4<T> Row0(inverse[0].x, inverse[1].x, inverse[2].x, inverse[3].x);

		T x = m[0][0] * Row0.x;
		T y = m[0][1] * Row0.y;
		T z = m[0][2] * Row0.z;
		T w = m[0][3] * Row0.w;
		Vector4<T> Dot0(x, y, z, w);
		T Dot1 = (Dot0.x + Dot0.y) + (Dot0.z + Dot0.w);

		T OneOverDeterminant = static_cast<T>(1) / Dot1;

		memcpy(&matrix, inverse.data(), sizeof(T) * 16);

		matrix = matrix * OneOverDeterminant;
		matrix = Transpose(matrix);

		return matrix;
	}


	template <typename T>
	void operator+=(Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1)
	{
		for (int i = 1; i < 5; i++)
		{
			for (int j = 1; j < 5; j++)
			{
				aMatrix0(i, j) += aMatrix1(i, j);
			}
		}
	}

	template <typename T>
	Matrix4x4<T> operator+(const Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1)
	{
		Matrix4x4<T> resultMatrix(aMatrix0);

		resultMatrix += aMatrix1;

		return resultMatrix;
	}

	template <typename T>
	void operator-=(Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1)
	{
		for (int i = 1; i < 5; i++)
		{
			for (int j = 1; j < 5; j++)
			{
				aMatrix0(i, j) -= aMatrix1(i, j);
			}
		}
	}

	template <typename T>
	Matrix4x4<T> operator-(const Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1)
	{
		Matrix4x4<T> resultMatrix(aMatrix0);

		resultMatrix -= aMatrix1;

		return resultMatrix;
	}

	template <typename T>
	void operator*=(Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1)
	{
		Matrix4x4<T> result;
		result(1, 1) = aMatrix0(1, 1) * aMatrix1(1, 1) + aMatrix0(1, 2) * aMatrix1(2, 1) + aMatrix0(1, 3) * aMatrix1(3, 1) + aMatrix0(1, 4) * aMatrix1(4, 1);
		result(1, 2) = aMatrix0(1, 1) * aMatrix1(1, 2) + aMatrix0(1, 2) * aMatrix1(2, 2) + aMatrix0(1, 3) * aMatrix1(3, 2) + aMatrix0(1, 4) * aMatrix1(4, 2);
		result(1, 3) = aMatrix0(1, 1) * aMatrix1(1, 3) + aMatrix0(1, 2) * aMatrix1(2, 3) + aMatrix0(1, 3) * aMatrix1(3, 3) + aMatrix0(1, 4) * aMatrix1(4, 3);
		result(1, 4) = aMatrix0(1, 1) * aMatrix1(1, 4) + aMatrix0(1, 2) * aMatrix1(2, 4) + aMatrix0(1, 3) * aMatrix1(3, 4) + aMatrix0(1, 4) * aMatrix1(4, 4);

		result(2, 1) = aMatrix0(2, 1) * aMatrix1(1, 1) + aMatrix0(2, 2) * aMatrix1(2, 1) + aMatrix0(2, 3) * aMatrix1(3, 1) + aMatrix0(2, 4) * aMatrix1(4, 1);
		result(2, 2) = aMatrix0(2, 1) * aMatrix1(1, 2) + aMatrix0(2, 2) * aMatrix1(2, 2) + aMatrix0(2, 3) * aMatrix1(3, 2) + aMatrix0(2, 4) * aMatrix1(4, 2);
		result(2, 3) = aMatrix0(2, 1) * aMatrix1(1, 3) + aMatrix0(2, 2) * aMatrix1(2, 3) + aMatrix0(2, 3) * aMatrix1(3, 3) + aMatrix0(2, 4) * aMatrix1(4, 3);
		result(2, 4) = aMatrix0(2, 1) * aMatrix1(1, 4) + aMatrix0(2, 2) * aMatrix1(2, 4) + aMatrix0(2, 3) * aMatrix1(3, 4) + aMatrix0(2, 4) * aMatrix1(4, 4);

		result(3, 1) = aMatrix0(3, 1) * aMatrix1(1, 1) + aMatrix0(3, 2) * aMatrix1(2, 1) + aMatrix0(3, 3) * aMatrix1(3, 1) + aMatrix0(3, 4) * aMatrix1(4, 1);
		result(3, 2) = aMatrix0(3, 1) * aMatrix1(1, 2) + aMatrix0(3, 2) * aMatrix1(2, 2) + aMatrix0(3, 3) * aMatrix1(3, 2) + aMatrix0(3, 4) * aMatrix1(4, 2);
		result(3, 3) = aMatrix0(3, 1) * aMatrix1(1, 3) + aMatrix0(3, 2) * aMatrix1(2, 3) + aMatrix0(3, 3) * aMatrix1(3, 3) + aMatrix0(3, 4) * aMatrix1(4, 3);
		result(3, 4) = aMatrix0(3, 1) * aMatrix1(1, 4) + aMatrix0(3, 2) * aMatrix1(2, 4) + aMatrix0(3, 3) * aMatrix1(3, 4) + aMatrix0(3, 4) * aMatrix1(4, 4);

		result(4, 1) = aMatrix0(4, 1) * aMatrix1(1, 1) + aMatrix0(4, 2) * aMatrix1(2, 1) + aMatrix0(4, 3) * aMatrix1(3, 1) + aMatrix0(4, 4) * aMatrix1(4, 1);
		result(4, 2) = aMatrix0(4, 1) * aMatrix1(1, 2) + aMatrix0(4, 2) * aMatrix1(2, 2) + aMatrix0(4, 3) * aMatrix1(3, 2) + aMatrix0(4, 4) * aMatrix1(4, 2);
		result(4, 3) = aMatrix0(4, 1) * aMatrix1(1, 3) + aMatrix0(4, 2) * aMatrix1(2, 3) + aMatrix0(4, 3) * aMatrix1(3, 3) + aMatrix0(4, 4) * aMatrix1(4, 3);
		result(4, 4) = aMatrix0(4, 1) * aMatrix1(1, 4) + aMatrix0(4, 2) * aMatrix1(2, 4) + aMatrix0(4, 3) * aMatrix1(3, 4) + aMatrix0(4, 4) * aMatrix1(4, 4);

		aMatrix0 = result;
	}

	template <typename T>
	Matrix4x4<T> operator*(const Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1)
	{
		Matrix4x4<T> result(aMatrix0);
		result *= aMatrix1;
		return result;
	}

	template <typename T>
	void operator*=(Vector4<T>& aVector, const Matrix4x4<T>& aMatrix)
	{
		Vector4<T> result;

		result.x = aVector.x * aMatrix(1, 1) + aVector.y * aMatrix(2, 1) + aVector.z * aMatrix(3, 1) + aVector.w * aMatrix(4, 1);
		result.y = aVector.x * aMatrix(1, 2) + aVector.y * aMatrix(2, 2) + aVector.z * aMatrix(3, 2) + aVector.w * aMatrix(4, 2);
		result.z = aVector.x * aMatrix(1, 3) + aVector.y * aMatrix(2, 3) + aVector.z * aMatrix(3, 3) + aVector.w * aMatrix(4, 3);
		result.w = aVector.x * aMatrix(1, 4) + aVector.y * aMatrix(2, 4) + aVector.z * aMatrix(3, 4) + aVector.w * aMatrix(4, 4);
		aVector = result;
	}

	template <typename T>
	Vector4<T> operator*(const Vector4<T>& aVector, const Matrix4x4<T>& aMatrix)
	{
		Vector4<T> result(aVector);
		result *= aMatrix;
		return result;
	}

	template <typename T>
	T& Matrix4x4<T>::operator[](const int aIndex)
	{
		return myElements[aIndex];
	}

	template <typename T>
	void operator*=(Matrix4x4<T>& aMatrix, const T& aScaler)
	{
		for (int i = 0; i < 16; i++)
		{
			aMatrix[i] *= aScaler;
		}

	}

	template <typename T>
	Matrix4x4<T> operator*(const Matrix4x4<T>& aMatrix, const T& aScaler)
	{
		Matrix4x4<T> result(aMatrix);
		result *= aScaler;
		return result;
	}

	template <typename T>
	Matrix4x4<T> operator*(const T& aScaler, const Matrix4x4<T>& aMatrix)
	{
		Matrix4x4<T> result(aMatrix);
		result *= aScaler;
		return result;
	}

	template<typename T>
	bool operator==(const Matrix4x4<T>& aLhs, const Matrix4x4<T>& aRhs)
	{
		for (int i = 1; i < 5; i++)
		{
			for (int j = 1; j < 5; j++)
			{
				if (aLhs(i, j) != aRhs(i, j))
				{
					return false;
				}

			}
		}
		return true;
	}




}
