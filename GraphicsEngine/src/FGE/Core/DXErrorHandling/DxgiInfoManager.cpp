#include "GraphicsEngine.pch.h"
#include "DxgiInfoManager.h"
#include "FGE/Core/Window.h"
#include "FGE/Core/DX11.h"

#include <dxgidebug.h>
#include <memory>

#pragma comment(lib, "dxguid.lib")


namespace FGE
{
	DxgiInfoManager::DxgiInfoManager()
	{
		//define function signature of DXGIGetDebugInterface
		typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);

		//load the dll that contains the function DXGIGetDebugInterface
		const auto hModDxgiDebug = LoadLibraryEx(L"dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
		if (hModDxgiDebug == nullptr)
		{
			throw FWND_LAST_EXCEPT();
		}

		//get adress of DXGIGetDebugInterface
		const auto DxgiGetDebugInterface =
			reinterpret_cast<DXGIGetDebugInterface>(GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface"));
		if (DxgiGetDebugInterface == nullptr)
		{
			throw FWND_LAST_EXCEPT();
		}

		HRESULT hr;

		GFX_THROW_NOINFO(DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), reinterpret_cast<void**>(&myPDxgiInfoQueue)));
	}

	DxgiInfoManager::~DxgiInfoManager()
	{
		if (myPDxgiInfoQueue != nullptr)
		{
			myPDxgiInfoQueue->Release();
		}
	}

	void DxgiInfoManager::Set() noexcept
	{
		//set the index (myNext) so that the next all to GetMessages()
		//will only get errors after this call
		myNext = myPDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	}

	std::vector<std::string> DxgiInfoManager::GetMessages() const
	{
		std::vector<std::string> messages;
		const auto end = myPDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
		for (auto i = myNext; i < end; ++i)
		{
			HRESULT hr;
			SIZE_T messageLength;
			//Get the size of message in bytes
			GFX_THROW_NOINFO(myPDxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength));
			// allocate memory for message
			auto bytes = std::make_unique<byte[]>(messageLength);
			auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());
			// get the message and push its description into the vector
			GFX_THROW_NOINFO(myPDxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &messageLength));
			messages.push_back(pMessage->pDescription);

		}
		return messages;
	}

}