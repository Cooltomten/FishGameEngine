#pragma once
#include "FGE/Rendering/RenderTexture.h"
#include <array>
namespace FGE
{
	class GBuffer
	{
	public: 
		enum GBufferTexture
		{
			GB_Albedo,
			GB_Normal,
			GB_Material,
			GB_VERTEXNORMAL,
			GB_POSITION,
			GB_AMBIENTOCCLUSION,
			GB_COUNT
		};
		void Init();
		void SetAsResource(uint32_t aStartSlot);
		void SetAsTarget(ID3D11DepthStencilView* aDSV);
		void Clear();

	private:
		std::array<RenderTexture, GB_COUNT > myTextures;
	};
}