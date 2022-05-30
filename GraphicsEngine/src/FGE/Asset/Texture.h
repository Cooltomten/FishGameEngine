#pragma once
//for Comptr
#include <wrl.h>
using namespace Microsoft::WRL;

struct ID3D11Resource;
struct ID3D11ShaderResourceView;


#include "Asset.h"

namespace FGE
{
	class Texture : public Asset
	{
		public:
		Texture() = default;
		~Texture() = default;

		static AssetType GetStaticType() { return AssetType::AnimatedMesh; }
		inline AssetType GetAssetType() const override { return GetStaticType(); }

		void SetAsResource();

	private:
		ComPtr<struct ID3D11ShaderResourceView> mySRV;
		ComPtr<struct ID3D11Resource> myTexture;
		
	};
}