#pragma once
//for ComPtr
#include <wrl.h>

using namespace Microsoft::WRL;

#include <CommonUtilities/Math/Vector3.hpp>
#include <array>
#include <FGE/Asset/Asset.h>




namespace CU = CommonUtilities;

struct ID3D11Resource;

namespace FGE
{
	namespace MaterialTextureChannel
	{
		enum 
		{
			Albedo,
			Normal,
			Material,
			COUNT
		};
	}

	class Texture;
	class Material : public Asset
	{

	public:
		struct MaterialData
		{
			alignas(16)CU::Vector3f Albedo;
			alignas(16)CU::Vector3f Normal;

		};
	public:

		Material();
		~Material();

			static AssetType GetStaticType() { return AssetType::Material; }
		inline AssetType GetAssetType() const override { return GetStaticType(); }

		void Init();

		void SetAsResource(ComPtr<ID3D11Resource> aMaterialBuffer);

		void SetTexture(uint32_t aMaterialTextureChannel, std::shared_ptr<Texture> aTexture);

		void SetAlbedo(const CU::Vector3f& aAlbedo);
		inline const CU::Vector3f& GetAlbedo() const { return myMaterialData.Albedo; };

	private:
		//friend class MaterialImporter;
		MaterialData myMaterialData;
		std::array < std::shared_ptr<Texture>, MaterialTextureChannel::COUNT> myTextures;

	public:
		static std::shared_ptr<Material> Default;
	};
}