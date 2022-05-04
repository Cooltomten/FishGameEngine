#pragma once
#include "Math/Vector3.hpp"																						

namespace CommonUtilities
{

	template<typename T>
	class Sphere
	{
	public:
		// Default constructor: there is no sphere, the radius is zero and the position is
		// the zero vector.
		Sphere();

		// Copy constructor.
		Sphere(const Sphere<T>& aSphere);

		// Constructor that takes the center position and radius of the sphere.
		Sphere(const Vector3<T>& aCenter, T aRadius);

		// Init the sphere with a center and a radius, the same as the constructor above.
		void InitWithCenterAndRadius(const Vector3<T>& aCenter, T aRadius);

		// Returns whether a point is inside the sphere: it is inside when the point is on the
		// sphere surface or inside of the sphere.
		bool IsInside(const Vector3<T>& aPosition) const;

		const Vector3<T>& GetPosition() const;
		void SetPosition(const Vector3<T>& aPosition);

		T GetRadius() const;
		void SetRadius(T aRadius);
	private:
		T myRadius;
		Vector3<T> myPosition;
	};

	template<typename T>
	inline Sphere<T>::Sphere()
	{
		myPosition = { 0,0,0 };
		myRadius = 0;
	}

	template<typename T>
	inline Sphere<T>::Sphere(const Sphere<T>& aSphere)
	{
		myPosition = aSphere.myPosition;
		myRadius = aSphere.myRadius;
	}

	template<typename T>
	inline Sphere<T>::Sphere(const Vector3<T>& aCenter, T aRadius)
	{
		myPosition = aCenter;
		myRadius = aRadius;
	}

	template<typename T>
	inline void Sphere<T>::InitWithCenterAndRadius(const Vector3<T>& aCenter, T aRadius)
	{
		myPosition = aCenter;
		myRadius = aRadius;
	}

	template<typename T>
	inline bool Sphere<T>::IsInside(const Vector3<T>& aPosition) const
	{
		float dist = (myPosition - aPosition).LengthSqr();

		return dist <= (myRadius * myRadius);
	}
	template<typename T>
	inline const Vector3<T>& Sphere<T>::GetPosition() const
	{
		return myPosition;
	}
	template<typename T>
	inline void Sphere<T>::SetPosition(const Vector3<T>& aPosition)
	{
		myPosition = aPosition;
	}
	template<typename T>
	inline T Sphere<T>::GetRadius() const
	{
		return myRadius;
	}
	template<typename T>
	inline void Sphere<T>::SetRadius(T aRadius)
	{
		myRadius = aRadius;
	}
}