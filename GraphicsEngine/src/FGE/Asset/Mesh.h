#pragma once
#include <CommonUtilities/Math/Matrix4x4.hpp>

#include "FGE/Asset/Asset.h"

#include <string>
#include <wrl.h>
#include <unordered_map>


namespace CU = CommonUtilities;
using namespace Microsoft::WRL;

struct ID3D11Buffer;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;

namespace FGE
{
	class SubMesh;
	class Material;
	class Mesh : public Asset
	{
	public:
		Mesh() = default;
		~Mesh() = default;
		static AssetType GetStaticType() { return AssetType::Mesh; }
		inline AssetType GetAssetType() const override { return GetStaticType(); }

		void Init(std::vector<std::shared_ptr<SubMesh>> aSubMesh);

		void Render(const CU::Matrix4x4<float>& aTransform, const std::vector<std::shared_ptr<Material>>& someMaterials);

	private:

		std::vector<std::shared_ptr<SubMesh>> mySubMeshes;

	};
}