/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Graphics/Direct3D11/RenderDeviceD3D11.hpp"

namespace TG::Graphics
{
	RenderDeviceD3D11::RenderDeviceD3D11(const D3D11CreateInfo& info, const winrt::com_ptr<ID3D11Device>& device)
	{
		m_d3dDevice = device;
	}
}
