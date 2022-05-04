#pragma once
#include "Math/Plane.hpp"
#include "Sphere.hpp"
#include "AABB3D.hpp"
#include "Ray.hpp"

namespace CommonUtilities
{
	// If the ray is parallel to the plane, aOutIntersectionPoint remains unchanged. If
	// the ray is in the plane, true is returned, if not, false is returned. If the ray
	// isn't parallel to the plane, the intersection point is stored in
	// aOutIntersectionPoint and true returned.k
	template<typename T>
	bool IntersectionPlaneRay(const Plane<T>& aPlane, const Ray<T>& aRay, Vector3<T>&
		aOutIntersectionPoint);
	// If the ray intersects the AABB, true is returned, if not, false is returned.
	// A ray in one of the AABB's sides is counted as intersecting it.
	template<typename T>
	bool IntersectionAABBRay(const AABB3D<T>& aAABB, const Ray<T>& aRay);
	// If the ray intersects the sphere, true is returned, if not, false is returned.
	// A ray intersecting the surface of the sphere is considered as intersecting it.
	template<typename T>
	bool IntersectionSphereRay(const Sphere<T>& aSphere, const Ray<T>& aRay);

	template<typename T>
	bool IntersectionSphereSphere(const Sphere<T>& aFirstSphere, const Sphere<T>& aSecondSphere);
	template<typename T>
	bool IntersectionAABBAABB(const AABB3D<T>& aFirstAABB, const AABB3D<T>& aSecondAABB);
	template<typename T>
	bool IntersectionAABBSphere(const Sphere<T>& aSphere, const AABB3D<T>& aAABB);



