#pragma once
#include <CommonUtilities/Math/Matrix4x4.hpp>
#include <string>
#include <wrl.h>

namespace CU = CommonUtilities;
using namespace Microsoft::WRL;

struct ID3D11Buffer;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;

namespace FGE
{
	class SubMesh;
	class Mesh
	{
	public:
		Mesh() = default;
		~Mesh() = default;

		void Init(std::string aPath,std::vector<std::shared_ptr<SubMesh>> aSubMesh);
		FORCEINLINE std::string const& GetName() const{return myName;}
		//FORCEINLINE std::shared_ptr<VertexArray> GetVertexArray() const{return myVertexArray;}

		void Render(const CU::Matrix4x4<float>& aTransform);

		
	private:
		std::string myName;
		
		std::vector<std::shared_ptr<SubMesh>> mySubMeshes;
	};
}