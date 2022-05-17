#pragma once
#include <CommonUtilities/Math/Matrix4x4.hpp>

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
	class Mesh
	{
	public:
		Mesh() = default;
		~Mesh() = default;

		void Init(std::string aPath,std::vector<std::shared_ptr<SubMesh>> aSubMesh);
		FORCEINLINE std::string const& GetName() const{return myName;}

		void Render(const CU::Matrix4x4<float>& aTransform);

		void SetMaterial(std::shared_ptr<Material> aMaterial, uint32_t aIndex);
		std::shared_ptr<Material> GetMaterial(uint32_t aIndex);
		
	private:
		std::string myName;
		
		std::vector<std::shared_ptr<SubMesh>> mySubMeshes;

		std::unordered_map<uint32_t, std::shared_ptr<Material>> myMaterials;
	};
}