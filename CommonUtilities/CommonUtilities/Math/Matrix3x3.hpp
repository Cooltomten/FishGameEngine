#pragma once
#include "Matrix4x4.hpp"
#include "Vector3.hpp"
#include <Math.h>
#include <cassert>
#include <initializer_list>
#include <memory>

namespace CommonUtilities
{
	template<typename T>
	class Matrix3x3
	{
	public:
		// Creates the identity matrix.
		Matrix3x3<T>();

		// Copy Constructor.
		Matrix3x3<T>(const Matrix3x3<T>& aMatrix);

		// Copies the top left 3x3 part of the Matrix4x4.
		Matrix3x3<T>(const Matrix4x4<T>& aMatrix);

		// () operator for accessing element (row, column) for read/write or read, respectively.
		T& operator()(const int aRow, const int aColumn);

		const T& operator()(const int aRow, const int aColumn) const;

		Matrix3x3<T>& operator=(const Matrix3x3<T>& aMatrix);
		Matrix3x3<T>& operator=(const std::initializer_list<T>& aList);

		// Static functions for creating rotation matrices.
		static Matrix3x3<T> CreateRotationAroundX(T aAngleInRadians);
		static Matrix3x3<T> CreateRotationAroundY(T aAngleInRadians);
		static Matrix3x3<T> CreateRotationAroundZ(T aAngleInRadians);

		// Static function for creating a transpose of a matrix.
		static Matrix3x3<T> Transpose(const Matrix3x3<T>& aMatrixToTranspose);

