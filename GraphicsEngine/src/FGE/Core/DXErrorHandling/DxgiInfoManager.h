#pragma once
#include "FabWin.h"
#include <vector>
#include <dxgidebug.h>
#include <string>
namespace FGE
{
	class DxgiInfoManager
	{
	public:
		DxgiInfoManager();
		~DxgiInfoManager();
		DxgiInfoManager(const DxgiInfoManager&) = delete;
		DxgiInfoManager& operator=(const DxgiInfoManager&) = delete;
		void Set() noexcept;
		std::vector<std::string> GetMessages() const;

	private:
		unsigned long long myNext = 0u;
		struct IDXGIInfoQueue* myPDxgiInfoQueue = nullptr;

	};
}
#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw DX11::HrException( __LINE__,__FILE__,hr )

