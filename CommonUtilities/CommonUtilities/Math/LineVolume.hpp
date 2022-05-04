#pragma once
#include <vector>
#include "Line.hpp"
namespace CommonUtilities
{
	template<typename T>
	class LineVolume
	{
	public:
		// Default constructor: empty LineVolume.
		LineVolume() = default;
		// Constructor taking a list of Line that makes up the LineVolume.
		LineVolume(const std::vector<Line<T>>& aLineList);
		// Add a Line to the LineVolume.
		void AddLine(const Line<T>& aLine);
		// Returns whether a point is inside the LineVolume: it is inside when the point is
		// on the line or on the side the normal is pointing away from for all the lines in
		// the LineVolume.
		bool IsInside(const Vector2<T>& aPosition);
	private:
		std::vector<Line<T>> myLines;
	};

	template<typename T>
	inline LineVolume<T>::LineVolume(const std::vector<Line<T>>& aLineList)
	{
		myLines = aLineList;
	}

	template<typename T>
	inline void LineVolume<T>::AddLine(const Line<T>& aLine)
	{
		myLines.push_back(aLine);
	}

	template<typename T>
	inline bool LineVolume<T>::IsInside(const Vector2<T>& aPosition)
	{

		for (int i = 0; i < myLines.size(); i++)
		{
			if (!myLines[i].IsInside(aPosition))
			{
				return false;
			}
		}
		return true;
	}

}