		static Matrix3x3<T> GetFastInverse(const Matrix3x3<T>& aTransform);
	private:
		T myElements[9];
	};
	template<typename T>
	inline Matrix3x3<T>::Matrix3x3()
	{
		for (int row = 1; row < 4; row++)
		{
			for (int column = 1; column < 4; column++)
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
	inline Matrix3x3<T>::Matrix3x3(const Matrix3x3<T>& aMatrix)
	{
		for (int i = 0; i < 9; i++)
		{
			this->myElements[i] = aMatrix.myElements[i];
		}
	}

	template<typename T>
	inline Matrix3x3<T>::Matrix3x3(const Matrix4x4<T>& aMatrix)
	{
		for (int row = 1; row < 4; row++)
		{
			for (int column = 1; column < 4; column++)
			{
				(*this)(row, column) = aMatrix(row, column);
			}
		}
	}

	template<typename T>
	inline T& Matrix3x3<T>::operator()(const int aRow, const int aColumn)
	{
		assert(aRow < 4 && "aRow was out of bounds, has to be a value below or equals to 3");
		assert(aColumn < 4 && "aColumn was out of bounds, has to be a value below or equals to 3");

		assert(aRow > 0 && "aRow was out of bounds, has to be a value above or equals to 1");
		assert(aColumn > 0 && "aColumn was out of bounds, has to be a value above or equals to 1");

		return myElements[(aRow - 1) * 3 + (aColumn - 1)];
	}
	template<typename T>
	inline const T& Matrix3x3<T>::operator()(const int aRow, const int aColumn) const
	{
		assert(aRow < 4 && "aRow was out of bounds, has to be a value below or equals to 3");
		assert(aColumn < 4 && "aColumn was out of bounds, has to be a value below or equals to 3");

		assert(aRow > 0 && "aRow was out of bounds, has to be a value above or equals to 1");
		assert(aColumn > 0 && "aColumn was out of bounds, has to be a value above or equals to 1");

		return myElements[(aRow - 1) * 3 + (aColumn - 1)];
	}

	template<typename T>
	inline Matrix3x3<T>& Matrix3x3<T>::operator=(const Matrix3x3<T>& aMatrix)
	{
		memcpy(myElements, aMatrix.myElements, sizeof(aMatrix.myElements));
		return *this;
	}

	template<typename T>
	inline Matrix3x3<T>& Matrix3x3<T>::operator=(const std::initializer_list<T>& aList)
	{
		assert(aList.size() == 9 && "Initializer list was not size 9");

		for (int i = 0; i < 9; i++)
		{
			myElements[i] = aList[i];
		}
		return *this;
	}

	template<typename T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundX(T aAngleInRadians)
	{
		Matrix3x3 <T> xRotateMatrix;
		xRotateMatrix(2, 2) = cos(aAngleInRadians);
		xRotateMatrix(2, 3) = sin(aAngleInRadians);

		xRotateMatrix(3, 2) = (-sin(aAngleInRadians));
		xRotateMatrix(3, 3) = cos(aAngleInRadians);
		return xRotateMatrix;
	}

	template<typename T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundY(T aAngleInRadians)
	{
		Matrix3x3 <T> yRotateMatrix;
		yRotateMatrix(1, 1) = cos(aAngleInRadians);
		yRotateMatrix(1, 3) = (-sin(aAngleInRadians));

		yRotateMatrix(3, 1) = sin(aAngleInRadians);
		yRotateMatrix(3, 3) = cos(aAngleInRadians);
		return yRotateMatrix;
	}

	template<typename T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundZ(T aAngleInRadians)
	{
		Matrix3x3 <T> zRotateMatrix;
		zRotateMatrix(1, 1) = cos(aAngleInRadians);
		zRotateMatrix(1, 2) = sin(aAngleInRadians);

		zRotateMatrix(2, 1) = (-sin(aAngleInRadians));
		zRotateMatrix(2, 2) = cos(aAngleInRadians);
		return zRotateMatrix;
	}

	template<typename T>
	inline Matrix3x3<T> Matrix3x3<T>::Transpose(const Matrix3x3<T>& aMatrixToTranspose)
	{
		Matrix3x3<T> transposedMatrix;

		for (int row = 1; row < 4; row++)
		{
			for (int column = 1; column < 4; column++)
			{
				transposedMatrix(row, column) = aMatrixToTranspose(column, row);
			}
		}
		return transposedMatrix;
	}

	template<typename T>
	inline Matrix3x3<T> Matrix3x3<T>::GetFastInverse(const Matrix3x3<T>& aTransform)
	{
		//Matrix3x3<T> matrixInverse(aTransform);
		//T temp = matrixInverse(1, 2);
		//matrixInverse(1,2) = matrixInverse(2,1) ;
		//matrixInverse(2, 1) = temp;

		//matrixInverse(3, 1) *= -1;
		//matrixInverse(3, 2) *= -1;
		assert(false && "NOT IMPLEMENTED");

		return Matrix3x3<T>();
	}

	template <typename T>
	void operator+=(Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		for (int i = 1; i < 4; i++)
		{
			for (int j = 1; j < 4; j++)
			{
				aMatrix0(i, j) += aMatrix1(i, j);
			}
		}
	}

	template <typename T>
	Matrix3x3<T> operator+(const Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		Matrix3x3<T> resultMatrix(aMatrix0);

		resultMatrix += aMatrix1;

		return resultMatrix;
	}

	template <typename T>
	void operator-=(Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		for (int i = 1; i < 4; i++)
		{
			for (int j = 1; j < 4; j++)
			{
				aMatrix0(i, j) -= aMatrix1(i, j);
			}
		}
	}

	template <typename T>
	Matrix3x3<T> operator-(const Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		Matrix3x3<T> resultMatrix(aMatrix0);

		resultMatrix -= aMatrix1;

		return resultMatrix;
	}

	template <typename T>
	void operator*=(Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		Matrix3x3<T> result;
		result(1, 1) = aMatrix0(1,1) * aMatrix1(1, 1) + aMatrix0(1, 2) * aMatrix1(2, 1) + aMatrix0(1, 3) * aMatrix1(3, 1);
		result(1, 2) = aMatrix0(1,1) * aMatrix1(1, 2) + aMatrix0(1, 2) * aMatrix1(2, 2) + aMatrix0(1, 3) * aMatrix1(3, 2);
		result(1, 3) = aMatrix0(1,1) * aMatrix1(1, 3) + aMatrix0(1, 2) * aMatrix1(2, 3) + aMatrix0(1, 3) * aMatrix1(3, 3);

		result(2, 1) = aMatrix0(2, 1) * aMatrix1(1, 1) + aMatrix0(2,2) * aMatrix1(2, 1) + aMatrix0(2,3) * aMatrix1(3, 1);
		result(2, 2) = aMatrix0(2, 1) * aMatrix1(1, 2) + aMatrix0(2,2) * aMatrix1(2, 2) + aMatrix0(2,3) * aMatrix1(3, 2);
		result(2, 3) = aMatrix0(2, 1) * aMatrix1(1, 3) + aMatrix0(2,2) * aMatrix1(2, 3) + aMatrix0(2,3) * aMatrix1(3, 3);

		result(3, 1) = aMatrix0(3,1) * aMatrix1(1, 1) + aMatrix0(3,2) * aMatrix1(2, 1) + aMatrix0(3,3) * aMatrix1(3, 1);
		result(3, 2) = aMatrix0(3,1) * aMatrix1(1, 2) + aMatrix0(3,2) * aMatrix1(2, 2) + aMatrix0(3,3) * aMatrix1(3, 2);
		result(3, 3) = aMatrix0(3,1) * aMatrix1(1, 3) + aMatrix0(3,2) * aMatrix1(2, 3) + aMatrix0(3,3) * aMatrix1(3, 3);

		aMatrix0 = result;
	}

	template <typename T>
	Matrix3x3<T> operator*(const Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		Matrix3x3<T> result(aMatrix0);
		result *= aMatrix1;
		return result;
	}

	template <typename T>
	void operator*=(Vector3<T>& aVector, const Matrix3x3<T>& aMatrix)
	{
		Vector3<T> result;

		result.x = aVector.x * aMatrix(1, 1) + aVector.y * aMatrix(2, 1) + aVector.z * aMatrix(3, 1);
		result.y = aVector.x * aMatrix(1, 2) + aVector.y * aMatrix(2, 2) + aVector.z * aMatrix(3, 2);
		result.z = aVector.x * aMatrix(1, 3) + aVector.y * aMatrix(2, 3) + aVector.z * aMatrix(3, 3);
		aVector = result;
	}

	template <typename T>
	Vector3<T> operator*(const Vector3<T>& aVector, const Matrix3x3<T>& aMatrix)
	{
		Vector3<T> result(aVector);
		result *= aMatrix;
		return result;
	}

	template <typename T>
	void operator*=(Matrix3x3<T>& aMatrix, const T& aScaler)
	{
		for (int i = 0; i < 9; i++)
		{
			aMatrix.myElements[i] *= aScaler;
		}

	}

	template <typename T>
	Matrix3x3<T> operator*(const Matrix3x3<T>& aMatrix, const T& aScaler)
	{
		Matrix3x3<T> result(aMatrix);
		result *= aScaler;
		return result;
	}

	template <typename T>
	Matrix3x3<T> operator*(const T& aScaler, const Matrix3x3<T>& aMatrix)
	{
		Matrix3x3<T> result(aMatrix);
		result *= aScaler;
		return result;
	}

	template<typename T>
	bool operator==(const Matrix3x3<T>& aLhs, const Matrix3x3<T>& aRhs)
	{
		for (int i = 1; i < 4; i++)
		{
			for (int j = 1; j < 4; j++)
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
