#pragma once
#include <tuple>
namespace CommonUtilities
{
	namespace ColorUtilities
	{
		inline std::tuple<int, int, int>RGBToHSV(float aR, float aG, float aB, bool aIsHexFlag = false)
		{
			float h, s, v;
			float r = aR;
			float g = aG;
			float b = aB;

			if (aIsHexFlag) 
			{
				r /= 255;
				g /= 255;
				b /= 255;
			}

			float cMax = std::max(std::max(r, g), b);
			float cMin = std::min(std::min(r, g), b);
			float delta = cMax - cMin;

			if (delta == 0)h = 0;
			else if (cMax == r)h = 60 * fmod((g - b) / delta, 6.f);
			else if (cMax == g)h= 60 * ((b - r) / delta + 2);
			else if (cMax == b)h = 60 * ((r - g) / delta + 4);

			if (cMax == 0)s = 0;
			else s = delta / cMax;

			v = cMax;

			return std::make_tuple(h,s,v);

		}
	}
}