	template<typename T>
	bool IntersectionPlaneRay(const Plane<T>& aPlane, const Ray<T>& aRay, Vector3<T>& aOutIntersectionPoint)
	{
		T  dot = aPlane.GetNormal().Dot(aRay.GetDirection());
		if (dot > -0.0001f && dot < 0.0001f)
		{
			return false;
		}


		if (aPlane.IsInside(aRay.GetOrigin()))
		{
			if (dot > 0)
			{
				T t = (aPlane.GetPoint().Dot(aPlane.GetNormal()) - (aRay.GetOrigin().Dot(aPlane.GetNormal()))) / (aRay.GetDirection().Dot(aPlane.GetNormal()));
				aOutIntersectionPoint = aRay.GetOrigin() + aRay.GetDirection() * t;
				return true;
			}
			else
			{
				return false;
			}

		}
		else
		{
			if (dot < 0)
			{
				T t = (aPlane.GetPoint().Dot(aPlane.GetNormal()) - (aRay.GetOrigin().Dot(aPlane.GetNormal()))) / (aRay.GetDirection().Dot(aPlane.GetNormal()));
				aOutIntersectionPoint = aRay.GetOrigin() + aRay.GetDirection() * t;
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	template<typename T>
	bool IntersectionAABBRay(const AABB3D<T>& aAABB, const Ray<T>& aRay)
	{
		Plane<T> plane;

		Vector3<T> mn = aAABB.GetMinCorner();
		Vector3<T> mx = aAABB.GetMaxCorner();
			   
		Vector3<T> out;

		//front
		plane.InitWith3Points(mn, { mx.x, mn.y, mn.z }, { mx.x, mx.y, mn.z });
		if (IntersectionPlaneRay(plane, aRay, out))
		{
			if (aAABB.IsInside(out))
			{
				return true;
			}
		}

		//Right
		plane.InitWith3Points({ mx.x, mn.y, mn.z }, { mx.x, mn.y, mx.z }, mx);
		if (IntersectionPlaneRay(plane, aRay, out))
		{
			if (aAABB.IsInside(out))
			{
				return true;
			}
		}

		//top
		plane.InitWith3Points(mn, { mn.x, mn.y, mx.z }, { mx.x, mn.y, mx.z });
		if (IntersectionPlaneRay(plane, aRay, out))
		{
			if (aAABB.IsInside(out))
			{
				return true;
			}
		}

		//left
		plane.InitWith3Points(mn, { mn.x, mn.y, mx.z }, { mn.x, mx.y, mx.z });
		if (IntersectionPlaneRay(plane, aRay, out))
		{
			if (aAABB.IsInside(out))
			{
				return true;
			}
		}

		//bottom
		plane.InitWith3Points(mx, { mn.x, mx.y, mx.z }, { mx.x, mx.y, mn.z });
		if (IntersectionPlaneRay(plane, aRay, out))
		{
			if (aAABB.IsInside(out))
			{
				return true;
			}
		}

		//back
		plane.InitWith3Points(mx, { mn.x, mx.y, mx.z }, { mn.x, mn.y,mx.z });
		if (IntersectionPlaneRay(plane, aRay, out))
		{
			if (aAABB.IsInside(out))
			{
				return true;
			}
		}

		return false;
	}

	template<typename T>
	bool IntersectionSphereRay(const Sphere<T>& aSphere, const Ray<T>& aRay)
	{
		Vector3<T> diff = (aSphere.GetPosition() - aRay.GetOrigin());
		T t = diff.Dot(aRay.GetDirection());

		Vector3<T> closestPoint = aRay.GetOrigin() + aRay.GetDirection() * t;

		if (aSphere.IsInside(closestPoint)) 
		{
			return true;
		}

		return false;
	}

	template<typename T>
	bool IntersectionSphereSphere(const Sphere<T>& aFirstSphere, const Sphere<T>& aSecondSphere)
	{
		T dist = (aFirstSphere.GetPosition() - aSecondSphere.GetPosition()).LengthSqr();
		return (dist < ((aFirstSphere.GetRadius() + aSecondSphere.GetRadius())* (aFirstSphere.GetRadius() + aSecondSphere.GetRadius())));
	}

	template<typename T>
	bool IntersectionAABBAABB(const AABB3D<T>& aFirstAABB, const AABB3D<T>& aSecondAABB)
	{
		const CommonUtilities::Vector3<T>& aFirstMin = aFirstAABB.GetMinCorner();
		const CommonUtilities::Vector3<T>& aFirstMax = aFirstAABB.GetMaxCorner();

		const CommonUtilities::Vector3<T>& aSecondMin = aSecondAABB.GetMinCorner();
		const CommonUtilities::Vector3<T>& aSecondMax = aSecondAABB.GetMaxCorner();

		if (aFirstMin.x > aSecondMax.x) return false;
		if (aFirstMax.x < aSecondMin.x) return false;

		if (aFirstMin.y > aSecondMax.y) return false;
		if (aFirstMax.y < aSecondMin.y) return false;

		if (aFirstMin.z > aSecondMax.z) return false;
		if (aFirstMax.z < aSecondMin.z) return false;

		return true;
	}

	template<typename T>
	bool IntersectionAABBSphere(const Sphere<T>& aSphere, const AABB3D<T>& aAABB)
	{
		T sqDist = 0;

		// for each axis count any excess distance outside box extents
		T v = aSphere.GetPosition().x;
		if (v < aAABB.GetMinCorner().x)
		{
			sqDist += (aAABB.GetMinCorner().x - v) * (aAABB.GetMinCorner().x - v);
		}
		else if (v > aAABB.GetMaxCorner().x)
		{
			sqDist += (v - aAABB.GetMaxCorner().x) * (v - aAABB.GetMaxCorner().x);
		}

		v = aSphere.GetPosition().y;
		if (v < aAABB.GetMinCorner().y)
		{
			sqDist += (aAABB.GetMinCorner().y - v) * (aAABB.GetMinCorner().y - v);
		}
		else if (v > aAABB.GetMaxCorner().y)
		{
			sqDist += (v - aAABB.GetMaxCorner().y) * (v - aAABB.GetMaxCorner().y);
		}

		v = aSphere.GetPosition().z;
		if (v < aAABB.GetMinCorner().z)
		{
			sqDist += (aAABB.GetMinCorner().z - v) * (aAABB.GetMinCorner().z - v);
		}
		else if (v > aAABB.GetMaxCorner().z)
		{
			sqDist += (v - aAABB.GetMaxCorner().z) * (v - aAABB.GetMaxCorner().z);
		}

		return sqDist <= aSphere.GetRadius() * aSphere.GetRadius();
	}

};