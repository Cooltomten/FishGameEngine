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
	private:
		friend class TextureImporter;
	public:
		Texture() = default;
		~Texture();

		static AssetType GetStaticType() { return AssetType::Texture; }
		inline AssetType GetAssetType() const override { return GetStaticType(); }

		void SetAsResource(uint32_t aSlot);

	private:
		ComPtr<struct ID3D11ShaderResourceView> mySRV;
		ComPtr<struct ID3D11Resource> myTexture;

	};
}