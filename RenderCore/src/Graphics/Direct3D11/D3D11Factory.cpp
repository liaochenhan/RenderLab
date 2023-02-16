/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Graphics/Direct3D11/D3D11Factory.hpp"
#include "Diagnostics/Debug.hpp"

namespace TG::Graphics
{
	D3D11Factory::D3D11Factory()
	{
		CheckHResult(CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgiFactory)));
	}

	bool D3D11Factory::EnumAdapter(unsigned int index, AdapterDesc& adapterDesc) const
	{
		winrt::com_ptr<IDXGIAdapter> pAdapter;
		if (m_dxgiFactory->EnumAdapters(index, pAdapter.put()) == DXGI_ERROR_NOT_FOUND);
			return false;

		DXGI_ADAPTER_DESC desc;
		CheckHResult(pAdapter->GetDesc(&desc));
		adapterDesc.description = desc.Description;
		adapterDesc.videoMemory = desc.DedicatedVideoMemory / 1048576;
		return true;
	}

	void D3D11Factory::CreateDeviceAndContext(ICreateInfo const* info, IRenderDevice** ppDevice, IDeviceContext** ppContext) const
	{
		if (typeid(info) != typeid(D3D11CreateInfo))
			throw BaseException(L"Parameter info should be D3D11CreateInfo type");

		D3D11CreateInfo const* d3dInfo = static_cast<D3D11CreateInfo const*>(info);

		winrt::com_ptr<ID3D11Device> d3dDevice;
		winrt::com_ptr<ID3D11DeviceContext> d3dContext;
		UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
		};
		winrt::com_ptr<IDXGIAdapter> pAdapter;
		if (m_dxgiFactory->EnumAdapters(d3dInfo->adapterIndex, pAdapter.put()) == DXGI_ERROR_NOT_FOUND);
			throw BaseException(L"Invalid adapter index");

		CheckHResult(D3D11CreateDevice(pAdapter.get(), D3D_DRIVER_TYPE_UNKNOWN, nullptr, creationFlags,
			featureLevels, 2, D3D11_SDK_VERSION, d3dDevice.put(), nullptr, d3dContext.put()));

		*ppDevice = DBG_NEW RenderDeviceD3D11(*d3dInfo, d3dDevice);
		*ppContext = DBG_NEW DeviceContextD3D11(*d3dInfo, d3dContext);
	}

	void D3D11Factory::CreateSwapChain(IRenderDevice const* pDevice, IDeviceContext const* pContext, const SwapChainDesc& desc, ISwapChain** ppSwapChain) const
	{
		RenderDeviceD3D11 const* d3dDevice = dynamic_cast<RenderDeviceD3D11 const*>(pDevice);
		if (d3dDevice == nullptr)
			throw BaseException(L"pDevice should be RenderDeviceD3D11 type");
		DeviceContextD3D11 const* d3dContext = dynamic_cast<DeviceContextD3D11 const*>(pContext);
		if (d3dContext == nullptr)
			throw BaseException(L"pContext should be DeviceContextD3D11 type");

		DXGI_FORMAT format = TexFormatToDXGIFormat(desc.format);
		UINT numQualityLevels;
		CheckHResult(d3dDevice->device()->CheckMultisampleQualityLevels(format, desc.sampleCount,
			&numQualityLevels));

		DXGI_RATIONAL refreshRate;
		refreshRate.Denominator = 1;
		refreshRate.Numerator = 60;

		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		swapChainDesc.BufferDesc.Width = desc.width;
		swapChainDesc.BufferDesc.Height = desc.height;
		swapChainDesc.BufferDesc.Format = format;
		swapChainDesc.BufferDesc.RefreshRate = refreshRate;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.SampleDesc.Count = desc.sampleCount;
		swapChainDesc.SampleDesc.Quality = numQualityLevels - 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.OutputWindow = d3dContext->hwnd();
		swapChainDesc.Windowed = true;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		winrt::com_ptr<IDXGISwapChain> swapChain;
		CheckHResult(m_dxgiFactory->CreateSwapChain(d3dDevice->device(), &swapChainDesc, swapChain.put()));
		
		*ppSwapChain = DBG_NEW SwapChainD3D11(swapChain);
	}
}
