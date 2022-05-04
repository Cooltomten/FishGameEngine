#pragma once
#include "FGE/Transform.h"
#include <string>
#include <wrl.h>

using namespace Microsoft::WRL;

struct ID3D11Buffer;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;

namespace FGE
{
	class Model
	{
	public:
		struct MeshData
		{
			UINT NumberOfVertices;
			UINT NumberOfIndices;
			UINT Stride;
			UINT Offset;
			ComPtr<ID3D11Buffer> VertexBuffer;
			ComPtr<ID3D11Buffer> IndexBuffer;
			ComPtr<ID3D11VertexShader> VertexShader;
			ComPtr<ID3D11PixelShader> PixelShader;
			ComPtr<ID3D11InputLayout> InputLayout;
			UINT PrimitiveTopology;
		};

	public:
		Model() = default;
		~Model() = default;

		void Init(const MeshData& someMeshData, std::string aPath);
		FORCEINLINE MeshData const& GetMeshData() const { return myMeshData; };
		FORCEINLINE std::string const& GetName() const{return myName;}
		
		Transform& GetTransform();

		
	private:
		MeshData myMeshData;
		std::string myName;
		Transform myTransform;
	};
}