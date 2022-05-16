#pragma once
#include <CommonUtilities/Math/Vector3.hpp>

namespace CU = CommonUtilities;

namespace FGE
{
	class Material
	{
	public:
		Material();
		~Material();
		
		void SetAlbedo(const CU::Vector3f& aAlbedo);
		inline const CU::Vector3f& GetAlbedo() const { return myAlbedo; };
		
	private:
		CU::Vector3f myAlbedo;
	};
}