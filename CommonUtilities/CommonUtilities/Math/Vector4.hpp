#pragma once
#include <cassert>

namespace CommonUtilities
{
	template<typename T>
	class Vector4
	{
	public:
		T x;
		T y;
		T z;
		T w;

		//Creates a null-vector
		Vector4<T>()
		{
			x = 0;
			y = 0;
			z = 0;
			w = 0;
		}

		//Creates a vector (aX, 0, 0, 0)
		Vector4<T>(const T& aX)
		{
			x = aX;
			y = 0;
			z = 0;
			w = 0;
		}

		//Creates a vector (aX, aY, 0, 0)
		Vector4<T>(const T& aX, const T& aY)
		{
			x = aX;
			y = aY;
			z = 0;
			w = 0;
		}
		//Creates a vector (aX, aY, aZ, 0)
		Vector4<T>(const T& aX, const T& aY, const T& aZ)
		{
			x = aX;
			y = aY;
			z = aZ;
			w = 0;
		}
		//Creates a vector (aX, aY, aZ, 0)
		Vector4<T>(const T& aX, const T& aY, const T& aZ, const T& aW)
		{
			x = aX;
			y = aY;
			z = aZ;
			w = aW;
		}


		//Copy constructor (compiler generated)
		Vector4<T>(const Vector4<T>& aVector) = default;

		//Assignment operator (compiler generated)
		Vector4<T>& operator=(const Vector4<T>& aVector) = default;

		//Destructor (compiler generated)
		~Vector4<T>() = default;

		//Returns the squared length of the vector
		T LengthSqr() const;

		//Returns the length of the vector
		T Length() const;

		//Returns a normalized copy of this
		Vector4<T> GetNormalized() const;

		//Normalizes the vector
		void Normalize();

		//Returns the dot product of this and aVector
		T Dot(const Vector4<T>& aVector) const;
	};

	template<typename T>
	inline T Vector4<T>::LengthSqr() const
	{
		return (x * x + y * y + z * z + w*w);
	}

	template<typename T>
	inline T Vector4<T>::Length() const
	{
		return sqrt(LengthSqr());
	}

	template<typename T>
	inline Vector4<T> Vector4<T>::GetNormalized() const
	{
		Vector4<T>  normalizedVector(*this);
		normalizedVector.Normalize();
		return normalizedVector;
	}

	template<typename T>
	inline void Vector4<T>::Normalize()
	{
		float length = Length();
		assert(length != 0 && "Division by zero when normalizing Vector4");
		x /= length;
		y /= length;
		z /= length;
		w /= length;
	}

	template<typename T>
	inline T Vector4<T>::Dot(const Vector4<T>& aVector) const
	{
		return (x * aVector.x + y * aVector.y + z * aVector.z + w * aVector.w);
	}


	//Returns the vector sum of aVector0 and aVector1
	template <typename  T> Vector4<T> operator+(const Vector4<T>& aVector0, const Vector4<T>&
		aVector1)
	{
		return Vector4<T>(aVector0.x + aVector1.x, aVector0.y + aVector1.y, aVector0.z + aVector1.z, aVector0.w + aVector1.w);
	}

	//Returns the vector difference of aVector0 and aVector1
	template <class T> Vector4<T> operator-(const Vector4<T>& aVector0, const Vector4<T>&
		aVector1)
	{
		return Vector4<T>(aVector0.x - aVector1.x, aVector0.y - aVector1.y, aVector0.z - aVector1.z, aVector0.w - aVector1.w );
	}

	//Returns the vector aVector multiplied by the scalar aScalar
	template <class T> Vector4<T> operator*(const Vector4<T>& aVector, const T& aScalar)
	{
		return Vector4<T>(aVector.x * aScalar, aVector.y * aScalar, aVector.z * aScalar, aVector.w * aScalar);
	}

	//Returns the vector aVector multiplied by the scalar aScalar
	template <class T> Vector4<T> operator*(const T& aScalar, const Vector4<T>& aVector)
	{
		return Vector4<T>(aVector.x * aScalar, aVector.y * aScalar, aVector.z * aScalar,aVector.w * aScalar);
	}

	//Component wise multiplication
	template <class T> Vector4<T> operator*(const Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		return Vector4<T>(aVector0.x * aVector1.x, aVector0.y * aVector1.y, aVector0.z * aVector1.z, aVector0.w * aVector1.w);
	}

	//Component wise division
	template <class T> Vector4<T> operator/(const Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		return Vector4<T>(aVector0.x / aVector1.x, aVector0.y / aVector1.y, aVector0.z / aVector1.z, aVector0.w / aVector1.w);

	}

	//Component wise multiplication
	template <class T>  void operator*=(Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		aVector0.x *= aVector1.x;
		aVector0.y *= aVector1.y;
		aVector0.z *= aVector1.z;
		aVector0.w *= aVector1.w;
	}

	//Component wise division
	template <class T> void operator/=(Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		aVector0.x /= aVector1.x;
		aVector0.y /= aVector1.y;
		aVector0.z /= aVector1.z;
		aVector0.w /= aVector1.w;

	}


	//Returns the vector aVector divided by the scalar aScalar (equivalent to aVector multiplied by 1 / aScalar)
	template <class T> Vector4<T> operator/(const Vector4<T>& aVector, const T& aScalar)
	{
		return Vector4<T>(aVector.x / aScalar, aVector.y / aScalar, aVector.z / aScalar , aVector.w / aScalar);
	}

	//Equivalent to setting aVector0 to (aVector0 + aVector1)
	template <class T> void operator+=(Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		aVector0.x += aVector1.x;
		aVector0.y += aVector1.y;
		aVector0.z += aVector1.z;
		aVector0.w += aVector1.w;

	}

	//Equivalent to setting aVector0 to (aVector0 - aVector1)
	template <class T> void operator-=(Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		aVector0.x -= aVector1.x;
		aVector0.y -= aVector1.y;
		aVector0.z -= aVector1.z;
		aVector0.w -= aVector1.w;
	}

	//Equivalent to setting aVector to (aVector * aScalar)
	template <class T> void operator*=(Vector4<T>& aVector, const T& aScalar)
	{
		aVector.x *= aScalar;
		aVector.y *= aScalar;
		aVector.z *= aScalar;
		aVector.w *= aScalar;
	}

	//Equivalent to setting aVector to (aVector / aScalar)
	template <class T> void operator/=(Vector4<T>& aVector, const T& aScalar)
	{
		aVector.x /= aScalar;
		aVector.y /= aScalar;
		aVector.z /= aScalar;
		aVector.w /= aScalar;
	}
	typedef Vector4<float> Vector4f;
}



