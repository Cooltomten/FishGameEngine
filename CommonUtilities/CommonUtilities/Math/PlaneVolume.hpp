#pragma once
#include "Plane.hpp"
#include <vector>
namespace CommonUtilities
{
	template<typename T>
	class PlaneVolume
	{
	public :
		// Default constructor: empty PlaneVolume.
		PlaneVolume() = default;
		// Constructor taking a list of Plane that makes up the PlaneVolume.
		PlaneVolume(const std::vector<Plane<T>>& aPlaneList);
		// Add a Plane to the PlaneVolume.
		void AddPlane(const Plane<T>& aPlane);
		// Returns whether a point is inside the PlaneVolume: it is inside when the point is on the
			// plane or on the side the normal is pointing away from for all the planes in the PlaneVolume.
		bool IsInside(const Vector3<T>& aPosition);
	private:
		std::vector<Plane<T>> myPlanes;
	};
	template<typename T>
	inline PlaneVolume<T>::PlaneVolume(const std::vector<Plane<T>>& aPlaneList)
	{
		myPlanes = aPlaneList;
	}
	template<typename T>
	inline void PlaneVolume<T>::AddPlane(const Plane<T>& aPlane)
	{
		myPlanes.push_back(aPlane);
	}
	template<typename T>
	inline bool PlaneVolume<T>::IsInside(const Vector3<T>& aPosition)
	{
		for (int i = 0; i < myPlanes.size(); i++)
		{
			if (!myPlanes[i].IsInside(aPosition)) 
			{
				return false;
			}
		}
		return true;
	}
}